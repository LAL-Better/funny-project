#!/bin/bash
colcon build --packages-up-to smart_car
source install/local_setup.bash
colcon build --packages-up-to car_publish_test
source install/local_setup.bash
