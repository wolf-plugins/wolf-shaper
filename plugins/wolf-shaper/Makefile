#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX, Christopher Arndt, and Patrick Desaulniers
#

# --------------------------------------------------------------
# Installation directories

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
DSSI_DIR ?= $(LIBDIR)/dssi
LADSPA_DIR ?= $(LIBDIR)/ladspa
ifneq ($(MACOS_OR_WINDOWS),true)
LV2_DIR ?= $(LIBDIR)/lv2
VST_DIR ?= $(LIBDIR)/vst
endif
ifeq ($(MACOS),true)
LV2_DIR ?= /Library/Audio/Plug-Ins/LV2
VST_DIR ?= /Library/Audio/Plug-Ins/VST
endif
ifeq ($(WINDOWS),true)
LV2_DIR ?= $(COMMONPROGRAMFILES)/LV2
VST_DIR ?= $(COMMONPROGRAMFILES)/VST2
endif

USER_DSSI_DIR ?= $(HOME)/.dssi
USER_LADSPA_DIR ?= $(HOME)/.ladspa
ifneq ($(MACOS_OR_WINDOWS),true)
USER_LV2_DIR ?= $(HOME)/.lv2
USER_VST_DIR ?= $(HOME)/.vst
endif
ifeq ($(MACOS),true)
USER_LV2_DIR ?= $(HOME)/Library/Audio/Plug-Ins/LV2
USER_VST_DIR ?= $(HOME)/Library/Audio/Plug-Ins/VST
endif
ifeq ($(WINDOWS),true)
USER_LV2_DIR ?= $(APPDATA)/LV2
USER_VST_DIR ?= $(APPDATA)/VST
endif

# --------------------------------------------------------------
# Project name, used for binaries

NAME = wolf-shaper

# --------------------------------------------------------------
# Plugin types to build

BUILD_LV2 ?= true
BUILD_VST2 ?= true
BUILD_JACK ?= true
BUILD_DSSI ?= true
BUILD_LADSPA ?= false

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	Common/Utils/src/Mathf.cpp \
	Common/Structures/src/Graph.cpp \
	Common/Structures/src/Oversampler.cpp \
	Common/Structures/src/ParamSmooth.cpp \
	Libs/DSPFilters/source/Butterworth.cpp \
	Libs/DSPFilters/source/Biquad.cpp \
	Libs/DSPFilters/source/Cascade.cpp \
	Libs/DSPFilters/source/Filter.cpp \
	Libs/DSPFilters/source/Param.cpp \
	Libs/DSPFilters/source/Butterworth.cpp \
	Libs/DSPFilters/source/PoleFilter.cpp \
	Libs/DSPFilters/source/RBJ.cpp \
	Libs/DSPFilters/source/Bessel.cpp \
	Libs/DSPFilters/source/RootFinder.cpp \
	WolfShaperPlugin.cpp

FILES_UI  = \
	Common/Utils/src/Mathf.cpp \
	Common/Structures/src/Graph.cpp \
	Common/Structures/src/Margin.cpp \
	Common/Structures/src/Animation.cpp \
	Common/Widgets/src/WolfWidget.cpp \
	Common/Widgets/src/GraphNode.cpp \
	Common/Widgets/src/GraphWidget.cpp \
	Common/Widgets/src/NanoSwitch.cpp \
	Common/Widgets/src/RemoveDCSwitch.cpp \
	Common/Widgets/src/NanoButton.cpp \
	Common/Widgets/src/ResetGraphButton.cpp \
	Common/Widgets/src/NanoWheel.cpp \
	Common/Widgets/src/OversampleWheel.cpp \
	Common/Widgets/src/NanoKnob.cpp \
	Common/Widgets/src/VolumeKnob.cpp \
	Common/Widgets/src/ResizeHandle.cpp \
	Common/Widgets/src/LabelBox.cpp \
	Common/Widgets/src/LabelBoxList.cpp \
	Common/Widgets/src/BipolarModeSwitch.cpp \
	Common/Widgets/src/LabelContainer.cpp \
	Common/Widgets/src/GlowingLabelsBox.cpp \
	Common/Widgets/src/NanoLabel.cpp \
	Common/Widgets/src/WidgetBar.cpp \
	Common/Widgets/src/ArrowButton.cpp \
	Common/Widgets/src/RightClickMenu.cpp \
	Resources/Fonts/chivo_bold.cpp \
	Resources/Fonts/chivo_italic.cpp \
	WolfShaperUI.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = opengl
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Additional flags

