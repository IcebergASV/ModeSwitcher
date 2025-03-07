cmake_minimum_required(VERSION 3.8)  # Match your working example's version
project(waypoint_counter)

# Set C++17 explicitly (matches your working example)
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# Enable compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find ROS 2 packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(mavros_msgs REQUIRED)

# Install launch files.
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

# Declare executable
add_executable(tracker 
  src/tracker.cpp  # Ensure this path matches your actual file location
)

# Link dependencies
ament_target_dependencies(tracker
  rclcpp
  mavros_msgs
)

# Explicitly enforce C++17 (matches your working example)
target_compile_features(tracker PUBLIC cxx_std_17)

# Install executable to `lib/waypoint_counter` (same as your working example)
install(TARGETS tracker
  DESTINATION lib/${PROJECT_NAME}
)


# Finalize ROS 2 package
ament_package()