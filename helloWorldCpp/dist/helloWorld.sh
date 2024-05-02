#!/bin/sh
PORTS_FOLDER=$(realpath "$(dirname "$0")")
cd "$PORTS_FOLDER"
cd ".helloWorld"
GAMEDIR="$PORTS_FOLDER"
HOME="$GAMEDIR" SDL_ASSERT=always_ignore ./app