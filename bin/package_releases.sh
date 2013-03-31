#!/bin/bash

# ----------------------------------------------------------------------------
#
#  traKmeter
#  =========
#  Loudness meter for correctly setting up tracking and mixing levels
#
#  Copyright (c) 2012-2013 Martin Zuther (http://www.mzuther.de/)
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Thank you for using free software!
#
# ----------------------------------------------------------------------------

TRAKMETER_VERSION="1.04"

TRAKMETER_EXECUTABLE_DIR="final"
TRAKMETER_RELEASE_DIR="releases"
TRAKMETER_DOCUMENTATION_DIR="../doc"

function copy_new_executable
{
	if [ -f "$1" ]; then
		echo "  Finalising file $1..."
		cp "$1" "$TRAKMETER_EXECUTABLE_DIR"/
	fi
}

function move_new_executable
{
	if [ -f "$1" ]; then
		echo "  Finalising file $1..."
		mv "$1" "$TRAKMETER_EXECUTABLE_DIR"/
	fi
}

function fill_archive
{
	if [ ! -d "$2" ]; then
		mkdir -p "$2"
	fi

	if [ -f "$1" ]; then
		echo "    $1"
		cp "$1" "$2"
	fi
}

function delete_old_archive
{
	if [ -f "$1" ]; then
		echo "  Deleting old archive \"$1\"..."
		rm "$1"
	fi
}

function create_new_archive
{
	echo "  Creating folder \"$1\"..."
	echo "  Copying files to \"$1\"..."
	mkdir -p "$1"
	echo
}

function compress_new_archive
{
	echo
	echo "  Creating archive \"$1\"..."
	echo

	if [ "$3" = "bzip2" ]; then
		tar --create --bzip2 --verbose --file "$1" "$2"/* | gawk ' { print "    adding: " $1 } '
	elif [ "$3" = "zip" ]; then
		zip --recurse-paths "$1" "$2"/* | gawk ' { print "  " $0 } '
	fi

	echo
	echo "  Removing folder \"$2\"..."

	rm -r "$2"/

	echo "  Done."
	echo
}

echo


# ----- GNU/Linux Standalone (32 bit) -----

echo "  === GNU/Linux Standalone v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "trakmeter_stereo"
move_new_executable "trakmeter_multi"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux32/trakmeter-standalone.tar.bz2"

TRAKMETER_ARCHIVE_DIR="trakmeter-standalone_$TRAKMETER_VERSION"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux32/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux VST (32 bit) -----

echo "  === GNU/Linux VST v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "trakmeter_stereo.so"
move_new_executable "trakmeter_multi.so"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux32/trakmeter-vst.tar.bz2"

TRAKMETER_ARCHIVE_DIR="trakmeter-vst_$TRAKMETER_VERSION"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux32/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux Standalone (64 bit) -----

echo "  === GNU/Linux Standalone v$TRAKMETER_VERSION (64 bit) ==="
echo

move_new_executable "trakmeter_stereo_x64"
move_new_executable "trakmeter_multi_x64"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux64/trakmeter-standalone.tar.bz2"

TRAKMETER_ARCHIVE_DIR="trakmeter-standalone_$TRAKMETER_VERSION"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo_x64" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi_x64" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux64/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux VST (64 bit) -----

echo "  === GNU/Linux VST v$TRAKMETER_VERSION (64 bit) ==="
echo

move_new_executable "trakmeter_stereo_x64.so"
move_new_executable "trakmeter_multi_x64.so"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux64/trakmeter-vst.tar.bz2"

TRAKMETER_ARCHIVE_DIR="trakmeter-vst_$TRAKMETER_VERSION"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo_x64.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi_x64.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux64/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- Windows Standalone (32 bit) -----

echo "  === Windows Standalone v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "traKmeter (Stereo).exe"
move_new_executable "traKmeter (Multi).exe"

delete_old_archive "$TRAKMETER_RELEASE_DIR/w32/trakmeter-standalone.zip"

TRAKMETER_ARCHIVE_DIR="trakmeter-standalone_$TRAKMETER_VERSION"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Stereo).exe" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Multi).exe" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/w32/$TRAKMETER_ARCHIVE_DIR.zip" "$TRAKMETER_ARCHIVE_DIR" "zip"


# ----- Windows VST (32 bit) -----

echo "  === Windows VST v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "traKmeter (Stereo).dll"
move_new_executable "traKmeter (Multi).dll"

delete_old_archive "$TRAKMETER_RELEASE_DIR/w32/trakmeter-vst.zip"

TRAKMETER_ARCHIVE_DIR="trakmeter-vst_$TRAKMETER_VERSION"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Stereo).dll" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Multi).dll" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/w32/$TRAKMETER_ARCHIVE_DIR.zip" "$TRAKMETER_ARCHIVE_DIR" "zip"
