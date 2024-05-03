#!/bin/sh

ROM_GO=/tmp/rom_go
NEXT_PLAY_FILE="next_play.txt"

HOME=$(realpath "$(dirname "$0")")
cd "$HOME"

sleep 0.3

MAX_WAIT_TIME=50
counter=0

# Continuously check for ROM_GO to not exist within the time limit
while [ -e "$ROM_GO" ] && [ "$counter" -lt "$MAX_WAIT_TIME" ]; do
    ((counter++))
    sleep 0.1
done

if [ ! -e "$ROM_GO" ] && [ -f "$NEXT_PLAY_FILE" ]; then
    NEXT_PLAY_CONTENT=$(cat "$NEXT_PLAY_FILE")
    echo "$NEXT_PLAY_CONTENT" > "$ROM_GO"
    rm "$NEXT_PLAY_FILE"
fi