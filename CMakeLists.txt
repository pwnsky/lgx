# Author: i0gan
# Date  : 2021-04-05
# Camke script for lgx

set(CMAKE_CXX_FLAGS 
    "-std=c++14 -O3"
)

MESSAGE(STATUS "Project: SERVER") 
PROJECT(lgx)
set(INSTALL_CONFIG_PATH "/etc/lgx")
set(INSTALL_BINARY_PATH "/usr/bin")
set(INSTALL_WWW_PATH "/var")

include_directories(
${PROJECT_SOURCE_DIR}/src
)

set(SRC
${PROJECT_SOURCE_DIR}/src/main.cc
${PROJECT_SOURCE_DIR}/src/start_up.cc
${PROJECT_SOURCE_DIR}/src/net/epoll.cc
${PROJECT_SOURCE_DIR}/src/net/eventloop.cc
${PROJECT_SOURCE_DIR}/src/net/channel.cc
${PROJECT_SOURCE_DIR}/src/net/eventloop_thread.cc
${PROJECT_SOURCE_DIR}/src/net/eventloop_threadpool.cc
${PROJECT_SOURCE_DIR}/src/net/util.cc
${PROJECT_SOURCE_DIR}/src/net/net.cc
${PROJECT_SOURCE_DIR}/src/net/timer.cc
${PROJECT_SOURCE_DIR}/src/net/http.cc
${PROJECT_SOURCE_DIR}/src/thread/thread.cc
${PROJECT_SOURCE_DIR}/src/util/util.cc
${PROJECT_SOURCE_DIR}/src/util/firewall.cc
${PROJECT_SOURCE_DIR}/src/util/md5.cc
${PROJECT_SOURCE_DIR}/src/util/mysql/sql.cc
${PROJECT_SOURCE_DIR}/src/util/mysql/query.cc
${PROJECT_SOURCE_DIR}/src/work/work.cc
${PROJECT_SOURCE_DIR}/src/work/chat.cc
${PROJECT_SOURCE_DIR}/src/log/log.cc
${PROJECT_SOURCE_DIR}/src/log/log_thread.cc
)

link_directories(/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} pthread mysqlclient ssl crypto dl curses)

INSTALL(CODE "MESSAGE(\"LGX Installing...\")")

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_BINARY_PATH}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/conf/ DESTINATION ${INSTALL_CONFIG_PATH}
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/www DESTINATION ${INSTALL_WWW_PATH}
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
