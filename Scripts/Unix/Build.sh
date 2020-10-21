#!/bin/bash

cd ../../
./premake5 gmake

bear make -j12 config=debug
