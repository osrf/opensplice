#!/bin/bash

#echo "WARNING: this script is brittle; be careful."
#echo "Press any key to continue (and hope for the best)."
#read

# Allow the caller to specify the build type on the command line
. configure $*
make
make install
set -x
rm -rf install/minimal
mkdir -p install/minimal
mkdir -p install/minimal/lib
cp -aL lib/*/* install/minimal/lib
pushd .
cd install/minimal/lib
for f in *so; do mv -v $f $f.6.4; ln -v -s $f.6.4 $f;done
popd
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
