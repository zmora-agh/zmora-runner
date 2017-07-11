
set(SOURCE_FILES
        main.cpp
        Runner.cpp)
add_executable(zmora_runner ${SOURCE_FILES})
install (TARGETS zmora_runner RUNTIME DESTINATION bin)
target_compile_definitions(zmora_runner PRIVATE VERSION="${VERSION}")