#!/bin/bash

cmake -DEMSCRIPTEN=1 -DMATH_TESTS_EXECUTABLE=1 -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Platform/Emscripten_unix.cmake -DCMAKE_MODULE_PATH=$EMSCRIPTEN/cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..

