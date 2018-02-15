@echo off

@setlocal
@set rsync_path=E:\Documents\System\Tools\rsync\bin
@set rsync_cmd=%rsync_path%\rsync.exe --archive

@set destination=/cygdrive/d/Vst/32bit/radix/
@echo %destination%
@%rsync_cmd% --delete "vst/trakmeter"                    "%destination%"
@%rsync_cmd%          "vst/traKmeter (Stereo).dll"       "%destination%"
@%rsync_cmd%          "vst/traKmeter (Multi).dll"        "%destination%"

@set destination=/cygdrive/d/Vst/Categories/Analyzer/Meter/
@echo %destination%
@%rsync_cmd% --delete "vst/trakmeter"                    "%destination%"
@%rsync_cmd%          "vst/traKmeter (Stereo).dll"       "%destination%"
@%rsync_cmd%          "vst/traKmeter (Multi).dll"        "%destination%"

@set destination=/cygdrive/d/Plugins/VST2 (32 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "vst/trakmeter"                    "%destination%"
@%rsync_cmd%          "vst/traKmeter (Stereo).dll"       "%destination%"
@%rsync_cmd%          "vst/traKmeter (Multi).dll"        "%destination%"

@set destination=/cygdrive/d/Plugins/VST2 (64 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "vst/trakmeter"                    "%destination%"
@%rsync_cmd%          "vst/traKmeter (Stereo x64).dll"   "%destination%"
@%rsync_cmd%          "vst/traKmeter (Multi x64).dll"    "%destination%"

@set destination=/cygdrive/d/Plugins/VST3 (32 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "vst3/trakmeter"                    "%destination%"
@%rsync_cmd%          "vst3/traKmeter (Stereo).vst3"      "%destination%"
@%rsync_cmd%          "vst3/traKmeter (Multi).vst3"       "%destination%"

@set destination=/cygdrive/d/Plugins/VST3 (64 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "vst3/trakmeter"                    "%destination%"
@%rsync_cmd%          "vst3/traKmeter (Stereo x64).vst3"  "%destination%"
@%rsync_cmd%          "vst3/traKmeter (Multi x64).vst3"   "%destination%"

@echo.
@pause
