# Detect Platforms
IF (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    # Windows
    SET (RFC_PLATFORM_WINDOWS ON)

    # Windows Definitions
    ADD_COMPILE_OPTIONS ("/DUNICODE")
    ADD_COMPILE_OPTIONS ("/GS-")
    ADD_COMPILE_OPTIONS ("/Gs9999999")

    ADD_LINK_OPTIONS ("/stack:0x100000,0x100000")
    ADD_LINK_OPTIONS ("/nodefaultlib")
    ADD_LINK_OPTIONS ("/subsystem:console")
    ADD_LINK_OPTIONS ("/entry:WinMainCRTStartup")

ELSEIF (CMAKE_SYSTEM_NAME STREQUAL "Linux")

    # Linux
    SET (RFC_PLATFORM_LINUX ON)
    MESSAGE (WARNING "TODO: Add Linux support!")

ELSEIF (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    # macOS
    SET (RFC_PLATFORM_MACOS ON)
    MESSAGE (WARNING "TODO: Add macOS support!")

ENDIF ()