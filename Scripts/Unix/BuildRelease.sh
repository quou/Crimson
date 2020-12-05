#!/bin/bash

cd ../../
./premake5 gmake

make -j12 config=release

cp bin/Release-linux-x86_64/Game/Game Editor/Game
