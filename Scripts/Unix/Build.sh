#!/bin/bash

cd ../../
./premake5 gmake

make -j12 config=debug

cp bin/Debug-linux-x86_64/Game/Game Editor/Game
