# GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x32
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild prelink

ifeq ($(config),debug_x32)
  ifeq ($(origin CC), default)
    CC = clang
  endif
  ifeq ($(origin CXX), default)
    CXX = clang++
  endif
  ifeq ($(origin AR), default)
    AR = ar
  endif
  TARGETDIR = ../../../bin/vst2
  TARGET = $(TARGETDIR)/trakmeter_multi_vst2_debug.so
  OBJDIR = ../../../bin/.intermediate_linux/vst2_multi_debug/x32
  DEFINES += -DLINUX=1 -D_DEBUG=1 -DDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=1 -DTRAKMETER_MULTI=1 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries -I../../../libraries/juce/modules -I../../../Source/frut -I/usr/include -I/usr/include/freetype2 -I../../../libraries/vst2/VST2_SDK
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O0 -fPIC -g -Wall -Wextra -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fno-inline -ggdb
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O0 -fPIC -g -Wall -Wextra -std=c++17 -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fno-inline -ggdb
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -ldl -lfreetype -lpthread -lrt -lX11 -lXext
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -shared -Wl,-soname=trakmeter_multi_vst2_debug.so -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),debug_x64)
  ifeq ($(origin CC), default)
    CC = clang
  endif
  ifeq ($(origin CXX), default)
    CXX = clang++
  endif
  ifeq ($(origin AR), default)
    AR = ar
  endif
  TARGETDIR = ../../../bin/vst2
  TARGET = $(TARGETDIR)/trakmeter_multi_vst2_debug_x64.so
  OBJDIR = ../../../bin/.intermediate_linux/vst2_multi_debug/x64
  DEFINES += -DLINUX=1 -D_DEBUG=1 -DDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=1 -DTRAKMETER_MULTI=1 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries -I../../../libraries/juce/modules -I../../../Source/frut -I/usr/include -I/usr/include/freetype2 -I../../../libraries/vst2/VST2_SDK
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -fPIC -g -Wall -Wextra -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fno-inline -ggdb
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -fPIC -g -Wall -Wextra -std=c++17 -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fno-inline -ggdb
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -ldl -lfreetype -lpthread -lrt -lX11 -lXext
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -shared -Wl,-soname=trakmeter_multi_vst2_debug_x64.so -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release_x32)
  ifeq ($(origin CC), default)
    CC = clang
  endif
  ifeq ($(origin CXX), default)
    CXX = clang++
  endif
  ifeq ($(origin AR), default)
    AR = ar
  endif
  TARGETDIR = ../../../bin/vst2
  TARGET = $(TARGETDIR)/trakmeter_multi_vst2.so
  OBJDIR = ../../../bin/.intermediate_linux/vst2_multi_release/x32
  DEFINES += -DLINUX=1 -DNDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=0 -DTRAKMETER_MULTI=1 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries -I../../../libraries/juce/modules -I../../../Source/frut -I/usr/include -I/usr/include/freetype2 -I../../../libraries/vst2/VST2_SDK
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fvisibility=hidden -pipe -Wno-deprecated
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -std=c++17 -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fvisibility=hidden -pipe -Wno-deprecated
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -ldl -lfreetype -lpthread -lrt -lX11 -lXext
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -shared -Wl,-soname=trakmeter_multi_vst2.so -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release_x64)
  ifeq ($(origin CC), default)
    CC = clang
  endif
  ifeq ($(origin CXX), default)
    CXX = clang++
  endif
  ifeq ($(origin AR), default)
    AR = ar
  endif
  TARGETDIR = ../../../bin/vst2
  TARGET = $(TARGETDIR)/trakmeter_multi_vst2_x64.so
  OBJDIR = ../../../bin/.intermediate_linux/vst2_multi_release/x64
  DEFINES += -DLINUX=1 -DNDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=0 -DTRAKMETER_MULTI=1 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries -I../../../libraries/juce/modules -I../../../Source/frut -I/usr/include -I/usr/include/freetype2 -I../../../libraries/vst2/VST2_SDK
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fvisibility=hidden -pipe -Wno-deprecated
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -std=c++17 -DHAVE_LROUND -fmessage-length=78 -fcolor-diagnostics -fvisibility=hidden -pipe -Wno-deprecated
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -ldl -lfreetype -lpthread -lrt -lX11 -lXext
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -shared -Wl,-soname=trakmeter_multi_vst2_x64.so -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

