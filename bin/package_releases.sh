#!/bin/bash

# ----------------------------------------------------------------------------
#
#  traKmeter
#  =========
#  Loudness meter for correctly setting up tracking and mixing levels
#
#  Copyright (c) 2012-2014 Martin Zuther (http://www.mzuther.de/)
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

TRAKMETER_VERSION="1.05"

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
	else
		echo "  Old archive \"$1\" not found."
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

TRAKMETER_ARCHIVE_DIR="trakmeter-standalone_$TRAKMETER_VERSION"

echo "  === GNU/Linux Standalone v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "trakmeter_stereo"
move_new_executable "trakmeter_multi"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux32/trakmeter-standalone_$TRAKMETER_VERSION.tar.bz2"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux32/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux LV2 (32 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-lv2_$TRAKMETER_VERSION"
TRAKMETER_LV2_DIR="trakmeter_lv2"

echo "  === GNU/Linux LV2 v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "$TRAKMETER_LV2_DIR/trakmeter_stereo_lv2.so"
move_new_executable "$TRAKMETER_LV2_DIR/trakmeter_multi_lv2.so"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux32/trakmeter-lv2_$TRAKMETER_VERSION.tar.bz2"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo_lv2.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi_lv2.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_LV2_DIR/manifest.ttl" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_LV2_DIR/trakmeter_stereo.ttl" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_LV2_DIR/trakmeter_multi.ttl" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux32/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux VST (32 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-vst_$TRAKMETER_VERSION"

echo "  === GNU/Linux VST v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "trakmeter_stereo_vst.so"
move_new_executable "trakmeter_multi_vst.so"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux32/trakmeter-vst_$TRAKMETER_VERSION.tar.bz2"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo_vst.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi_vst.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux32/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux Standalone (64 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-standalone_$TRAKMETER_VERSION"

echo "  === GNU/Linux Standalone v$TRAKMETER_VERSION (64 bit) ==="
echo

move_new_executable "trakmeter_stereo_x64"
move_new_executable "trakmeter_multi_x64"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux64/trakmeter-standalone_$TRAKMETER_VERSION.tar.bz2"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo_x64" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi_x64" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux64/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux LV2 (64 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-lv2_$TRAKMETER_VERSION"
TRAKMETER_LV2_DIR="trakmeter_lv2_x64"

echo "  === GNU/Linux LV2 v$TRAKMETER_VERSION (64 bit) ==="
echo

move_new_executable "$TRAKMETER_LV2_DIR/trakmeter_stereo_lv2_x64.so"
move_new_executable "$TRAKMETER_LV2_DIR/trakmeter_multi_lv2_x64.so"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux64/trakmeter-lv2_$TRAKMETER_VERSION.tar.bz2"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo_lv2_x64.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi_lv2_x64.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_LV2_DIR/manifest.ttl" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_LV2_DIR/trakmeter_stereo.ttl" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_LV2_DIR/trakmeter_multi.ttl" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux64/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux VST (64 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-vst_$TRAKMETER_VERSION"

echo "  === GNU/Linux VST v$TRAKMETER_VERSION (64 bit) ==="
echo

move_new_executable "trakmeter_stereo_vst_x64.so"
move_new_executable "trakmeter_multi_vst_x64.so"

delete_old_archive "$TRAKMETER_RELEASE_DIR/linux64/trakmeter-vst_$TRAKMETER_VERSION.tar.bz2"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_stereo_vst_x64.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/trakmeter_multi_vst_x64.so" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/linux64/$TRAKMETER_ARCHIVE_DIR.tar.bz2" "$TRAKMETER_ARCHIVE_DIR" "bzip2"


# ----- Windows Standalone (32 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-standalone_$TRAKMETER_VERSION"

echo "  === Windows Standalone v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "traKmeter (Stereo).exe"
move_new_executable "traKmeter (Multi).exe"

delete_old_archive "$TRAKMETER_RELEASE_DIR/w32/trakmeter-standalone_$TRAKMETER_VERSION.zip"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Stereo).exe" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Multi).exe" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/w32/$TRAKMETER_ARCHIVE_DIR.zip" "$TRAKMETER_ARCHIVE_DIR" "zip"


# ----- Windows VST (32 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-vst_$TRAKMETER_VERSION"

echo "  === Windows VST v$TRAKMETER_VERSION (32 bit) ==="
echo

move_new_executable "traKmeter (Stereo).dll"
move_new_executable "traKmeter (Multi).dll"

delete_old_archive "$TRAKMETER_RELEASE_DIR/w32/trakmeter-vst_$TRAKMETER_VERSION.zip"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Stereo).dll" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Multi).dll" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/w32/$TRAKMETER_ARCHIVE_DIR.zip" "$TRAKMETER_ARCHIVE_DIR" "zip"


# ----- Windows Standalone (64 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-standalone_$TRAKMETER_VERSION"

echo "  === Windows Standalone v$TRAKMETER_VERSION (64 bit) ==="
echo

move_new_executable "traKmeter (Stereo x64).exe"
move_new_executable "traKmeter (Multi x64).exe"

delete_old_archive "$TRAKMETER_RELEASE_DIR/w64/trakmeter-standalone_$TRAKMETER_VERSION.zip"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Stereo x64).exe" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Multi x64).exe" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/w64/$TRAKMETER_ARCHIVE_DIR.zip" "$TRAKMETER_ARCHIVE_DIR" "zip"


# ----- Windows VST (64 bit) -----

TRAKMETER_ARCHIVE_DIR="trakmeter-vst_$TRAKMETER_VERSION"

echo "  === Windows VST v$TRAKMETER_VERSION (64 bit) ==="
echo

move_new_executable "traKmeter (Stereo x64).dll"
move_new_executable "traKmeter (Multi x64).dll"

delete_old_archive "$TRAKMETER_RELEASE_DIR/w64/trakmeter-vst_$TRAKMETER_VERSION.zip"

create_new_archive "$TRAKMETER_ARCHIVE_DIR"

fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Stereo x64).dll" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_EXECUTABLE_DIR/traKmeter (Multi x64).dll" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/LICENSE" "$TRAKMETER_ARCHIVE_DIR"
fill_archive "$TRAKMETER_DOCUMENTATION_DIR/trakmeter.pdf" "$TRAKMETER_ARCHIVE_DIR"

compress_new_archive "$TRAKMETER_RELEASE_DIR/w64/$TRAKMETER_ARCHIVE_DIR.zip" "$TRAKMETER_ARCHIVE_DIR" "zip"
