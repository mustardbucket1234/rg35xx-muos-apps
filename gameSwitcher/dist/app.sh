#!/bin/sh

ROM_LAST=/tmp/rom_last

cd "$(realpath "$(dirname "$0")")"
cd "data"

# Get the data of the current script
# Store it in next_play.txt
if [ -f "$ROM_LAST" ]; then
    echo "$(cat "$ROM_LAST")" > "next_play.txt"
fi

# Launch app executable and wait for it to finish
# Store the exit code in APP_EXIT_CODE
cd "bin"
./app "$(realpath "$0")" &
APP_PID=$!
wait $APP_PID
APP_EXIT_CODE=$?
cd ..

if [ $APP_EXIT_CODE -ne 0 ]; then
    echo "App was closed"
    rm "next_play.txt"
else
    echo "./app executed successfully. Trigger resumePlay.sh"
    ./resumePlay.sh &
fi