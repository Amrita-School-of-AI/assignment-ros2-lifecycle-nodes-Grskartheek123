cmake_minimum_required(VERSION 3.8)
project(ros2_lifecycle)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(std_msgs REQUIRED)

# TODO: Add the executable target named 'lifecycle_sensor'
# 1. Add executable using src/lifecycle_sensor.cpp
# 2. Add dependencies (ament_target_dependencies) for rclcpp, rclcpp_lifecycle, std_msgs
# 3. Install the target to the DESTINATION lib/${PROJECT_NAME}

add_executable(lifecycle_sensor src/lifecycle_sensor.cpp)
ament_target_dependencies(lifecycle_sensor rclcpp rclcpp_lifecycle std_msgs)

install(TARGETS
  lifecycle_sensor
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
