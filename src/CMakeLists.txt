add_executable (vollo
    controller.cpp
    delivery_flag.cpp
    gameplay.cpp
    main.cpp
    physics.cpp
    scene.cpp
    timer.cpp
)

add_subdirectory (math)

target_include_directories (vollo PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries (vollo ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

install (TARGETS vollo DESTINATION bin)
