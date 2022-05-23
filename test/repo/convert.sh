#!/bin/sh

# Converts an image from its current encoding to webp encoding.
[ "$(echo "$1" | cut -d'.' -f2)" = "webp" ] ||
    cwebp -q 100 "$1" -o "$(echo "$1" | cut -d'.' -f1).webp"
