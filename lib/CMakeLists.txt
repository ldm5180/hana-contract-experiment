file(GLOB SRCS *.cpp)

# Shared library. Remove "SHARED" to get a statically linked library.
add_library (hanaexpr SHARED ${SRCS})

 # Make sure the compiler can find include files for our library.
target_include_directories (hanaexpr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
