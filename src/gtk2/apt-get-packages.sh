#!/bin/bash
#
# Helper script to grab all the packages vpython needs Run as root.
#
apt-get -q -y update
apt-get -q -y install git libgtkglextmm-x11-1.2-dev python-dev python-pip python-numpy python-fonttools python-wxtools libboost-python-dev libboost-numpy-dev libboost-thread-dev libboost-signals-dev
