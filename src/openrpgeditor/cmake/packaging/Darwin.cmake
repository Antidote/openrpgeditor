# Get dynamic SDL2 lib into Frameworks folder in app bundle.
# For development:
add_custom_command(TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${NAME}>/../Frameworks/$<TARGET_FILE_NAME:SDL3::SDL3>)

# For distribution without XCode:
if (NOT "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    install(FILES $<TARGET_FILE:SDL3::SDL3> DESTINATION $<TARGET_FILE_DIR:${NAME}>/../Frameworks/)
endif ()

# macOS package settings
string(TIMESTAMP CURR_YEAR "%Y")
set_target_properties(${NAME} PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        MACOSX_BUNDLE_BUNDLE_VERSION "${BUILD_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Manifests/Info.plist"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_COMPANY_NAMESPACE}.${CMAKE_PROJECT_NAME}"
        MACOSX_BUNDLE_COPYRIGHT "(c) ${CURR_YEAR} ${PROJECT_COMPANY_NAME}"
        INSTALL_RPATH @executable_path/../Frameworks
        RESOURCE "${MACOSX_STATIC_ASSETS}")
