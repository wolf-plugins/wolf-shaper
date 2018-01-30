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
	Widgets/src/GraphNodes.cpp.o \
	Widgets/src/GraphWidget.cpp.o \
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
