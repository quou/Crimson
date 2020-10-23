#!/bin/bash

cd ../../
./premake5 gmake
./premake5 compiledb

make -j12 config=debug
