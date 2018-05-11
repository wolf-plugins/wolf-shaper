# Wolf Shaper 
[![Build Status](https://travis-ci.org/pdesaulniers/wolf-shaper.svg?branch=master)](https://travis-ci.org/pdesaulniers/wolf-shaper)
[![Chat on Matrix](https://matrix.to/img/matrix-badge.svg)](https://riot.im/app/#/user/@pdesaulniers:matrix.org?action=chat)

![Wolf Shaper](https://raw.githubusercontent.com/pdesaulniers/wolf-shaper/master/plugins/wolf-shaper/Screenshot.png)

Wolf Shaper is a waveshaper plugin with a graph editor. It can be built as an LV2, DSSI or VST plugin and as a standalone Jack application.

#### Features:
* Add up to 100 nodes in the graph
* Symmetrical and asymmetrical waveshaping
* Resizable vector-based UI

## Build and install

First, clone the repo (note the "--recursive" argument):

```
git clone --recursive https://github.com/pdesaulniers/wolf-shaper.git
cd wolf-shaper
```

Then:

```
BUILD_VST2=true BUILD_LV2=true BUILD_DSSI=true BUILD_JACK=true make
```

Prepend WIN32=true or MACOS=true to the command if applicable.

All plugin builds will then be placed in the bin folder. Copy them to their appropriate place so that your plugin host can find them, and you're done :)

## How to use

Right-click in the graph to add a vertex. 

Left-click and drag a vertex to move it.

Double left-click a vertex to remove it.
