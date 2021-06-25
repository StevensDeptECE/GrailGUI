#!/usr/bin/env bash

if [ -d "build" ]; then
	cd "build" || exit 1
	pwd
	ninja
else
	echo "build directory \"build\" does not exist"
	echo "Please run ./configure.sh"
fi
