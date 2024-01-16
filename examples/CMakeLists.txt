project(examples)

add_executable(DirectoriesExample directories/main.c)
target_include_directories(DirectoriesExample PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(DirectoriesExample PUBLIC ExtendedAPI)


add_executable(ProcessesExample process/main.c)
target_include_directories(ProcessesExample PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ProcessesExample PUBLIC ExtendedAPI)

add_executable(ProcessesSignalExample process/signals.c)
target_include_directories(ProcessesSignalExample PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ProcessesSignalExample PUBLIC ExtendedAPI)
if(WIN32)
    set_target_properties(ProcessesSignalExample PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")
endif()

add_executable(NetworkInterfaceExample network/interface.c)
target_include_directories(NetworkInterfaceExample PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(NetworkInterfaceExample PUBLIC ExtendedAPI)