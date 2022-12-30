#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX, Christopher Arndt, and Patrick Desaulniers
#

export DISTRHO_NAMESPACE = WolfShaperDISTRHO
export DGL_NAMESPACE = WolfShaperDGL
export FILE_BROWSER_DISABLED = true

include ../dpf/Makefile.base.mk

# --------------------------------------------------------------
# Installation directories

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
DSSI_DIR ?= $(LIBDIR)/dssi
LADSPA_DIR ?= $(LIBDIR)/ladspa
CLAP_DIR ?= $(LIBDIR)/clap

ifneq ($(MACOS_OR_WINDOWS),true)
LV2_DIR ?= $(LIBDIR)/lv2
VST2_DIR ?= $(LIBDIR)/vst
VST3_DIR ?= $(LIBDIR)/vst3
endif
ifeq ($(MACOS),true)
LV2_DIR ?= /Library/Audio/Plug-Ins/LV2
VST2_DIR ?= /Library/Audio/Plug-Ins/VST
VST3_DIR ?= /Library/Audio/Plug-Ins/VST3
endif
ifeq ($(WINDOWS),true)
LV2_DIR ?= $(COMMONPROGRAMFILES)/LV2
VST2_DIR ?= $(COMMONPROGRAMFILES)/VST2
VST3_DIR ?= $(COMMONPROGRAMFILES)/VST3
endif

USER_DSSI_DIR ?= $(HOME)/.dssi
USER_LADSPA_DIR ?= $(HOME)/.ladspa
USER_CLAP_DIR ?= $(HOME)/.clap
ifneq ($(MACOS_OR_WINDOWS),true)
USER_LV2_DIR ?= $(HOME)/.lv2
USER_VST2_DIR ?= $(HOME)/.vst
USER_VST3_DIR ?= $(HOME)/.vst3
endif
ifeq ($(MACOS),true)
USER_LV2_DIR ?= $(HOME)/Library/Audio/Plug-Ins/LV2
USER_VST2_DIR ?= $(HOME)/Library/Audio/Plug-Ins/VST
USER_VST3_DIR ?= $(HOME)/Library/Audio/Plug-Ins/VST3
endif
ifeq ($(WINDOWS),true)
USER_LV2_DIR ?= $(APPDATA)/LV2
USER_VST2_DIR ?= $(APPDATA)/VST
USER_VST3_DIR ?= $(APPDATA)/VST3
endif

# --------------------------------------------------------------
# Project name, used for binaries

NAME = wolf-shaper

# --------------------------------------------------------------
# Plugin types to build

BUILD_LV2 ?= true
BUILD_VST2 ?= true
BUILD_VST3 ?= true
BUILD_CLAP ?= true
BUILD_JACK ?= true
BUILD_DSSI ?= true
BUILD_LADSPA ?= false

# required for modgui builds
MODGUI_CLASS_NAME = wolf_plugins_wolf_shaper

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	Utils/src/Mathf.cpp \
	Structures/src/Graph.cpp \
	Structures/src/Oversampler.cpp \
	Structures/src/ParamSmooth.cpp \
	../libs/DSPFilters/source/Butterworth.cpp \
	../libs/DSPFilters/source/Biquad.cpp \
	../libs/DSPFilters/source/Cascade.cpp \
	../libs/DSPFilters/source/Filter.cpp \
	../libs/DSPFilters/source/Param.cpp \
	../libs/DSPFilters/source/Butterworth.cpp \
	../libs/DSPFilters/source/PoleFilter.cpp \
	../libs/DSPFilters/source/RBJ.cpp \
	../libs/DSPFilters/source/Bessel.cpp \
	../libs/DSPFilters/source/RootFinder.cpp \
	WolfShaperPlugin.cpp

FILES_UI  = \
	Utils/src/Mathf.cpp \
	Structures/src/Graph.cpp \
	Structures/src/Margin.cpp \
	Structures/src/Animation.cpp \
	Widgets/src/WolfWidget.cpp \
	Widgets/src/GraphNode.cpp \
	Widgets/src/GraphWidget.cpp \
	Widgets/src/NanoSwitch.cpp \
	Widgets/src/RemoveDCSwitch.cpp \
	Widgets/src/NanoButton.cpp \
	Widgets/src/ResetGraphButton.cpp \
	Widgets/src/NanoWheel.cpp \
	Widgets/src/OversampleWheel.cpp \
	Widgets/src/NanoKnob.cpp \
	Widgets/src/VolumeKnob.cpp \
	Widgets/src/ResizeHandle.cpp \
	Widgets/src/LabelBox.cpp \
	Widgets/src/LabelBoxList.cpp \
	Widgets/src/BipolarModeSwitch.cpp \
	Widgets/src/LabelContainer.cpp \
	Widgets/src/GlowingLabelsBox.cpp \
	Widgets/src/NanoLabel.cpp \
	Widgets/src/WidgetBar.cpp \
	Widgets/src/ArrowButton.cpp \
	Widgets/src/MenuWidget.cpp \
	Resources/Fonts/chivo_bold.cpp \
	Resources/Fonts/chivo_italic.cpp \
	WolfShaperUI.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = opengl

DPF_PATH = ../dpf
DPF_TARGET_DIR = ../bin
DPF_BUILD_DIR = ../build/$(NAME)

include ../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Additional flags

BASE_FLAGS += -I. -I../../dpf/distrho -I../../dpf/dgl -I./Structures -I./Widgets -I./Utils -I./Resources -I../libs/DSPFilters/include

BASE_FLAGS += $(CUSTOM_DEFINES)

# Fix missing GL/gl.h include on FreeBSD (commented out for now since it breaks CI)
# BASE_FLAGS += $(shell $(PKG_CONFIG) --cflags-only-I gl)

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
TARGETS += vst2
endif

ifeq ($(BUILD_VST3),true)
TARGETS += vst3
endif

ifeq ($(BUILD_CLAP),true)
TARGETS += clap
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
	@mkdir -p -m755 $(DESTDIR)$(VST2_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT) $(DESTDIR)$(VST2_DIR)
endif
ifeq ($(BUILD_VST3),true)
	@mkdir -p -m755 $(DESTDIR)$(VST3_DIR) && \
	  cp -r $(TARGET_DIR)/$(NAME).vst3 $(DESTDIR)$(VST3_DIR)
endif
ifeq ($(BUILD_CLAP),true)
	@mkdir -p -m755 $(DESTDIR)$(CLAP_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME).clap $(DESTDIR)$(CLAP_DIR)
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
	@mkdir -p -m755 $(USER_VST2_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT) $(USER_VST2_DIR)
endif
ifeq ($(BUILD_VST3),true)
	@mkdir -p -m755 $(USER_VST3_DIR) && \
	  cp -r $(TARGET_DIR)/$(NAME).vst3 $(USER_VST3_DIR)
endif
ifeq ($(BUILD_CLAP),true)
	@mkdir -p -m755 $(USER_CLAP_DIR) && \
	  install -m755 $(TARGET_DIR)/$(NAME).clap $(USER_CLAP_DIR)
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
