#!/bin/bash

if [[ -f "Grail_Workspace.code-workspace" ]]; then
	echo "\"Grail_Workspace.code-workspace\" found, most likely in Grail's root"
elif [[ $(pwd | awk -F"/" '{print $NF}') == "test" ]]; then
	echo "Current directory is a test directory, assuming Grail's test"
	cd ..
elif [[ -z ${GRAIL} ]]; then # Currently does not find GRAIL
	echo "GRAIL environment variable set"
	cd "${GRAIL}" || exit 1
	pwd
fi

if [ -d "build" ]; then
	echo "Build directory found"
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
	cmake -GNinja -DCMAKE_BUILD_TYPE=DEBUG ..
	ninja
fi

cd .. || exit 1
if [[ ! -L "compile_commands.json" ]]; then
	cp build/compile_commands.json compile_commands.json
fi
