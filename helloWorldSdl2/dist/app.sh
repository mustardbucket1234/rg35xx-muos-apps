#!/bin/sh
PORTS_FOLDER=$(realpath "$(dirname "$0")")
cd "$PORTS_FOLDER"
cd "data"
PORTS_FOLDER=$(pwd)
GAMEDIR="$PORTS_FOLDER"
HOME="$GAMEDIR" SDL_ASSERT=always_ignore ./app