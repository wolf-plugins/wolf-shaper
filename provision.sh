#!/usr/bin/env bash

export LANGUAGE=en_US.UTF-8
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
locale-gen en_US.UTF-8
dpkg-reconfigure locales

apt-get update
apt-get install -y g++ pkg-config git libx11-dev libgl1-mesa-dev liblo-dev libjack-dev

#TODO: 
#clone repo 
#cd repo 
#rm -r bin
#ln -s /vagrant/bin bin/
#./make_release.sh (etc)
