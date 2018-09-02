# Wolf Shaper 
[![Build Status](https://travis-ci.org/pdesaulniers/wolf-shaper.svg?branch=master)](https://travis-ci.org/pdesaulniers/wolf-shaper)
[![Chat on Matrix](https://matrix.to/img/matrix-badge.svg)](https://riot.im/app/#/user/@pdesaulniers:matrix.org?action=chat)

![Wolf Shaper](https://raw.githubusercontent.com/pdesaulniers/wolf-shaper/master/plugins/wolf-shaper/Screenshot.png)

Wolf Shaper is a waveshaper plugin with a graph editor. It can be built as an LV2, DSSI or VST plugin and as a standalone Jack application.

Right now, it works on GNU/Linux and Windows. MacOS support is coming soon.

#### Features:
* Add up to 100 nodes in the graph
* Symmetrical and asymmetrical waveshaping
* Resizable vector-based UI

## Install

You can find some precompiled plugin binaries in the [Releases](https://github.com/pdesaulniers/wolf-shaper/releases) tab. Some packages are also available for Ubuntu and Arch Linux:

### Ubuntu
Download the plugin from the [pdesaulniers/wolf PPA](https://launchpad.net/~pdesaulniers/+archive/ubuntu/wolf):
```
sudo apt-add-repository -y ppa:pdesaulniers/wolf
sudo apt-get update
sudo apt-get install wolf-shaper
```
### Arch Linux
Wolf Shaper is available [in the AUR](https://aur.archlinux.org/packages/wolf-shaper-git/). 

## Build manually

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

Online documentation is available at https://pdesaulniers.github.io/wolf-shaper/.

## Updating

This project uses git submodules. Thus, to update your local copy of the repo, you need to run the following commands:
```
git pull
git submodule update
```
You should then be able to build the plugin with the most recent changes.
