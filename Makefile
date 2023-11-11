#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX, Christopher Arndt, and Patrick Desaulniers
#

export DISTRHO_NAMESPACE = WolfShaperDISTRHO
export DGL_NAMESPACE = WolfShaperDGL
export FILE_BROWSER_DISABLED = true

export CFLAGS += -fno-fast-math
export CXXFLAGS += -fno-fast-math

include dpf/Makefile.base.mk

all: libs plugins gen

# --------------------------------------------------------------

libs:
	$(MAKE) -C dpf/dgl opengl

plugins: libs
	$(MAKE) all -C src

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@bash $(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen: plugins dpf/utils/lv2_ttl_generator.exe
	@bash $(CURDIR)/dpf/utils/generate-ttl.sh

dpf/utils/lv2_ttl_generator.exe:
	$(MAKE) -C dpf/utils/lv2-ttl-generator WINDOWS=true
endif

# --------------------------------------------------------------

release:
	LINUX=true ./tools/make-release.sh
	WIN32=true ./tools/make-release.sh
	./tools/bundle-source.sh

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C src
	rm -rf bin build

install: all
	$(MAKE) install -C src

install-user: all
	$(MAKE) install-user -C src

# --------------------------------------------------------------

.PHONY: all clean install install-user libs plugins gen
