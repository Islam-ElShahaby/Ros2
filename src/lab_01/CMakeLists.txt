cmake_minimum_required(VERSION 3.8)
project(lab_01)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(cpu_temperature_publisher src/Task1/CPU_Temperature_Publisher.cpp)
ament_target_dependencies(cpu_temperature_publisher rclcpp std_msgs)

add_executable(command_velocity_limiter src/Task3/Command_Velocity_Limiter.cpp)
ament_target_dependencies(command_velocity_limiter rclcpp geometry_msgs)


add_executable(odometry_publisher src/Task4/Odometry_Publisher.cpp)
ament_target_dependencies(odometry_publisher rclcpp nav_msgs)

add_executable(imu_csv_playback src/Task5/IMU_CSV_Playback.cpp)
ament_target_dependencies(imu_csv_playback rclcpp sensor_msgs)


install(TARGETS
  cpu_temperature_publisher
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  command_velocity_limiter
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  odometry_publisher
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  imu_csv_playback
  DESTINATION lib/${PROJECT_NAME}
)

install(PROGRAMS
  src/Task2/distance_sensor_sim.py
  src/Task2/emergency_stop.py
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY src/Task2/launch/
  DESTINATION share/${PROJECT_NAME}/launch)

ament_package()