BASE_FLAGS += -I. -I../../dpf/distrho -I../../dpf/dgl -I./Common/Structures -I./Common/Widgets -I./Common/Utils -I./Resources -I./Config -I./Libs/inih -I./Libs/DSPFilters/include

# Fix missing GL/gl.h include on FreeBSD
BASE_FLAGS += $(shell pkg-config --cflags-only-I gl)

LINK_FLAGS += -lpthread

ifeq ($(PARANOID_MODE),true)
BASE_FLAGS += -Wall -Wextra -Werror
endif

# --------------------------------------------------------------
# Enable all selected plugin types

ifeq ($(BUILD_LV2),true)
ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif
endif

ifeq ($(BUILD_VST2),true)
TARGETS += vst
endif

ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(BUILD_DSSI),true)
ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
endif
endif
endif

ifeq ($(BUILD_LADSPA),true)
TARGETS += ladspa
endif

all: $(TARGETS)

install: all
ifeq ($(BUILD_DSSI),true)
ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
	@mkdir -p -m755 $(DESTDIR)$(DSSI_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-dssi$(LIB_EXT) $(DESTDIR)$(DSSI_DIR)
endif
endif
endif
endif
ifeq ($(BUILD_LADSPA),true)
	@mkdir -p -m755 $(DESTDIR)$(LADSPA_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-ladspa$(LIB_EXT) $(DESTDIR)$(LADSPA_DIR)
endif
ifeq ($(BUILD_VST2),true)
	@mkdir -p -m755 $(DESTDIR)$(VST_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT) $(DESTDIR)$(VST_DIR)
endif
ifeq ($(BUILD_LV2),true)
	@mkdir -p -m755 $(DESTDIR)$(LV2_DIR)/$(NAME).lv2 && \
	  install -m755 $(TARGET_DIR)/$(NAME).lv2/*$(LIB_EXT) $(DESTDIR)$(LV2_DIR)/$(NAME).lv2 && \
	  install -m644 $(TARGET_DIR)/$(NAME).lv2/*.ttl $(DESTDIR)$(LV2_DIR)/$(NAME).lv2
endif
ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
	@mkdir -p -m755 $(DESTDIR)$(BINDIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)$(APP_EXT) $(DESTDIR)$(BINDIR)
endif
endif

install-user: all
ifeq ($(BUILD_DSSI),true)
ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
	@mkdir -p -m755 $(USER_DSSI_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-dssi$(LIB_EXT) $(USER_DSSI_DIR)
endif
endif
endif
endif
ifeq ($(BUILD_LADSPA),true)
	@mkdir -p -m755 $(USER_LADSPA_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-ladspa$(LIB_EXT) $(USER_LADSPA_DIR)
endif
ifeq ($(BUILD_VST2),true)
	@mkdir -p -m755 $(USER_VST_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT) $(USER_VST_DIR)
endif
ifeq ($(BUILD_LV2),true)
	@mkdir -p -m755 $(USER_LV2_DIR)/$(NAME).lv2 && \
	  install -m755 $(TARGET_DIR)/$(NAME).lv2/*$(LIB_EXT) $(USER_LV2_DIR)/$(NAME).lv2 && \
	  install -m644 $(TARGET_DIR)/$(NAME).lv2/*.ttl $(USER_LV2_DIR)/$(NAME).lv2
endif
ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
	@mkdir -p -m755 $(HOME)/bin && \
	  install -m755 $(TARGET_DIR)/$(NAME)$(APP_EXT) $(HOME)/bin
endif
endif

# --------------------------------------------------------------

.PHONY: all install install-user
