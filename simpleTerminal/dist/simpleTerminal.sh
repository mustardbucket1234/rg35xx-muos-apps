#!/bin/sh

# muOS v11 compatibility
if [ -d "/usr/lib32" ]; then
    export LD_LIBRARY_PATH=/usr/lib32
fi

PORTS_FOLDER=$(realpath "$(dirname "$0")")
cd "$PORTS_FOLDER"
cd ".simpleTerminal"
GAMEDIR="$PORTS_FOLDER"
HOME="$GAMEDIR" SDL_ASSERT=always_ignore ./app