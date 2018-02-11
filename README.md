# spoonie WaveShaper

![alt text](https://raw.githubusercontent.com/pdesaulniers/spoonie-waveshaper/master/plugins/WaveShaper/Screenshot.png)

Free, cross-platform distortion plugin. Can be built as an LV2, DSSI or VST plugin and as a standalone Jack application.

While kind of usable, the plugin is not done yet. Please check the issues on GitHub before using it :)

## Build and install

First, clone the repo (note the "--recursive" argument):

```
git clone --recursive https://github.com/pdesaulniers/spoonie-waveshaper.git
cd spoonie-waveshaper
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
