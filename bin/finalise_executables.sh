#!/bin/bash

#  ----------------------------------------------------------------------------
#  
#  traKmeter
#  =========
#  Loudness meter for correctly setting up tracking and mixing levels
#  
#  Copyright (c) 2012-2020 Martin Zuther (http://www.mzuther.de/)
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
#  WARNING: this file is auto-generated, please do not edit!
#
###############################################################################

executable_dir="final"


function finalise_executable
{
    filepath=$1
    filename=$(basename "$1")

    if [ -f "./$filepath" ]; then
        echo "  Finalising:  $filepath -->"
        echo "               $executable_dir/$filename"

        mv "./$filepath" "./$executable_dir/$filename"

        echo
    fi
}


function finalise_symbols
{
    filepath=$1
    filename=$(basename "$1")

    if [ -f "./$filepath" ]; then
        echo "  Finalising:  $filepath -->"
        echo "               $executable_dir/debug_symbols/$filepath"

        mkdir -p "$(dirname "./$executable_dir/debug_symbols/$filepath")"
        mv "./$filepath" "./$executable_dir/debug_symbols/$filepath"

        echo
    fi
}


echo
echo "  === Finalising executables ==="
echo

finalise_executable "standalone/trakmeter_stereo"
finalise_executable "standalone/trakmeter_multi"

finalise_executable "vst2/trakmeter_stereo_vst2.so"
finalise_executable "vst2/trakmeter_multi_vst2.so"

finalise_executable "standalone/trakmeter_stereo_x64"
finalise_executable "standalone/trakmeter_multi_x64"

finalise_executable "vst2/trakmeter_stereo_vst2_x64.so"
finalise_executable "vst2/trakmeter_multi_vst2_x64.so"

finalise_executable "standalone/traKmeter (Stereo).exe"
finalise_symbols    "standalone/traKmeter (Stereo).pdb"
finalise_executable "standalone/traKmeter (Multi).exe"
finalise_symbols    "standalone/traKmeter (Multi).pdb"

finalise_executable "vst2/traKmeter (Stereo).dll"
finalise_symbols    "vst2/traKmeter (Stereo).pdb"
finalise_executable "vst2/traKmeter (Multi).dll"
finalise_symbols    "vst2/traKmeter (Multi).pdb"

finalise_executable "vst3/traKmeter (Stereo).vst3"
finalise_symbols    "vst3/traKmeter (Stereo).pdb"
finalise_executable "vst3/traKmeter (Multi).vst3"
finalise_symbols    "vst3/traKmeter (Multi).pdb"

finalise_executable "standalone/traKmeter (Stereo x64).exe"
finalise_symbols    "standalone/traKmeter (Stereo x64).pdb"
finalise_executable "standalone/traKmeter (Multi x64).exe"
finalise_symbols    "standalone/traKmeter (Multi x64).pdb"

finalise_executable "vst2/traKmeter (Stereo x64).dll"
finalise_symbols    "vst2/traKmeter (Stereo x64).pdb"
finalise_executable "vst2/traKmeter (Multi x64).dll"
finalise_symbols    "vst2/traKmeter (Multi x64).pdb"

finalise_executable "vst3/traKmeter (Stereo x64).vst3"
finalise_symbols    "vst3/traKmeter (Stereo x64).pdb"
finalise_executable "vst3/traKmeter (Multi x64).vst3"
finalise_symbols    "vst3/traKmeter (Multi x64).pdb"

echo "  Done."
echo
echo
