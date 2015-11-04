# rts
BUILDING:

For dependencies, put .lib (static or import) in lib, .dll (if shared) in system / .exe location.

Dependencies:

GLFW: shared
GLEW: shared
GTEST: https://github.com/google/googletest
  - clone repo
  - open CMakeLists.txt
  - options BUILD_SHARED_LIBS to OFF, gtest_force_shared_crt to ON
  - building should produce static library for inclusion in project

Build with cmake generator flag -G (I used -G "Visual Studio 14 2015 Win64")
