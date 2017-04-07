add_executable(llox main.cpp)

target_link_libraries(libllox)

target_link_libraries(llox LINK_PUBLIC libllox)
