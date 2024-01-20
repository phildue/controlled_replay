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

add_library(${PROJECT_NAME} SHARED
  src/Remote.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

rclcpp_components_register_node(
    ${PROJECT_NAME}
    PLUGIN "controlled_replay::Remote"
    EXECUTABLE "remote"
)

ament_target_dependencies(${PROJECT_NAME}
  std_msgs
  rclcpp_components
  rosbag2_interfaces
  controlled_replay_interfaces
  )
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING # install only matched files
    PATTERN "*.h*") # select header files

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION include/)
  
install(
    TARGETS ${PROJECT_NAME} remote 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib
    INCLUDES DESTINATION include)
    
if(BUILD_TESTING)
  if(BUILD_LINTING)
    find_package(ament_lint_auto REQUIRED)
    ament_lint_auto_find_test_dependencies()
  endif()
endif()

ament_package()
