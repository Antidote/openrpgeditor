# Copy .so files on Linux to the target App build folder.
# For development:
add_custom_command(TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${NAME}>)

# For distribution:
install(FILES $<TARGET_FILE:SDL3::SDL3>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        RENAME ${NAME}_$<TARGET_FILE_NAME:SDL3::SDL3>)

# Copy assets into app bundle
# For development:
add_custom_command(TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/src/assets
        $<TARGET_FILE_DIR:${NAME}>/../share)

# For distribution:
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/assets/ DESTINATION ${CMAKE_INSTALL_DATADIR})

# Linux app icon setup
configure_file(
        ${PROJECT_SOURCE_DIR}/src/openrpgeditor/Manifests/App.desktop.in
        ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.desktop
        @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.desktop
        DESTINATION share/applications)
install(FILES ${PROJECT_SOURCE_DIR}/src/assets/icons/BaseAppIcon.png
        DESTINATION share/pixmaps
        RENAME ${NAME}_icon.png)
