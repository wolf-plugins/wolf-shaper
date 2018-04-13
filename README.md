# Wolf Shaper

![alt text](https://raw.githubusercontent.com/pdesaulniers/spoonie-waveshaper/master/plugins/WaveShaper/Screenshot.png)

Free, cross-platform distortion plugin with a spline-based graph. Can be built as an LV2, DSSI or VST plugin and as a standalone Jack application.

You might want to check the issues on GitHub before using the plugin :)

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

## Customization

The UI can be customized using a configuration file.
Copy the [default configuration](https://github.com/pdesaulniers/spoonie-waveshaper/blob/master/plugins/WaveShaper/Config/Themes/default.conf) to its appropriate location:

Linux:
```
~/.config/wolf-shaper.conf
```
Mac:
```
~/Library/Application Support/wolf-shaper.conf
```
Windows:
```
<My Documents>\wolf-shaper.conf
```

You can then edit the plugin's colors to your liking.
