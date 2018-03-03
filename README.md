# spoonie WaveShaper

![alt text](https://raw.githubusercontent.com/pdesaulniers/spoonie-waveshaper/master/plugins/WaveShaper/Screenshot.png)

spoonie WaveShaper (name subject to change) is a free, cross-platform distortion plugin. It can be built as an LV2, DSSI or VST plugin and as a standalone Jack application.

While kind of usable, it is not done yet. You might want to check the issues on GitHub before using the plugin :)

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

## Customization

The UI can be customized using a configuration file.
Copy the [default configuration](https://github.com/pdesaulniers/spoonie-waveshaper/blob/master/plugins/WaveShaper/Config/Themes/default.conf) to its appropriate location:

Linux:
```
~/.config/spoonie-waveshaper.conf
```
Mac:
```
~/Library/Application Support/spoonie-waveshaper.conf
```
Windows:
```
<My Documents>\spoonie-waveshaper.conf
```
