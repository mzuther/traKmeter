#! /usr/bin/env bash

#  ----------------------------------------------------------------------------
#  
#  traKmeter
#  =========
#  Loudness meter for correctly setting up tracking and mixing levels
#  
#  Copyright (c) 2012-2021 Martin Zuther (http://www.mzuther.de/)
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
#  ----------------------------------------------------------------------------


###############################################################################
#
#  WARNING: this file was auto-generated, please do not edit!
#
###############################################################################

binary_dir="./final"


function finalise_binary
{
    input_file=$1
    output_file="$binary_dir/$input_file"

    if [ -f "./$input_file" ]; then
        if [ ! -f "$output_file" ] || [ "./$input_file" -nt "$output_file" ]; then
            printf "  Finalising binary:   %s -->\n" "$input_file"
            printf "                       %s\n"     "$output_file"

            mkdir -p "$(dirname "./$output_file")"
            cp "./$input_file" "./$output_file"

            printf "\n"
        fi
    fi
}


function finalise_symbols
{
    input_file=$1
    output_file="$binary_dir/debug_symbols/$input_file"

    if [ -f "./$input_file" ]; then
        if [ ! -f "$output_file" ] || [ "./$input_file" -nt "$output_file" ]; then
            printf "  Finalising symbols:  %s -->\n" "$input_file"
            printf "                       %s\n"     "$output_file"

            mkdir -p "$(dirname "./$output_file")"
            cp "./$input_file" "./$output_file"

            printf "\n"
        fi
    fi
}


printf "\n  === Finalising binaries ===\n\n"

mkdir -p "$binary_dir/debug_symbols"

finalise_binary  "standalone/trakmeter_x64"
finalise_binary  "standalone/trakmeter_8ch_x64"


finalise_binary  "vst2/trakmeter_vst2_x64.so"
finalise_binary  "vst2/trakmeter_vst2_8ch_x64.so"

finalise_binary  "vst3/traKmeter.vst3/Contents/x86_64-linux/traKmeter.so"

finalise_binary  "standalone/traKmeter.exe"
finalise_symbols "standalone/traKmeter.pdb"
finalise_binary  "standalone/traKmeter (8ch).exe"
finalise_symbols "standalone/traKmeter (8ch).pdb"

finalise_binary  "vst2/traKmeter.dll"
finalise_symbols "vst2/traKmeter.pdb"
finalise_binary  "vst2/traKmeter (8ch).dll"
finalise_symbols "vst2/traKmeter (8ch).pdb"

finalise_binary  "vst3/traKmeter.vst3/Contents/x86-win/traKmeter.vst3"
finalise_symbols "vst3/traKmeter.vst3/Contents/x86-win/traKmeter.pdb"

finalise_binary  "standalone/traKmeter x64.exe"
finalise_symbols "standalone/traKmeter x64.pdb"
finalise_binary  "standalone/traKmeter (8ch) x64.exe"
finalise_symbols "standalone/traKmeter (8ch) x64.pdb"

finalise_binary  "vst2/traKmeter x64.dll"
finalise_symbols "vst2/traKmeter x64.pdb"
finalise_binary  "vst2/traKmeter (8ch) x64.dll"
finalise_symbols "vst2/traKmeter (8ch) x64.pdb"

finalise_binary  "vst3/traKmeter.vst3/Contents/x86_64-win/traKmeter.vst3"
finalise_symbols "vst3/traKmeter.vst3/Contents/x86_64-win/traKmeter.pdb"

printf "  Done.\n\n\n"
