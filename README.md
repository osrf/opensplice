# OpenSplice

This is a fork of PrismTech's OpenSplice package, with changes primarily to
improve the build process.

## Build instructions for Linux

1. Install prerequisites.  If you find something missing here, please add it.

        sudo apt-get install build-essential flex bison gawk

1. Create a directory to build in (note that the directory `build` exists as
part of the OpenSplice source tree):

        mkdir build2
        cd build2

1. Configure and build.  This looks like CMake, but that's just a thin wrapper
around OpenSplice's own, custom build tool.  After running their build, we
heuristically collect the resulting artifacts into `../install/minimal`.

        cmake ..
        make

## Running programs

The above procedure should build the libraries and the demos (of which there
seems to be just one, `ishapes`).  To run a program that uses the OpenSplice
libraries, you need to have some environment context established, both to
find the libraries, and to let OpenSplice know where to find its configuration:

    cd ../install/minimal
    export OSPL_HOME=`pwd`
    export PATH=$OSPL_HOME/bin:$PATH
    export LD_LIBRARY_PATH=$OSPL_HOME/lib:$LD_LIBRARY_PATH
    export OSPL_URI=file://$OSPL_HOME/etc/opensplice/config/ospl.xml

Now you can run a demo, e.g.:

    # demo_ishapes is in $OSPL_HOME/bin
    demo_ishapes
