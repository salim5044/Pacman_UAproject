add_executable(pacman
        src/main.cpp
        src/Game.cpp
        src/ConcreteFactory.cpp
        src/Menustate.cpp
        src/Playstate.cpp
        src/Gameoverstate.cpp
        src/Victorystate.cpp
        src/PausedState.cpp
)

if(WIN32)
    # 1) SFML dll's kopiëren
    set(SFML_BIN_DIR "${SFML_DIR}/../../../bin")
    file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")

    add_custom_command(TARGET pacman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_DLLS}
            $<TARGET_FILE_DIR:pacman>
    )

    # 2) MinGW runtime dll's kopiëren (libstdc++, libgcc, winpthread)
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

    foreach(dll libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll)
        add_custom_command(TARGET pacman POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MINGW_BIN_DIR}/${dll}"
                $<TARGET_FILE_DIR:pacman>
        )
    endforeach()
endif()


target_compile_features(pacman PRIVATE cxx_std_20)
target_include_directories(pacman PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src)


find_package(SFML 2.5 CONFIG REQUIRED COMPONENTS graphics window system audio)

# SFML 2.6.x (config) gebruikt typisch targets zoals: sfml-graphics, sfml-window, ...
if(TARGET sfml-graphics)
    target_link_libraries(pacman PRIVATE
            pacman_logic
            sfml-graphics sfml-window sfml-system sfml-audio
    )
    # Sommige setups bieden ook SFML::Graphics aliassen
elseif(TARGET SFML::Graphics)
    target_link_libraries(pacman PRIVATE
            pacman_logic
            SFML::Graphics SFML::Window SFML::System SFML::Audio
    )
else()
    message(FATAL_ERROR "SFML targets not found. Check SFML_DIR points to .../lib/cmake/SFML")
endif()



# Copy assets next to the executable
add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:pacman>/assets
)