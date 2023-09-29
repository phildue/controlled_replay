#!/bin/bash
bagfile=bag

ros2 run controlled_replay_example listener --ros-args -p task_time:=0.01 -r __node:=listener1 &
ros2 run controlled_replay_example listener --ros-args -p task_time:=0.5 -r __node:=listener2 -r chatter:=hearsay -r hearsay:=hearsay1&
ros2 run controlled_replay remote --ros-args -p batch_size:=1 -p use_sim_time:=True &

ros2 bag play --clock --start-paused $bagfile

killall listener
killall ros2
