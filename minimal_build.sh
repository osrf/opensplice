#!/bin/bash

set -e
echo "WARNING: this script is brittle; be careful."
echo "Press any key to continue (and hope for the best)."
read

set -x

#. configure
#make
#make install
rm -rf install/minimal
mkdir -p install/minimal
cp -a install/HDE/*/lib install/minimal
mkdir -p install/minimal/include
cp -a install/HDE/*/include install/minimal/include/opensplice
mkdir -p install/minimal/etc
cp -a install/HDE/*/etc install/minimal/etc/opensplice
cp -a install/HDE/*/bin install/minimal
mkdir -p install/minimal/share/opensplice/cmake
cp opensplice-config.cmake install/minimal/share/opensplice/cmake
cd install/minimal && tar czf opensplice-minimal.tgz lib include etc bin share

set +x

echo "Your minimal package is in install/minimal/opensplice-minimal.tgz."
