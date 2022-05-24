#!/bin/bash
# A custom sway bar to show date and pomodoro duration.
path=$(dirname $(realpath $0))
date_formatted=$(date "+%a %F %H:%M")
pomodoro_time=$(sh $path/pomo/pomo.sh clock)
echo $pomodoro_time '|' $date_formatted
