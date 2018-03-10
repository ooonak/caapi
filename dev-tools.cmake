# Additional target to perform clang-format/clang-tidy run
# Requires clang-format, clang-tidy and cppcheck

# Get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.hpp)

add_custom_target(
        clang-format
        COMMAND /usr/bin/clang-format
        -style=llvm
        -i
        ${ALL_SOURCE_FILES}
)

add_custom_target(
        clang-tidy
        COMMAND /usr/bin/clang-tidy
        -checks='*'
        -fix
        ${ALL_SOURCE_FILES}
        --
        -std=c++14
        ${INCLUDE_DIRECTORIES}
)

add_custom_target(
        cppcheck
        COMMAND /usr/bin/cppcheck
        --std=c++14
        --language=c++
        --enable=warning,style,performance,portability,information,unusedFunction,missingInclude
        --verbose
        --quiet
        ${ALL_SOURCE_FILES}
)
