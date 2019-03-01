file(GLOB CPP_TD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(
    .
    ${CTP_LIBRARY_DIRS}
)

link_directories(
    ${CTP_SDK_DIR}
)

add_executable(td ${CPP_TD_SRC})

target_link_libraries(td
        ${CTP_LIBRARIES}
        Threads::Threads
)

target_link_libraries(td -static-libgcc -static-libstdc++)

set(BIN_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install)
file(MAKE_DIRECTORY ${BIN_INSTALL_DIR})
file(COPY ${CTP_LIB_FILES} DESTINATION ${BIN_INSTALL_DIR})
# file(COPY ${CMAKE_CURRENT_BINARY_DIR}/Debug/trade.exe DESTINATION ${BIN_INSTALL_DIR})

set(DEBUG_DIR ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${CTP_LIB_FILES} DESTINATION ${DEBUG_DIR})

file(COPY ${CTP_SHAREDLIBFILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

