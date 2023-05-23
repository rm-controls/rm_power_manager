# Find Git executable
find_package(Git REQUIRED)

# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get whether the current branch is dirty
execute_process(
        COMMAND ${GIT_EXECUTABLE} diff-index --quiet HEAD --
        RESULT_VARIABLE GIT_DIFF_INDEX_RESULT
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_QUIET ERROR_QUIET
)

# Set a flag indicating whether the branch is dirty
if (${GIT_DIFF_INDEX_RESULT} EQUAL 1)
    set(GIT_COMMIT_DIRTY +)
else ()
    set(GIT_COMMIT_DIRTY)
endif ()

# Configure a header file to pass the Git commit hash and dirty flag to the source code
configure_file(
        ${CMAKE_SOURCE_DIR}/../system/version.h.in
        ${CMAKE_SOURCE_DIR}/../system/version.h
)
