#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by Patrick Desaulniers
#

include Makefile.mk

all: libs plugins gen

# --------------------------------------------------------------

PREFIX  ?= /usr/local
DESTDIR ?=
VST_FOLDER_NAME ?= vst

define MISSING_SUBMODULES_ERROR

Cannot find DGL! Please run "make submodules" to clone the missing submodules, then retry building the plugin.

endef

# --------------------------------------------------------------

submodules: 
	git submodule update --init --recursive

libs:
ifeq (,$(wildcard dpf/dgl))
	$(error $(MISSING_SUBMODULES_ERROR))
endif

ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl
endif

plugins: libs
	$(MAKE) all -C plugins/wolf-shaper

gen: plugins dpf/utils/lv2_ttl_generator
	"$(CURDIR)/dpf/utils/generate-ttl.sh"
ifeq ($(MACOS),true)
	"$(CURDIR)/dpf/utils/generate-vst-bundles.sh"
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator

# --------------------------------------------------------------

clean:
ifeq ($(HAVE_DGL),true)
	$(MAKE) clean -C dpf/dgl
endif
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator

	$(MAKE) clean -C plugins/wolf-shaper

# --------------------------------------------------------------

install:
	install -d $(DESTDIR)$(PREFIX)/lib/dssi/
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/
	install -d "$(DESTDIR)$(PREFIX)/lib/$(VST_FOLDER_NAME)/"

	cp bin/*-dssi.*   $(DESTDIR)$(PREFIX)/lib/dssi/
	cp bin/*-vst.*    "$(DESTDIR)$(PREFIX)/lib/$(VST_FOLDER_NAME)/"


ifeq ($(HAVE_DGL),true)
	cp -r bin/*-dssi  $(DESTDIR)$(PREFIX)/lib/dssi/
endif
	cp -r bin/*.lv2   $(DESTDIR)$(PREFIX)/lib/lv2/

# --------------------------------------------------------------

.PHONY: plugins
