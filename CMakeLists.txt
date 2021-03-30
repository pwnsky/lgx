PROJECT(HELLO)

set( CMAKE_CXX_FLAGS "-std=c++14" )
MESSAGE(STATUS "Project: SERVER") 
PROJECT(lgx)
#add_definitions("-DEFAULT_CONFIG_FILE")
add_definitions("-DDEFAULT_CONFIG_FILE=\"/etc/lgx/conf.json\"")

include_directories(
${PROJECT_SOURCE_DIR}/src
${PROJECT_SOURCE_DIR}/src/net
${PROJECT_SOURCE_DIR}/src/log
${PROJECT_SOURCE_DIR}/src/util
${PROJECT_SOURCE_DIR}/src/util/mysql
${PROJECT_SOURCE_DIR}/src/work
${PROJECT_SOURCE_DIR}/src/thread
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
)

link_directories(/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} pthread sqlclient)

