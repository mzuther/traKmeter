# GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x32
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild prelink

ifeq ($(config),debug_x32)
  RESCOMP = windres
  TARGETDIR = ../../../bin
  TARGET = $(TARGETDIR)/trakmeter_stereo_vst_debug.so
  OBJDIR = ../../../bin/intermediate_linux/vst_stereo_debug/x32
  DEFINES += -DLINUX=1 -D_DEBUG=1 -DDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=1 -DTRAKMETER_STEREO=1 -DJucePlugin_Build_LV2=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_ASIO=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries/juce/modules -I../../../Source/common -I../../../libraries -I/usr/include/at-spi-2.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/dbus-1.0 -I/usr/include/freetype2 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/gio-unix-2.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/harfbuzz -I/usr/include/libpng12 -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/mirclient -I/usr/include/mircookie -I/usr/include/mircore -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/webkitgtk-4.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../../../libraries/vst
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -fPIC -g -Wall -Wextra -DHAVE_LROUND -fno-inline -ggdb
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -fPIC -g -Wall -Wextra -std=c++11 -DHAVE_LROUND -fno-inline -ggdb
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lX11 -lXext -lXinerama -latk-1.0 -lcairo -lcairo-gobject -lcurl -ldl -lfreetype -lgdk-3 -lgdk_pixbuf-2.0 -lgio-2.0 -lglib-2.0 -lgobject-2.0 -lgtk-3 -ljavascriptcoregtk-4.0 -lpango-1.0 -lpangocairo-1.0 -lpthread -lrt -lsoup-2.4 -lwebkit2gtk-4.0
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -shared -Wl,-soname=trakmeter_stereo_vst_debug.so -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),debug_x64)
  RESCOMP = windres
  TARGETDIR = ../../../bin
  TARGET = $(TARGETDIR)/trakmeter_stereo_vst_debug_x64.so
  OBJDIR = ../../../bin/intermediate_linux/vst_stereo_debug/x64
  DEFINES += -DLINUX=1 -D_DEBUG=1 -DDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=1 -DTRAKMETER_STEREO=1 -DJucePlugin_Build_LV2=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_ASIO=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries/juce/modules -I../../../Source/common -I../../../libraries -I/usr/include/at-spi-2.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/dbus-1.0 -I/usr/include/freetype2 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/gio-unix-2.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/harfbuzz -I/usr/include/libpng12 -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/mirclient -I/usr/include/mircookie -I/usr/include/mircore -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/webkitgtk-4.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../../../libraries/vst
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -fPIC -g -Wall -Wextra -DHAVE_LROUND -fno-inline -ggdb
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -fPIC -g -Wall -Wextra -std=c++11 -DHAVE_LROUND -fno-inline -ggdb
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lX11 -lXext -lXinerama -latk-1.0 -lcairo -lcairo-gobject -lcurl -ldl -lfreetype -lgdk-3 -lgdk_pixbuf-2.0 -lgio-2.0 -lglib-2.0 -lgobject-2.0 -lgtk-3 -ljavascriptcoregtk-4.0 -lpango-1.0 -lpangocairo-1.0 -lpthread -lrt -lsoup-2.4 -lwebkit2gtk-4.0
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -shared -Wl,-soname=trakmeter_stereo_vst_debug_x64.so -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release_x32)
  RESCOMP = windres
  TARGETDIR = ../../../bin
  TARGET = $(TARGETDIR)/trakmeter_stereo_vst.so
  OBJDIR = ../../../bin/intermediate_linux/vst_stereo_release/x32
  DEFINES += -DLINUX=1 -DNDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=0 -DTRAKMETER_STEREO=1 -DJucePlugin_Build_LV2=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_ASIO=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries/juce/modules -I../../../Source/common -I../../../libraries -I/usr/include/at-spi-2.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/dbus-1.0 -I/usr/include/freetype2 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/gio-unix-2.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/harfbuzz -I/usr/include/libpng12 -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/mirclient -I/usr/include/mircookie -I/usr/include/mircore -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/webkitgtk-4.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../../../libraries/vst
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -DHAVE_LROUND -fvisibility=hidden -pipe
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -std=c++11 -DHAVE_LROUND -fvisibility=hidden -pipe
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lX11 -lXext -lXinerama -latk-1.0 -lcairo -lcairo-gobject -lcurl -ldl -lfreetype -lgdk-3 -lgdk_pixbuf-2.0 -lgio-2.0 -lglib-2.0 -lgobject-2.0 -lgtk-3 -ljavascriptcoregtk-4.0 -lpango-1.0 -lpangocairo-1.0 -lpthread -lrt -lsoup-2.4 -lwebkit2gtk-4.0
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -shared -Wl,-soname=trakmeter_stereo_vst.so -s -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release_x64)
  RESCOMP = windres
  TARGETDIR = ../../../bin
  TARGET = $(TARGETDIR)/trakmeter_stereo_vst_x64.so
  OBJDIR = ../../../bin/intermediate_linux/vst_stereo_release/x64
  DEFINES += -DLINUX=1 -DNDEBUG=1 -DJUCE_CHECK_MEMORY_LEAKS=0 -DTRAKMETER_STEREO=1 -DJucePlugin_Build_LV2=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_VST=1 -DJUCE_ALSA=0 -DJUCE_JACK=0 -DJUCE_ASIO=0 -DJUCE_WASAPI=0 -DJUCE_DIRECTSOUND=0
  INCLUDES += -I../../../JuceLibraryCode -I../../../libraries/juce/modules -I../../../Source/common -I../../../libraries -I/usr/include/at-spi-2.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/dbus-1.0 -I/usr/include/freetype2 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/gio-unix-2.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/harfbuzz -I/usr/include/libpng12 -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/mirclient -I/usr/include/mircookie -I/usr/include/mircore -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/webkitgtk-4.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../../../libraries/vst
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -DHAVE_LROUND -fvisibility=hidden -pipe
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -fomit-frame-pointer -O3 -fPIC -Wall -Wextra -std=c++11 -DHAVE_LROUND -fvisibility=hidden -pipe
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lX11 -lXext -lXinerama -latk-1.0 -lcairo -lcairo-gobject -lcurl -ldl -lfreetype -lgdk-3 -lgdk_pixbuf-2.0 -lgio-2.0 -lglib-2.0 -lgobject-2.0 -lgtk-3 -ljavascriptcoregtk-4.0 -lpango-1.0 -lpangocairo-1.0 -lpthread -lrt -lsoup-2.4 -lwebkit2gtk-4.0
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -shared -Wl,-soname=trakmeter_stereo_vst_x64.so -s -Wl,--no-undefined
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
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
	$(OBJDIR)/include_juce_events.o \
	$(OBJDIR)/include_juce_graphics.o \
	$(OBJDIR)/include_juce_gui_basics.o \
	$(OBJDIR)/include_juce_gui_extra.o \
	$(OBJDIR)/include_juce_video.o \
	$(OBJDIR)/audio_file_player.o \
	$(OBJDIR)/include_frut_audio.o \
	$(OBJDIR)/include_frut_dsp.o \
	$(OBJDIR)/include_frut_math.o \
	$(OBJDIR)/include_frut_parameter.o \
	$(OBJDIR)/include_frut_skin.o \
	$(OBJDIR)/include_frut_widget.o \
	$(OBJDIR)/meter_ballistics.o \
	$(OBJDIR)/meter_bar_average.o \
	$(OBJDIR)/meter_bar_peak.o \
	$(OBJDIR)/overload_meter.o \
	$(OBJDIR)/plugin_editor.o \
	$(OBJDIR)/plugin_parameters.o \
	$(OBJDIR)/plugin_processor.o \
	$(OBJDIR)/skin.o \
	$(OBJDIR)/trakmeter.o \
	$(OBJDIR)/window_validation_content.o \

RESOURCES := \

CUSTOMFILES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

$(TARGET): $(GCH) ${CUSTOMFILES} $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking trakmeter_vst_stereo
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

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
	@echo Cleaning trakmeter_vst_stereo
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
$(OBJECTS): $(GCH) $(PCH)
$(GCH): $(PCH)
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
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
$(OBJDIR)/include_frut_audio.o: ../../../Source/common/amalgamated/include_frut_audio.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_dsp.o: ../../../Source/common/amalgamated/include_frut_dsp.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_math.o: ../../../Source/common/amalgamated/include_frut_math.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_parameter.o: ../../../Source/common/amalgamated/include_frut_parameter.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_skin.o: ../../../Source/common/amalgamated/include_frut_skin.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/include_frut_widget.o: ../../../Source/common/amalgamated/include_frut_widget.cpp
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
$(OBJDIR)/window_validation_content.o: ../../../Source/window_validation_content.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif