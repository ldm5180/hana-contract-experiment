set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable (hanaexpr-test main.cpp)

# Link the executable to the Reverse library. Since the Reverse library has
# public include directories we will use those link directories when building.
# target_link_libraries (hanaexpr-test LINK_PUBLIC hanaexpr)