OBJECTS := \
	$(OBJDIR)/include_juce_audio_basics.o \
	$(OBJDIR)/include_juce_audio_devices.o \
	$(OBJDIR)/include_juce_audio_formats.o \
	$(OBJDIR)/include_juce_audio_plugin_client_VST2.o \
	$(OBJDIR)/include_juce_audio_plugin_client_utils.o \
	$(OBJDIR)/include_juce_audio_processors.o \
	$(OBJDIR)/include_juce_audio_utils.o \
	$(OBJDIR)/include_juce_core.o \
	$(OBJDIR)/include_juce_cryptography.o \
	$(OBJDIR)/include_juce_data_structures.o \
	$(OBJDIR)/include_juce_dsp.o \
	$(OBJDIR)/include_juce_events.o \
	$(OBJDIR)/include_juce_graphics.o \
	$(OBJDIR)/include_juce_gui_basics.o \
	$(OBJDIR)/include_juce_gui_extra.o \
	$(OBJDIR)/include_juce_video.o \
	$(OBJDIR)/audio_file_player.o \
	$(OBJDIR)/include_frut_audio.o \
	$(OBJDIR)/include_frut_dsp.o \
	$(OBJDIR)/include_frut_math.o \
	$(OBJDIR)/include_frut_parameters.o \
	$(OBJDIR)/include_frut_skin.o \
	$(OBJDIR)/include_frut_widgets.o \
	$(OBJDIR)/meter_ballistics.o \
	$(OBJDIR)/meter_bar_average.o \
	$(OBJDIR)/meter_bar_peak.o \
	$(OBJDIR)/overload_meter.o \
	$(OBJDIR)/plugin_editor.o \
	$(OBJDIR)/plugin_parameters.o \
	$(OBJDIR)/plugin_processor.o \
	$(OBJDIR)/skin.o \
	$(OBJDIR)/trakmeter.o \
	$(OBJDIR)/trakmeter_skin.o \
	$(OBJDIR)/window_validation_content.o \

RESOURCES := \

CUSTOMFILES := \

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

$(TARGET): $(GCH) ${CUSTOMFILES} $(OBJECTS) $(LDDEPS) $(RESOURCES) | $(TARGETDIR)
	@echo Linking trakmeter_vst2_multi
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(CUSTOMFILES): | $(OBJDIR)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning trakmeter_vst2_multi
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) $(PCH) | $(OBJDIR)
$(GCH): $(PCH) | $(OBJDIR)
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
else
$(OBJECTS): | $(OBJDIR)
endif

$(OBJDIR)/include_juce_audio_basics.o: ../../../JuceLibraryCode/include_juce_audio_basics.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_audio_devices.o: ../../../JuceLibraryCode/include_juce_audio_devices.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_audio_formats.o: ../../../JuceLibraryCode/include_juce_audio_formats.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_audio_plugin_client_VST2.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_audio_plugin_client_utils.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_audio_processors.o: ../../../JuceLibraryCode/include_juce_audio_processors.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_audio_utils.o: ../../../JuceLibraryCode/include_juce_audio_utils.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_core.o: ../../../JuceLibraryCode/include_juce_core.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_cryptography.o: ../../../JuceLibraryCode/include_juce_cryptography.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_data_structures.o: ../../../JuceLibraryCode/include_juce_data_structures.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_dsp.o: ../../../JuceLibraryCode/include_juce_dsp.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_events.o: ../../../JuceLibraryCode/include_juce_events.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_graphics.o: ../../../JuceLibraryCode/include_juce_graphics.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_gui_basics.o: ../../../JuceLibraryCode/include_juce_gui_basics.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_gui_extra.o: ../../../JuceLibraryCode/include_juce_gui_extra.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_juce_video.o: ../../../JuceLibraryCode/include_juce_video.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/audio_file_player.o: ../../../Source/audio_file_player.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_audio.o: ../../../Source/frut/amalgamated/include_frut_audio.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_dsp.o: ../../../Source/frut/amalgamated/include_frut_dsp.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_math.o: ../../../Source/frut/amalgamated/include_frut_math.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_parameters.o: ../../../Source/frut/amalgamated/include_frut_parameters.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_skin.o: ../../../Source/frut/amalgamated/include_frut_skin.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_widgets.o: ../../../Source/frut/amalgamated/include_frut_widgets.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/meter_ballistics.o: ../../../Source/meter_ballistics.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/meter_bar_average.o: ../../../Source/meter_bar_average.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/meter_bar_peak.o: ../../../Source/meter_bar_peak.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/overload_meter.o: ../../../Source/overload_meter.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/plugin_editor.o: ../../../Source/plugin_editor.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/plugin_parameters.o: ../../../Source/plugin_parameters.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/plugin_processor.o: ../../../Source/plugin_processor.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/skin.o: ../../../Source/skin.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/trakmeter.o: ../../../Source/trakmeter.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/trakmeter_skin.o: ../../../Source/trakmeter_skin.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/window_validation_content.o: ../../../Source/window_validation_content.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif