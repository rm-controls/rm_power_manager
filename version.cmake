# Find Git executable
find_package(Git REQUIRED)

# Get whether the current branch is dirty
execute_process(
        COMMAND ${GIT_EXECUTABLE} diff-index --quiet HEAD --
        RESULT_VARIABLE GIT_DIFF_INDEX_RESULT
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_QUIET ERROR_QUIET
)

# Set a flag indicating whether the branch is dirty
if (${GIT_DIFF_INDEX_RESULT} EQUAL 1)
    # Get the latest abbreviated commit hash of the working branch
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short=15 HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(GIT_COMMIT_DIRTY +)
else ()
    # Get the latest abbreviated commit hash of the working branch
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short=16 HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(GIT_COMMIT_DIRTY)
endif ()

# Get the current system time
message(STATUS "Operation system is ${CMAKE_HOST_SYSTEM_NAME}")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    execute_process(
            COMMAND date +"%Y-%m-%d %H:%M"
            OUTPUT_VARIABLE BUILD_TIME
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REPLACE "\"" "" BUILD_TIME ${BUILD_TIME})
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    execute_process(
            COMMAND powershell -Command "Get-Date -Format 'yyyy-MM-dd HH:mm'"
            OUTPUT_VARIABLE BUILD_TIME
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

message(STATUS "GIT_HASH ${GIT_COMMIT_HASH}${GIT_COMMIT_DIRTY}")
message(STATUS "BUILD_TIME ${BUILD_TIME}")

# Configure a header file to pass the Git commit hash and dirty flag to the source code
configure_file(
        ${CMAKE_SOURCE_DIR}/../system/version.h.in
        ${CMAKE_SOURCE_DIR}/../system/version.h
)
