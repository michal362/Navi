# Navi
Komunikacja (Publishery oraz Subscribery) z topicami /scan oraz /odom w folderze nawigacja.
Próby pozyskania danych odnośnie wielkości mapy / zajętych oraz wolnych voxeli w przestrzeni znajdują się w pliku octomap_mapping/octomap_server/src/octo_sub_dane.cpp.
# Uruchamianie symulacji domu:
roslaunch turtlebot3_gazebo turtlebot3_house.launch model:=waffle
# Uruchomienie mapowania RTABMAP:
roslaunch rtabmap_ros demo_turtlebot3_navigation.launch localization:=false model:=waffle
# Uruchomienie mapowania Octomap: (sama powierzchnia podłogi)
roslaunch octomap_server octomap_mapping.launch
