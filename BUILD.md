# How to Build DVDStyler

DVD Styler is a cross-platform application and can be difficult to build across toolchains.
This document is an index for platform-specific guides and in-progress platform stubs.

## Windows

[Windows build instructions](BUILD-WINDOWS.md)

Windows builds currently only support x64 hardware. Native ARM64 support is still experimental.

## macOS

Going foreward only Apple Silicon builds will be officially supported.

DVDStyler will likely still be buildable on Intel hardware (as long as there are still supported x64 third-party libraries), but will no longer targeting or testing on x64 for future MacOS builds.

For macOS you have two choices for dependency management: **Homebrew** or **MacPorts**.
Each has tradeoffs that are discussed in the platform-specific documents.

Currently the MacPorts setup is broken - only the Homebrew process will yield a successful build.

[Homebrew build instructions](BUILD-MACOS-HOMEBREW.md)

[MacPorts build instructions](BUILD-MACOS-MACPORTS.md)

## Linux

[Linux build instructions](BUILD-LINUX.md)

Linux builds are fully supported on both x64 and ARM64 hardware.
