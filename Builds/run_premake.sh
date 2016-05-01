#!/bin/bash

# ----------------------------------------------------------------------------
#
#  traKmeter
#  =========
#  Loudness meter for correctly setting up tracking and mixing levels
#
#  Copyright (c) 2012-2016 Martin Zuther (http://www.mzuther.de/)
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

visual_toolkit="Windows7.1SDK"


cd $(dirname $0)

echo
premake4 --os=windows vs2010
python3 visual_studio_fix.py "$visual_toolkit"

echo
premake4 --cc=gcc --os=linux gmake

echo
