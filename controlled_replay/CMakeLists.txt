cmake_minimum_required(VERSION 3.8)
project(controlled_replay)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rosbag2_interfaces REQUIRED)
find_package(controlled_replay_interfaces REQUIRED)

add_library(controlled_replay_nodes SHARED
  src/Remote.cpp)

target_include_directories(controlled_replay_nodes PUBLIC include)
set_property(TARGET controlled_replay_nodes PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_features(controlled_replay_nodes PUBLIC cxx_std_20)

rclcpp_components_register_node(
    controlled_replay_nodes
    PLUGIN "controlled_replay::Remote"
    EXECUTABLE "remote"
)

ament_target_dependencies(controlled_replay_nodes
  std_msgs
  rclcpp_components
  rosbag2_interfaces
  controlled_replay_interfaces
  )
ament_export_include_directories(include)
  
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING # install only matched files
    PATTERN "*.h*") # select header files

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION include/)
  
install(
    TARGETS controlled_replay_nodes remote 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib
    INCLUDES DESTINATION include)
    
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

ament_package()
