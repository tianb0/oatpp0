project(${CMAKE_PROJECT_NAME})

find_package(oatpp REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main
  PRIVATE
  oatpp::oatpp)

