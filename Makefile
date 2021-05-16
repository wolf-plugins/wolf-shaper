#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX, Christopher Arndt, and Patrick Desaulniers
#

include dpf/Makefile.base.mk

all: libs plugins gen

# --------------------------------------------------------------

DISTRHO_NAMESPACE = WolfShaperDISTRHO
DGL_NAMESPACE = WolfShaperDGL

# FIXME
export CUSTOM_DEFINES = -DDGL_FILE_BROWSER_DISABLED -DPUGL_NO_INCLUDE_GLU_H -DDISTRHO_NAMESPACE=$(DISTRHO_NAMESPACE) -DDGL_NAMESPACE=$(DGL_NAMESPACE)

# --------------------------------------------------------------

libs:
	$(MAKE) -C dpf/dgl ../build/libdgl-opengl.a DGL_FLAGS="$(DGL_FLAGS) $(CUSTOM_DEFINES)"

plugins: libs
	$(MAKE) all -C plugins/wolf-shaper

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

patch:
	patch -d dpf -p 1 -i "$(CURDIR)/dpf-subwidgets.patch"

# --------------------------------------------------------------

release:
	LINUX=true ./plugins/wolf-shaper/Common/Utils/make_release.sh
	WIN32=true ./plugins/wolf-shaper/Common/Utils/make_release.sh
	./plugins/wolf-shaper/Common/Utils/bundle_source.sh

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/wolf-shaper
	rm -rf bin build

install: all
	$(MAKE) install -C plugins/wolf-shaper

install-user: all
	$(MAKE) install-user -C plugins/wolf-shaper

# --------------------------------------------------------------

.PHONY: all clean install install-user libs plugins gen
