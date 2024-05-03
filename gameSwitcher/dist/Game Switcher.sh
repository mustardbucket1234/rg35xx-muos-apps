#!/bin/sh

ROM_LAST=/tmp/rom_last
ROM_LAST_DATA=""
cd "$(realpath "$(dirname "$0")")"
cd ".data"

# Get the data of the current script
# Store it in next_play.txt
if [ -f "$ROM_LAST" ]; then
    ROM_LAST_DATA=$(cat "$ROM_LAST")
    echo "$ROM_LAST_DATA" > "rom_last.txt"
fi

# Launch app executable and wait for it to finish
# Store the exit code in APP_EXIT_CODE
cd "bin"
./app "$(realpath "$0")" &
APP_PID=$!
wait $APP_PID
APP_EXIT_CODE=$?