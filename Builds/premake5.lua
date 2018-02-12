--------------------------------------------------------------------------------
--
--  traKmeter
--  =========
--  Loudness meter for correctly setting up tracking and mixing levels
--
--  Copyright (c) 2012-2018 Martin Zuther (http://www.mzuther.de/)
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
    print ("=== Generating project files (GNU g++, " .. os.target():upper() .. ") ===")
elseif string.startswith(_ACTION, "codeblocks") then
    print "=== Generating project files (Code::Blocks, Windows) ==="
elseif string.startswith(_ACTION, "vs") then
    print "=== Generating project files (Visual C++, Windows) ==="
elseif string.startswith(_ACTION, "xcode") then
    print "=== Generating project files (Xcode, Mac OS X) ==="
else
    print "Action not specified\n"
end


workspace "trakmeter"
    language "C++"
    platforms { "x32", "x64" }
    configurations { "Debug", "Release" }

    location (os.target() .. "/" .. _ACTION .. "/")
    targetdir "../bin/"
    targetprefix ""

    files {
        "../Source/frut/FrutHeader.h",
        "../Source/frut/amalgamated/*.h",
        "../Source/frut/amalgamated/*.cpp",

        "../Source/*.h",
        "../Source/*.cpp",

        "../JuceLibraryCode/include_juce_audio_basics.cpp",
        "../JuceLibraryCode/include_juce_audio_devices.cpp",
        "../JuceLibraryCode/include_juce_audio_formats.cpp",
        "../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp",
        "../JuceLibraryCode/include_juce_audio_processors.cpp",
        "../JuceLibraryCode/include_juce_audio_utils.cpp",
        "../JuceLibraryCode/include_juce_core.cpp",
        "../JuceLibraryCode/include_juce_cryptography.cpp",
        "../JuceLibraryCode/include_juce_data_structures.cpp",
        "../JuceLibraryCode/include_juce_events.cpp",
        "../JuceLibraryCode/include_juce_graphics.cpp",
        "../JuceLibraryCode/include_juce_gui_basics.cpp",
        "../JuceLibraryCode/include_juce_gui_extra.cpp",
        "../JuceLibraryCode/include_juce_video.cpp"
    }

    includedirs {
        "../JuceLibraryCode/",
        "../libraries/juce/modules/",
        "../Source/frut/",
        "../libraries/"
    }

    cppdialect "C++14"

    filter { "system:linux" }
        defines {
            "LINUX=1"
        }

        includedirs {
            "/usr/include/at-spi-2.0",
            "/usr/include/at-spi2-atk/2.0",
            "/usr/include/atk-1.0",
            "/usr/include/cairo",
            "/usr/include/cairo",
            "/usr/include/dbus-1.0",
            "/usr/include/freetype2",
            "/usr/include/gdk-pixbuf-2.0",
            "/usr/include/gio-unix-2.0/",
            "/usr/include/glib-2.0",
            "/usr/include/gtk-3.0",
            "/usr/include/gtk-3.0",
            "/usr/include/gtk-3.0",
            "/usr/include/harfbuzz",
            "/usr/include/libpng12",
            "/usr/include/libpng12",
            "/usr/include/libsoup-2.4",
            "/usr/include/libxml2",
            "/usr/include/mirclient",
            "/usr/include/mircookie",
            "/usr/include/mircore",
            "/usr/include/pango-1.0",
            "/usr/include/pango-1.0",
            "/usr/include/pixman-1",
            "/usr/include/webkitgtk-4.0",
            "/usr/include/webkitgtk-4.0",
            "/usr/lib/x86_64-linux-gnu/dbus-1.0/include",
            "/usr/lib/x86_64-linux-gnu/glib-2.0/include"
        }

        linkoptions {
            -- check for unresolved symbols in shared libraries
            "-Wl,--no-undefined"
        }

        links {
            "X11",
            "Xext",
            "Xinerama",
            "atk-1.0",
            "cairo",
            "cairo-gobject",
            "curl",
            "dl",
            "freetype",
            "gdk-3",
            "gdk_pixbuf-2.0",
            "gio-2.0",
            "glib-2.0",
            "gobject-2.0",
            "gtk-3",
            "gtk-3",
            "javascriptcoregtk-4.0",
            "pango-1.0",
            "pangocairo-1.0",
            "pthread",
            "rt",
            "soup-2.4",
            "webkit2gtk-4.0"
        }

    filter { "system:windows" }
        defines {
            "_WINDOWS=1",
            "_USE_MATH_DEFINES=1",
        }

        systemversion "10.0.16299.0"

        flags {
            "NoMinimalRebuild",
            "StaticRuntime"
        }

        characterset "Unicode"
        vectorextensions "AVX"

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

    filter { "platforms:x32" }
        architecture "x86"

    filter { "platforms:x64" }
        architecture "x86_64"

    filter { "system:windows", "platforms:x32" }
        defines {
            "WIN32=1",
        }

    filter { "system:windows", "platforms:x64" }
        defines {
            "WIN64=1",
        }

    filter { "configurations:Debug" }
        defines { "_DEBUG=1", "DEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=1" }
        symbols "On"

    filter { "system:linux", "configurations:Debug" }
        warnings "Extra"
        buildoptions { "-fmessage-length=78" }

    filter { "system:linux", "configurations:Debug" }
        warnings "Extra"
        buildoptions { "-fno-inline", "-ggdb" }

    filter { "system:linux", "configurations:Debug", "platforms:x32" }
        targetsuffix "_debug"

    filter { "system:linux", "configurations:Debug", "platforms:x64" }
        targetsuffix "_debug_x64"

    filter { "system:windows", "configurations:Debug" }
        symbols "Full"

    filter { "system:windows", "configurations:Debug", "platforms:x32" }
        targetsuffix ", Debug)"

    filter { "system:windows", "configurations:Debug", "platforms:x64" }
        targetsuffix " x64, Debug)"

    filter { "configurations:Release" }
        defines { "NDEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=0" }
        flags { "NoFramePointer" }
        optimize "Speed"

    filter { "system:linux", "configurations:Release" }
        warnings "Extra"
        buildoptions { "-fvisibility=hidden", "-pipe" }

    filter { "system:linux", "configurations:Release", "platforms:x32" }
        targetsuffix ""

    filter { "system:linux", "configurations:Release", "platforms:x64" }
        targetsuffix "_x64"

    filter { "system:windows", "configurations:Release" }
        flags { "NoManifest" }
        buildoptions { "/Zi" }

    filter { "system:windows", "configurations:Release", "platforms:x32" }
        targetsuffix ")"

    filter { "system:windows", "configurations:Release", "platforms:x64" }
        targetsuffix " x64)"

--------------------------------------------------------------------------------

    project ("trakmeter_standalone_stereo")
        kind "WindowedApp"

        defines {
            "TRAKMETER_STEREO=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_Standalone=1",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        entrypoint "WinMainCRTStartup"

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp"
        }

        filter { "system:linux" }
            targetname "trakmeter_stereo"

            defines {
                "JUCE_ALSA=1",
                "JUCE_JACK=1",
                "JUCE_ASIO=0",
                "JUCE_WASAPI=0",
                "JUCE_DIRECTSOUND=0"
            }

            links {
                "asound"
            }

        filter { "system:windows" }
            targetname "traKmeter (Stereo"
            targetextension (".exe")

            defines {
                "JUCE_ALSA=0",
                "JUCE_JACK=0",
                "JUCE_ASIO=1",
                "JUCE_WASAPI=1",
                "JUCE_DIRECTSOUND=1"
            }

            includedirs {
                "../libraries/asio/common"
            }

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/standalone_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/standalone_stereo_release")

--------------------------------------------------------------------------------

    project ("trakmeter_standalone_multi")
        kind "WindowedApp"

        defines {
            "TRAKMETER_MULTI=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_Standalone=1",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        entrypoint "WinMainCRTStartup"

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp"
        }

        filter { "system:linux" }
            targetname "trakmeter_multi"

            defines {
                "JUCE_ALSA=1",
                "JUCE_JACK=1",
                "JUCE_ASIO=0",
                "JUCE_WASAPI=0",
                "JUCE_DIRECTSOUND=0"
            }

            links {
                "asound"
            }

        filter { "system:windows" }
            targetname "traKmeter (Multi"
            targetextension (".exe")

            defines {
                "JUCE_ALSA=0",
                "JUCE_JACK=0",
                "JUCE_ASIO=1",
                "JUCE_WASAPI=1",
                "JUCE_DIRECTSOUND=1"
            }

            includedirs {
                "../libraries/asio/common"
            }

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/standalone_multi_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/standalone_multi_release")

--------------------------------------------------------------------------------

    project ("trakmeter_vst_stereo")
        kind "SharedLib"

        defines {
            "TRAKMETER_STEREO=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=1",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_ASIO=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst"
        }

        filter { "system:linux" }
            targetname "trakmeter_stereo_vst"

        filter { "system:windows" }
            targetname "traKmeter (Stereo"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst_stereo_release")

--------------------------------------------------------------------------------

    project ("trakmeter_vst_multi")
        kind "SharedLib"

        defines {
            "TRAKMETER_MULTI=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=1",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_ASIO=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst"
        }

        filter { "system:linux" }
            targetname "trakmeter_multi_vst"

        filter { "system:windows" }
            targetname "traKmeter (Multi"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst_multi_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst_multi_release")

--------------------------------------------------------------------------------

-- create VST3 projects on Windows only
if os.target() == "windows" then

   project ("trakmeter_vst3_stereo")
        kind "SharedLib"

        defines {
            "TRAKMETER_STEREO=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=1"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_ASIO=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst/VST3_SDK"
        }

        filter { "system:windows" }
            targetname "traKmeter (Stereo"
            targetextension (".vst3")

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst3_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst3_stereo_release")

-- create VST3 projects on Windows only
end

--------------------------------------------------------------------------------

-- create VST3 projects on Windows only
if os.target() == "windows" then

   project ("trakmeter_vst3_multi")
        kind "SharedLib"

        defines {
            "TRAKMETER_MULTI=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=1"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_ASIO=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst/VST3_SDK"
        }

        filter { "system:windows" }
            targetname "traKmeter (Multi"
            targetextension (".vst3")

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst3_multi_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/vst3_multi_release")

-- create VST3 projects on Windows only
end

--------------------------------------------------------------------------------

-- create LV2 projects on Linux only
if os.target() == "linux" then

    project ("trakmeter_lv2_stereo")
        kind "SharedLib"

        defines {
            "TRAKMETER_STEREO=1",
            "JucePlugin_Build_LV2=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_LV2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_ASIO=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        filter { "system:linux" }
            targetname "trakmeter_stereo_lv2"

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/lv2_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/lv2_stereo_release")

-- create LV2 projects on Linux only
end

--------------------------------------------------------------------------------

-- create LV2 projects on Linux only
if os.target() == "linux" then

    project ("trakmeter_lv2_multi")
        kind "SharedLib"

        defines {
            "TRAKMETER_MULTI=1",
            "JucePlugin_Build_LV2=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_LV2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_ASIO=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        filter { "system:linux" }
            targetname "trakmeter_multi_lv2"

        filter { "configurations:Debug" }
            objdir ("../bin/intermediate_" .. os.target() .. "/lv2_multi_debug")

        filter { "configurations:Release" }
            objdir ("../bin/intermediate_" .. os.target() .. "/lv2_multi_release")

-- create LV2 projects on Linux only
end
