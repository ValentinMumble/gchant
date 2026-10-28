#!/bin/bash
# Builds a standalone GChant.app (GTK 2 libraries included) into build/,
# plus build/GChant-macOS.zip (the app, to attach to a GitHub release).
# Needs gtkmm 2.4, pkg-config and dylibbundler:
#   brew install gtkmm pkgconf dylibbundler
set -euo pipefail

cd "$(dirname "$0")"
version=1.0.0
brew_prefix="$(brew --prefix)"
app=build/GChant.app
# macOS refuses writes into a folder named .app once it holds a program, so the
# app is put together in a plain folder and renamed at the end
staging=build/staging
resources="$staging/Contents/Resources"

make -s

rm -rf build
mkdir -p "$staging/Contents/MacOS" "$resources/loaders" "$staging/Contents/Frameworks"
cp GChant "$staging/Contents/MacOS/gchant-bin"
cp icon.gif "$resources/"
cp -R samples "$resources/"

# The window icon is a GIF, and gdk-pixbuf loads GIFs through a plug-in.
# List it now, while it can still find its Homebrew libraries, with the bundle
# path replaced by a placeholder that the launcher fills in at start.
cp "$brew_prefix/lib/gdk-pixbuf-2.0/2.10.0/loaders/libpixbufloader-gif.so" "$resources/loaders/"
"$brew_prefix/bin/gdk-pixbuf-query-loaders" "$PWD/$resources/loaders/libpixbufloader-gif.so" \
    | sed "s#$PWD/$resources#@RESOURCES@#g" > "$resources/loaders.cache.in"

# Copy every Homebrew library the app and the plug-in use into Frameworks
dylibbundler -of -b -cd -s "$brew_prefix/lib" \
    -x "$staging/Contents/MacOS/gchant-bin" -x "$resources/loaders/libpixbufloader-gif.so" \
    -d "$staging/Contents/Frameworks" -p @executable_path/../Frameworks/ </dev/null >/dev/null

# The app reads icon.gif and writes gchant.log in the folder it starts in,
# so the launcher starts it from Resources
cat > "$staging/Contents/MacOS/GChant" <<'LAUNCHER'
#!/bin/sh
resources="$(cd "$(dirname "$0")/../Resources" && pwd)"
cache="${TMPDIR:-/tmp}/gchant-loaders.cache"
sed "s#@RESOURCES@#$resources#g" "$resources/loaders.cache.in" > "$cache"
export GDK_PIXBUF_MODULE_FILE="$cache"
cd "$resources"
exec "$resources/../MacOS/gchant-bin" "$@"
LAUNCHER
chmod +x "$staging/Contents/MacOS/GChant"

# App icon, from the original pixel-art icon
iconset=build/GChant.iconset
mkdir -p "$iconset"
for size in 16 32 128 256 512; do
    sips -z "$size" "$size" app-icon.png --out "$iconset/icon_${size}x${size}.png" >/dev/null
    double=$((size * 2))
    sips -z "$double" "$double" app-icon.png --out "$iconset/icon_${size}x${size}@2x.png" >/dev/null
done
iconutil -c icns "$iconset" -o "$resources/GChant.icns"

cat > "$staging/Contents/Info.plist" <<PLIST
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleName</key><string>GChant</string>
    <key>CFBundleDisplayName</key><string>GChant</string>
    <key>CFBundleIdentifier</key><string>com.valentinmumble.gchant</string>
    <key>CFBundleExecutable</key><string>GChant</string>
    <key>CFBundleIconFile</key><string>GChant</string>
    <key>CFBundlePackageType</key><string>APPL</string>
    <key>CFBundleShortVersionString</key><string>$version</string>
    <key>CFBundleVersion</key><string>$version</string>
    <key>LSMinimumSystemVersion</key><string>11.0</string>
    <key>NSHighResolutionCapable</key><true/>
</dict>
</plist>
PLIST

mv "$staging" "$app"

# Changing the libraries' paths broke their signatures; sign everything again
codesign --force --deep --sign - "$app"

# ditto keeps the app bundle's symlinks and permissions intact
ditto -c -k --keepParent "$app" build/GChant-macOS.zip

echo "Built $app and build/GChant-macOS.zip"
echo "Install it with: cp -R $app /Applications/"
