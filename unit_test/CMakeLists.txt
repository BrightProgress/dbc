#use Google test
include(GoogleTest)

# enable use of googletest and google-benchmark
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

link_libraries(gtest_main)
link_libraries(dbc)


add_definitions(-DDBC_ENABLED)
add_executable(dbc_unit dbcunit.cpp)
# Only test DBC in debug configuration!

gtest_discover_tests(dbc_unit PROPERTIES DISABLED $<$<NOT:$<CONFIG:Debug>>:True>)
