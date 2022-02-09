#!/bin/sh

[ "$(echo "$1" | cut -d'.' -f2)" = "webp" ] ||
    cwebp -q 100 "$1" -o "$(echo "$1" | cut -d'.' -f1).webp"
