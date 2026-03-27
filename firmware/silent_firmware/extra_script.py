# Extra build script for PlatformIO
Import("env")

# Add custom build flags for LTO
env.Append(
    LINKFLAGS=[
        "-flto",
        "-fuse-linker-plugin",
    ],
    CFLAGS=[
        "-flto",
    ],
    CXXFLAGS=[
        "-flto",
    ],
)