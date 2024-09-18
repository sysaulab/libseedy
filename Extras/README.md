# Makefile build

This will build the optional tools that are not part of the library.
The libseedy headers files must be installed before making the tools.

You can read more about them in [src/tools](../../src/tools/).

## Instructions

Type the following commands from this directory.

	make                   <--- Build the programs and collect the files in folders here.
	sudo make install      <--- Need root access, Install to /usr/local.
	make clean             <--- Remove build files.

		*** At this point, most people use the program and just forget about it ***

	sudo make uninstall    <--- To completely remove the programs from your system.
