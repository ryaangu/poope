# Setup Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

# Setup Project Name
PROJECT (poope)

# Setup Compiler
SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)

# Include macros
INCLUDE ("cmake/config.cmake")

# Setup Sources
FILE (
    GLOB SOURCES

    # Main
    "source/*.cpp"
    
    # Compiler
    "source/compiler/*.cpp"

    # Core
    "source/core/*.cpp"

    # System
    "source/core/windows/*.cpp"
)

# Setup Executable
ADD_EXECUTABLE (poope ${SOURCES})

# Setup Include Directories
TARGET_INCLUDE_DIRECTORIES (poope PRIVATE "source")