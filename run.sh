clear
./premake5 gmake2
if make -j12 config=debug; then
	echo === RESTART === 
	
	cd editor
	./../bin/editor
fi
