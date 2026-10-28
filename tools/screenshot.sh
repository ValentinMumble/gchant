#!/bin/bash
# Opens GChant on a plan and saves its window as a PNG, 1600 px wide (this is
# how the screenshots in docs/ were made). Build with make first.
#   tools/screenshot.sh samples/2d_house_plan.DXF docs/house.png
set -euo pipefail

cd "$(dirname "$0")/.."
plan="$1"
output="$2"

./GChant "$PWD/$plan" &
app=$!
trap 'kill $app 2>/dev/null' EXIT
sleep 4

# The window id screencapture needs, looked up from the app's process id
window=$(swift - <<SWIFT
import CoreGraphics
let windows = CGWindowListCopyWindowInfo(.optionOnScreenOnly, kCGNullWindowID) as? [[String: Any]] ?? []
for window in windows where (window["kCGWindowOwnerPID"] as? Int32) == $app && (window["kCGWindowLayer"] as? Int) == 0 {
    print(window["kCGWindowNumber"]!)
    break
}
SWIFT
)
[ -n "$window" ] || { echo "No GChant window found" >&2; exit 1; }

screencapture -o -l "$window" "$output"
sips --resampleWidth 1600 "$output" >/dev/null
echo "Saved $output"
