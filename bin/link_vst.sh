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

destination_dir="$HOME/.vst/trakmeter"
use_final_binaries=0

resource_dir="trakmeter"
source_dir_current=$(realpath "$(dirname "$0")/vst2")
source_dir_final=$(realpath "$(dirname "$0")/final")


printf "\nCreating VST2 directory"
printf "\n=======================\n"
printf "%s\n" "$destination_dir"

mkdir -p "$destination_dir"


printf "\nUnlinking binaries"
printf "\n==================\n"

find  "$destination_dir" -iname "*_x64.so" -print -execdir rm {} \; | \
    sort


printf "\nLinking resource directory"
printf "\n==========================\n"
printf "%s\n" "$resource_dir"

if [ $use_final_binaries -eq 0 ]; then
    ln -sf "$source_dir_current/$resource_dir" "$destination_dir"
else
    ln -sf "$source_dir_final/$resource_dir" "$destination_dir"
fi


printf "\nLinking debug binaries"
printf "\n======================\n"

find "$source_dir_current" -iname "*vst2_debug_x64.so" -printf "%f\n" | \
    tee /dev/tty | \
    xargs "-d\n" -I{} ln -sf "$source_dir_current"/{} "$destination_dir"/{} | \
    sort


printf "\nLinking release binaries"
printf "\n========================\n"

if [ $use_final_binaries -eq 0 ]; then
    find "$source_dir_current" -iname "*vst2_x64.so" -printf "%f\n" | \
        tee /dev/tty | \
        xargs "-d\n" -I{} ln -sf "$source_dir_current"/{} "$destination_dir"/{} | \
        sort
else
    find "$source_dir_final" -iname "*vst2_x64.so" -printf "%f\n" | \
        tee /dev/tty | \
        xargs "-d\n" -I{} ln -sf "$source_dir_final"/{} "$destination_dir"/{} | \
        sort
fi


printf "\n"