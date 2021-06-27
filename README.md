# Wolf Shaper
[![Build Status](https://img.shields.io/github/workflow/status/wolf-plugins/wolf-shaper/Wolf%20Shaper%20CI.svg?logo=github)](https://github.com/wolf-plugins/wolf-shaper/actions?query=workflow%3A%22Wolf+Shaper+CI%22)

![Wolf Shaper](https://raw.githubusercontent.com/wolf-plugins/wolf-shaper/master/docs/images/screenshot.png)

Wolf Shaper is a waveshaper plugin with a graph editor. It can be built as an LV2, DSSI or VST plugin and as a standalone Jack application.

To find out more about this plugin, check out [this video series by unfa!](https://www.youtube.com/watch?v=2NViimz9Tnw)

## Install

You can find some precompiled plugin binaries in the [Releases](https://github.com/wolf-plugins/wolf-shaper/releases) tab. Some packages are also available for Ubuntu and Arch Linux:

### Ubuntu
Download the plugin from the [pdesaulniers/wolf PPA](https://launchpad.net/~pdesaulniers/+archive/ubuntu/wolf):
```
sudo apt-add-repository -y ppa:pdesaulniers/wolf
sudo apt-get update
sudo apt-get install wolf-shaper
```
### Arch Linux
For installing the latest release:
```
sudo pacman -S wolf-shaper
```
A git package is also available [in the AUR](https://aur.archlinux.org/packages/wolf-shaper-git/).

## Build manually

First, clone the repo (note the "--recursive" argument):

```
git clone --recursive https://github.com/wolf-plugins/wolf-shaper.git
cd wolf-shaper
```

Then:

```
BUILD_VST2=true BUILD_LV2=true BUILD_DSSI=true BUILD_JACK=true make
```

Prepend WIN32=true or MACOS=true to the command if applicable.

All plugin builds will then be placed in the bin folder. Copy them to their appropriate place so that your plugin host can find them, and you're done :)

## Updating

This project uses git submodules. Thus, to update your local copy of the repo, you need to run the following commands:
```
git pull
git submodule update --init --recursive
```
You should then be able to build the plugin with the most recent changes.
