#!/bin/bash

cd ../../
./premake5 gmake

make -j12 config=debug
