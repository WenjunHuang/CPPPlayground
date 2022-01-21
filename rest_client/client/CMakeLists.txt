if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CARGO_CMD cargo build)
  set(TARGET_DIR "debug")
else()
  set(CARGO_CMD cargo build --release)
  set(TARGET_DIR "release")
endif()

set(CLIENT_SO "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/client.dll")

add_custom_target(client ALL
                  COMMENT "Compiling client module"
                  COMMAND CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CARGO_CMD}
                  COMMAND ${CMAKE_COMMAND} -E copy ${CLIENT_SO} ${CMAKE_CURRENT_BINARY_DIR}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(client PROPERTIES LOCATION ${CMAKE_CURRENT_BINARY_DIR})