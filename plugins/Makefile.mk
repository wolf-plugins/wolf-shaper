#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# NAME, OBJS_DSP and OBJS_UI have been defined before

include ../../Makefile.mk

ifeq ($(OBJS_UI),)
HAVE_DGL = false
endif

# --------------------------------------------------------------
# Basic setup

TARGET_DIR = ../../bin

BUILD_C_FLAGS   += -I.
BUILD_CXX_FLAGS += -I. -I../../dpf/distrho -I../../dpf/dgl -I./Common/Structures -I./Common/Widgets -I./Common/Utils -I./Resources -I./Config -I./Libs/inih -I./Libs/DSPFilters/include

ifeq ($(HAVE_DGL),true)
BASE_FLAGS += -DHAVE_DGL
endif

ifeq ($(HAVE_JACK),true)
BASE_FLAGS += -DHAVE_JACK
endif

ifeq ($(HAVE_LIBLO),true)
BASE_FLAGS += -DHAVE_LIBLO
endif

# --------------------------------------------------------------
# Set plugin binary file targets

jack       = $(TARGET_DIR)/$(NAME)$(APP_EXT)
# ladspa_dsp = $(TARGET_DIR)/$(NAME)-ladspa$(LIB_EXT)
dssi_dsp   = $(TARGET_DIR)/$(NAME)-dssi$(LIB_EXT)
dssi_ui    = $(TARGET_DIR)/$(NAME)-dssi/$(NAME)-dssi_ui$(APP_EXT)
lv2        = $(TARGET_DIR)/$(NAME).lv2/$(NAME)$(LIB_EXT)
lv2_dsp    = $(TARGET_DIR)/$(NAME).lv2/$(NAME)_dsp$(LIB_EXT)
lv2_ui     = $(TARGET_DIR)/$(NAME).lv2/$(NAME)_ui$(LIB_EXT)
vst        = $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT)

# --------------------------------------------------------------
# Set distrho code files

DISTRHO_PLUGIN_FILES = ../../dpf/distrho/DistrhoPluginMain.cpp

ifeq ($(HAVE_DGL),true)
DISTRHO_UI_FILES     = ../../dpf/distrho/src/IdleThread.cpp ../../dpf/distrho/DistrhoUIMain.cpp ../../dpf/libdgl.a
endif

# --------------------------------------------------------------
# Handle plugins without UI

ifneq ($(HAVE_DGL),true)
dssi_ui =
lv2_ui =
DISTRHO_UI_FILES =
DGL_LIBS =
OBJS_UI =
endif

# --------------------------------------------------------------
# all needs to be first

all:

# --------------------------------------------------------------
# Common

%.c.o: %.c
	$(CC) $< $(BUILD_C_FLAGS) -MD -MP -c -o $@ $(EXTRA_LIBS)

%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) -MD -MP -c -o $@ $(EXTRA_LIBS)

clean:
	rm -f *.d *.o
	rm -f Common/Structures/src/*.d Common/Structures/src/*.o
	rm -f Common/Structures/test/*.d Common/Structures/test/*.o
	rm -f Common/Widgets/src/*.d Common/Widgets/src/*.o
	rm -f Common/Utils/Fonts/getchunk
	rm -f Common/Utils/src/*.d Common/Utils/src/*.o
	rm -f Resources/Fonts/*.d Resources/Fonts/*.o
	rm -f Config/src/*.d Config/src/*.o
	rm -f Libs/inih/*.d Libs/inih/*.o
	rm -f Libs/DSPFilters/source/*.d Libs/DSPFilters/source/*.o
	rm -rf $(TARGET_DIR)/$(NAME) $(TARGET_DIR)/$(NAME)-* $(TARGET_DIR)/$(NAME).lv2/
	rm -rf $(TARGET_DIR)/releases

# --------------------------------------------------------------
# JACK

jack: $(jack)

$(jack): $(OBJS_DSP) $(OBJS_UI) $(DISTRHO_PLUGIN_FILES) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(shell pkg-config --cflags --libs jack) -DDISTRHO_PLUGIN_TARGET_JACK -o $@

# --------------------------------------------------------------
# LADSPA
#
#ladspa: $(ladspa_dsp)
#
#$(ladspa_dsp): $(OBJS_DSP) $(DISTRHO_PLUGIN_FILES)
#	mkdir -p $(shell dirname $@)
#	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_LADSPA -o $@
#
# --------------------------------------------------------------
# DSSI

dssi:     $(dssi_dsp) $(dssi_ui)
dssi_dsp: $(dssi_dsp)
dssi_ui:  $(dssi_ui)

$(dssi_dsp): $(OBJS_DSP) $(DISTRHO_PLUGIN_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_DSSI -o $@

$(dssi_ui): $(OBJS_UI) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(shell pkg-config --cflags --libs liblo) -DDISTRHO_PLUGIN_TARGET_DSSI -o $@

# --------------------------------------------------------------
# LV2

lv2_one: $(lv2)
lv2_dsp: $(lv2_dsp)
lv2_sep: $(lv2_dsp) $(lv2_ui)

$(lv2): $(OBJS_DSP) $(OBJS_UI) $(DISTRHO_PLUGIN_FILES) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_LV2 -o $@

$(lv2_dsp): $(OBJS_DSP) $(DISTRHO_PLUGIN_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_LV2 -o $@

$(lv2_ui): $(OBJS_UI) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_LV2 -o $@

# --------------------------------------------------------------
# VST

vst: $(vst)

$(vst): $(OBJS_DSP) $(OBJS_UI) $(DISTRHO_PLUGIN_FILES) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(SHARED) -DDISTRHO_PLUGIN_TARGET_VST -o $@

# --------------------------------------------------------------

-include $(OBJS_DSP:%.o=%.d)
ifeq ($(HAVE_DGL),true)
-include $(OBJS_UI:%.o=%.d)
endif

# --------------------------------------------------------------
