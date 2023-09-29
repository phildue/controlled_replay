#!/bin/bash
bagfile=bag
ros2 bag record /chatter -o $bagfile&

ros2 run controlled_replay_example talker&

echo "Recording..."
sleep 5
echo "Finished..."
killall talker
killall ros2

