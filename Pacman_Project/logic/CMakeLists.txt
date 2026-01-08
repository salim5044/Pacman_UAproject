add_library(pacman_logic STATIC
        src/Stopwatch.cpp
        src/Random.cpp
        src/World.cpp
        src/Entity.cpp
        src/Ghost.cpp
        src/Score.cpp
        src/Config.cpp
)

target_include_directories(pacman_logic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(pacman_logic PUBLIC cxx_std_20)

# No SFML dependency allowed in logic lib.