project(examples)

add_executable(DirectoriesExample directories/main.c)
target_include_directories(DirectoriesExample PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(DirectoriesExample PUBLIC ExtendedAPI)

add_executable(ProcessesExample process/main.c)
target_include_directories(ProcessesExample PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ProcessesExample PUBLIC ExtendedAPI)