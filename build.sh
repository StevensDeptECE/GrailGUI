#!/usr/bin/env bash

if [[ $(pwd | awk -F"/" '{print $NF}') == "test" ]]; then
	cd ..
elif [[ -z "$GRAIL" ]]; then # Currently does not find GRAIL
	cd "$GRAIL" || exit 1
fi

if [ -d "build" ]; then
	cd "build" || exit 1
else
	if [[ -f "CMakeLists.txt" ]]; then
		mkdir build
		cd "build" || exit 1
	else
		{
			echo "Error: no CMakeLists.txt, no build directory"
			echo "Are you in the right directory?"
		} >/dev/stderr
		exit 1
	fi
fi

if [ -f "build.ninja" ]; then
	ninja
else
	cmake -GNinja ..
	ninja
fi
