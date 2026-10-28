/*
 * @file main.cpp
 */

#include <iostream>
#include <gtkmm/main.h>

#include "dxfhandler.h"
#include "gchant.h"

using namespace std;

void usage();

/*
 * @brief Main function for DXFLib test program.
 *
 * @param argc Number of delimited items on command line,
 *		including program name.
 * @param argv Pointer to array of command line items
 *
 * @retval 0 if missing input file argument or
 *		file couldn't be opened
 * @retval 1 if file opened
 */

int main (int argc, char** argv) {

	/*// Check given arguments:
	if (argc < 2) {
		usage();
		return 0;
	}*/
	
	/* Rediriger la sortie standard vers ./gchant.log */
	ofstream sortie;
	streambuf *sortie_buffer, *backup;
	
	sortie.open("gchant.log");
	backup=cout.rdbuf();
	
	sortie_buffer = sortie.rdbuf();
	cout.rdbuf(sortie_buffer);

	Gtk::Main kit(argc, argv);

	GChant gchant;
	gchant.set_icon_from_file("icon.gif");
	gchant.maximize();

	// GChant plan.dxf ouvre le plan au demarrage
	if (argc > 1 && argv[1][0] != '-') {
		gchant.ouvrir(argv[1]);
	}

	Gtk::Main::run(gchant);
	
	sortie.close();
	cout.rdbuf(backup);
	
	return 0;
}

/*
 * @brief Prints error message if file name not specified as command
 * line argument.
 */
void usage() {
	cerr << "\nUsage: test <DXF file>\n\n";
}

