cmake_minimum_required(VERSION 3.5)
project(smart_car)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

find_package(rosidl_default_generators REQUIRED)

include_directories(include)

set(msg_files
  "msg/Car.msg"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
)

ament_export_dependencies(rosidl_default_runtime)

find_package(rclcpp REQUIRED)



add_executable(publish_car src/mqtt_client.c src/publish_car.cpp)

target_link_libraries(publish_car paho-mqtt3c paho-mqtt3a)
ament_target_dependencies(publish_car rclcpp)




add_executable(subscriber_car src/subscriber_car.cpp)
ament_target_dependencies(subscriber_car rclcpp)

install(TARGETS
    publish_car
    subscriber_car
  DESTINATION lib/${PROJECT_NAME})

rosidl_target_interfaces(publish_car
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
rosidl_target_interfaces(subscriber_car
  ${PROJECT_NAME} "rosidl_typesupport_cpp")


ament_package()
