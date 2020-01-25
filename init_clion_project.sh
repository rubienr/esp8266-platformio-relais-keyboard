#!/bin/bash
BASEDIR=$(dirname "$0")

pushd `pwd`
    cd $BASEDIR
    pio init --ide clion
popd
