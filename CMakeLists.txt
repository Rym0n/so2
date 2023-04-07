#cmake_minimum_required(VERSION 3.20)
#project(so2 C)
#set(CMAKE_C_STANDARD 99)

#add_executable(so2 main.c server.c client.c server.h client.h)
#add_link_options(-lpthread)
#add_link_options(-lncursesw)
#include_directories(${CURSES_INCLUDE_DIR})


#target_link_libraries(so2 ${CURSES_LIBRARIES})
#target_link_libraries(so2 ncurses)
#target_link_libraries(so2 rt)
#target_link_libraries(so2 -lrt)

#target_link_libraries(client ${CURSES_LIBRARIES})
#target_link_libraries(client ncurses)
#target_link_libraries(client rt)


cmake_minimum_required(VERSION 3.16)
project(so2 C)

set(CMAKE_C_STANDARD 99)

#add_link_options(-lcurses)
add_link_options(-pthread)
add_link_options(-lncursesw)

#set(CMAKE_CXX_FLAGS "-lcurses")
#target_link_libraries(ncurses)

add_executable(so2 main.c server.c client.c server.h client.h)

target_link_libraries(so2 ncurses)
target_link_libraries(so2 rt)