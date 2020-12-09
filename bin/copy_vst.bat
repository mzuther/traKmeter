@echo off

rem  ----------------------------------------------------------------------------
rem  
rem  traKmeter
rem  =========
rem  Loudness meter for correctly setting up tracking and mixing levels
rem  
rem  Copyright (c) 2012-2020 Martin Zuther (http://www.mzuther.de/)
rem  
rem  This program is free software: you can redistribute it and/or modify
rem  it under the terms of the GNU General Public License as published by
rem  the Free Software Foundation, either version 3 of the License, or
rem  (at your option) any later version.
rem  
rem  This program is distributed in the hope that it will be useful,
rem  but WITHOUT ANY WARRANTY; without even the implied warranty of
rem  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem  GNU General Public License for more details.
rem  
rem  You should have received a copy of the GNU General Public License
rem  along with this program.  If not, see <http://www.gnu.org/licenses/>.
rem  
rem  Thank you for using free software!
rem  
rem  ----------------------------------------------------------------------------


rem  ############################################################################
rem
rem  WARNING: this file was auto-generated, please do not edit!
rem
rem  ############################################################################

setlocal

set rsync_path="E:\Documents\System\Tools\rsync\bin"
set rsync_cmd="%rsync_path%\rsync.exe" --archive

set vst2_32=/cygdrive/c/Program Files (x86)/Steinberg/VSTPlugins/radix/
set vst3_32=/cygdrive/c/Program Files (x86)/Common Files/VST3/radix/
set vst2_64=/cygdrive/c/Program Files/Steinberg/VSTPlugins/radix/
set vst3_64=/cygdrive/c/Program Files/Common Files/VST3/radix/

set vst2_32_categories="/cygdrive/d/Plugins/32-bit/Categories/Tools/Analyzer/Meter"
set vst2_64_categories="/cygdrive/d/Plugins/64-bit/Categories/Tools/Analyzer/Meter"


echo.
echo VST2 (32 bit)
echo.

call :CopyVst       "vst2/traKmeter (Stereo).dll" "%vst2_32%"
call :CopyVst       "vst2/traKmeter (Multi).dll" "%vst2_32%"
call :CopyVst       "vst2/Documentation/traKmeter.pdf" "%vst2_32%"

echo.
echo VST2 (32 bit, Categories)
echo.

call :CopyVst       "vst2/traKmeter (Stereo).dll" "%vst2_32_categories%"
call :CopyVst       "vst2/traKmeter (Multi).dll" "%vst2_32_categories%"
call :CopyVst       "vst2/Documentation/traKmeter.pdf" "%vst2_32_categories%"


echo.
echo VST3 (32 bit)
echo.

call :CopyVstDelete "vst3/trakmeter.vst3" "%vst3_32%"


echo.
echo VST2 (64 bit)
echo.

call :CopyVst       "vst2/traKmeter (Stereo x64).dll" "%vst2_64%"
call :CopyVst       "vst2/traKmeter (Multi x64).dll" "%vst2_64%"
call :CopyVst       "vst2/Documentation/traKmeter.pdf" "%vst2_64%"

echo.
echo VST2 (64 bit, Categories)
echo.

call :CopyVst       "vst2/traKmeter (Stereo x64).dll" "%vst2_64_categories%"
call :CopyVst       "vst2/traKmeter (Multi x64).dll" "%vst2_64_categories%"
call :CopyVst       "vst2/Documentation/traKmeter.pdf" "%vst2_64_categories%"


echo.
echo VST3 (64 bit)
echo.

call :CopyVstDelete "vst3/trakmeter.vst3" "%vst3_64%"


echo.
pause
exit /b %errorlevel%


:CopyVst
set source=%~1
set destination=%~2

echo CopyVst       %source%
%rsync_cmd% "%source%" "%destination%"

exit /b 0


:CopyVstDelete
set source=%~1
set destination=%~2

echo CopyVstDelete %source%
%rsync_cmd% --delete --exclude="*-linux" "%source%" "%destination%"

exit /b 0
