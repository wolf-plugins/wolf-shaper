#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = spoonie-waveshaper

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	Structures/src/Graph.cpp.o \
	WaveShaperPlugin.cpp.o

OBJS_UI  = \
	Structures/src/Graph.cpp.o \
	Widgets/src/GraphNode.cpp.o \
	Widgets/src/GraphWidget.cpp.o \
	Widgets/src/GraphWidgetSocket.cpp.o \
	Widgets/src/GraphNodesLayer.cpp.o \
	Widgets/src/NanoSwitch.cpp.o \
	Widgets/src/RemoveDCSwitch.cpp.o \
	Widgets/src/NanoButton.cpp.o \
	Widgets/src/ResetGraphButton.cpp.o \
	Widgets/src/NanoWheel.cpp.o \
	Widgets/src/OversampleWheel.cpp.o \
	Widgets/src/NanoKnob.cpp.o \
	Widgets/src/VolumeKnob.cpp.o \
	Widgets/src/ResizeHandle.cpp.o \
	Config/src/Config.cpp.o \
	Libs/inih/ini.c.o \
	Libs/inih/INIReader.cpp.o \
	Resources/Fonts/roboto_light.cpp.o \
	WaveShaperUI.cpp.o 

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(BUILD_LADSPA),true)
TARGETS += ladspa
endif

ifeq ($(BUILD_DSSI),true)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
endif
endif

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

all: $(TARGETS)

# --------------------------------------------------------------
