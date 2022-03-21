# Kroki, które należy wykonać w celu uruchomienia mapowania oraz algorytmu eksploracji:

Uruchomić robota a następnie odpalić następujące skrypty w terminalach.

1. vidia Jetson Nano

roslaunch turtlebot3_bringup turtlebot3_robot.launch

roslaunch realsense2_camera rs_camera.launch

2. Komputer

roslaunch rtabmap_ros demo_turtlebot3_navigation.launch localization:=false odom_topic:=/odom  --> RTABmap

roslaunch octomap_server octomap_mapping.launch --> Octomap

rosrun octomap_server naviOccupancyCHECKBOX --> Algorytm eksploracji

