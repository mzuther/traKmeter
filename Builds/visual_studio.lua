--------------------------------------------------------------------------------
--
--  traKmeter
--  =========
--  Loudness meter for correctly setting up tracking and mixing levels
--
--  Copyright (c) 2012-2016 Martin Zuther (http://www.mzuther.de/)
--
--  This program is free software: you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation, either version 3 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program.  If not, see <http://www.gnu.org/licenses/>.
--
--  Thank you for using free software!
--
--------------------------------------------------------------------------------

if not _ACTION then
	-- prevent "attempt to ... (a nil value)" errors
elseif _ACTION == "gmake" then
	print ("=== Generating project files (GNU g++, " .. os.get():upper() .. ") ===")
elseif string.startswith(_ACTION, "codeblocks") then
	print "=== Generating project files (Code::Blocks, Windows) ==="
elseif string.startswith(_ACTION, "vs") then
	print "=== Generating project files (Visual C++, Windows) ==="
elseif string.startswith(_ACTION, "xcode") then
	print "=== Generating project files (Xcode, Mac OS X) ==="
else
	print "Action not specified\n"
end

solution "trakmeter"
	language "C++"
	platforms { "x32", "x64" }
	configurations { "Debug", "Release" }

	location (os.get() .. "/" .. _ACTION .. "/")
	targetdir "../bin/"
	targetprefix ""

	files {
		"../Source/common/FrutHeader.h",
		"../Source/common/amalgamated/*.h",
		"../Source/common/amalgamated/*.cpp",

		"../Source/*.h",
		"../Source/*.cpp",

		"../JuceLibraryCode/juce_audio_basics.cpp",
		"../JuceLibraryCode/juce_audio_devices.cpp",
		"../JuceLibraryCode/juce_audio_formats.cpp",
		"../JuceLibraryCode/juce_audio_plugin_client_utils.cpp",
		"../JuceLibraryCode/juce_audio_processors.cpp",
		"../JuceLibraryCode/juce_audio_utils.cpp",
		"../JuceLibraryCode/juce_core.cpp",
		"../JuceLibraryCode/juce_cryptography.cpp",
		"../JuceLibraryCode/juce_data_structures.cpp",
		"../JuceLibraryCode/juce_events.cpp",
		"../JuceLibraryCode/juce_graphics.cpp",
		"../JuceLibraryCode/juce_gui_basics.cpp",
		"../JuceLibraryCode/juce_gui_extra.cpp",
		"../JuceLibraryCode/juce_video.cpp"
	}

	includedirs {
		"../JuceLibraryCode/",
		"../libraries/juce/modules/",
		"../Source/common/",
		"../libraries/"
	}

	configuration { "windows" }
		defines {
			"_WINDOWS=1",
			"_USE_MATH_DEFINES=1",
		}

		flags {
				"EnableSSE",
				"EnableSSE2",
				"NoMinimalRebuild",
				"StaticRuntime",
				"Unicode",
				"WinMain"
		}

		links {
			"kernel32",
			"user32",
			"gdi32",
			"winspool",
			"comdlg32",
			"advapi32",
			"shell32",
			"ole32",
			"oleaut32",
			"uuid",
			"odbc32",
			"odbccp32"
		 }

	configuration { "windows", "x32" }
		defines {
			"WIN32=1",
		}

	configuration { "windows", "x64" }
		defines {
			"WIN64=1",
		}

	configuration { "Debug*" }
		defines { "_DEBUG=1", "DEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=1" }
		flags { "Symbols" }

	configuration { "windows", "Debug", "x32" }
		targetsuffix ", Debug)"

	configuration { "windows", "Debug", "x64" }
		targetsuffix " x64, Debug)"

	configuration { "Release*" }
		defines { "NDEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=0" }
		flags { "OptimizeSpeed", "NoFramePointer" }

	configuration { "windows", "Release*" }
		flags { "NoManifest" }
		buildoptions { "/Zi" }

	configuration { "windows", "Release", "x32" }
		targetsuffix ")"

	configuration { "windows", "Release", "x64" }
		targetsuffix " x64)"

--------------------------------------------------------------------------------

	project ("trakmeter_standalone_stereo")
		kind "WindowedApp"

		defines {
			"TRAKMETER_STEREO=1",
			"JucePlugin_Build_LV2=0",
			"JucePlugin_Build_Standalone=1",
			"JucePlugin_Build_VST=0"
		}

		configuration { "windows" }
			targetname "K-Meter (Stereo"

			defines {
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=1",
				"JUCE_WASAPI=1",
				"JUCE_DIRECTSOUND=1"
			}

			includedirs {
				"../libraries/asiosdk2.3/common"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_stereo_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_stereo_release")

--------------------------------------------------------------------------------

	project ("trakmeter_standalone_multi")
		kind "WindowedApp"

		defines {
			"TRAKMETER_MULTI=1",
			"JucePlugin_Build_LV2=0",
			"JucePlugin_Build_Standalone=1",
			"JucePlugin_Build_VST=0"
		}

		configuration { "windows" }
			targetname "K-Meter (Multi"

			defines {
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=1",
				"JUCE_WASAPI=1",
				"JUCE_DIRECTSOUND=1"
			}

			includedirs {
				"../libraries/asiosdk2.3/common"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_multi_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_multi_release")

--------------------------------------------------------------------------------

	project ("trakmeter_vst_stereo")
		kind "SharedLib"

		configuration { "windows" }
			targetname "K-Meter (Stereo"

		defines {
			"TRAKMETER_STEREO=1",
			"JucePlugin_Build_LV2=0",
			"JucePlugin_Build_Standalone=0",
			"JucePlugin_Build_VST=1"
		}

		files {
			  "../JuceLibraryCode/juce_audio_plugin_client_VST2.cpp"
		}

		excludes {
			"../Source/standalone_application.h",
			"../Source/standalone_application.cpp"
		}

		defines {
			"JUCE_ALSA=0",
			"JUCE_JACK=0",
			"JUCE_ASIO=0",
			"JUCE_WASAPI=0",
			"JUCE_DIRECTSOUND=0"
		}

		includedirs {
			"../libraries/vstsdk3.6.5"
		}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_stereo_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_stereo_release")

--------------------------------------------------------------------------------

	project ("trakmeter_vst_multi")
		kind "SharedLib"

		configuration { "windows" }
			targetname "K-Meter (Multi"

		defines {
			"TRAKMETER_MULTI=1",
			"JucePlugin_Build_LV2=0",
			"JucePlugin_Build_Standalone=0",
			"JucePlugin_Build_VST=1"
		}

		files {
			  "../JuceLibraryCode/juce_audio_plugin_client_VST2.cpp"
		}

		excludes {
			"../Source/standalone_application.h",
			"../Source/standalone_application.cpp"
		}

		defines {
			"JUCE_ALSA=0",
			"JUCE_JACK=0",
			"JUCE_ASIO=0",
			"JUCE_WASAPI=0",
			"JUCE_DIRECTSOUND=0"
		}

		includedirs {
			"../libraries/vstsdk3.6.5"
		}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_multi_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_multi_release")
