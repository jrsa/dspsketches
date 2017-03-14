cmake_minimum_required(VERSION 3.3)

include_directories(
    "/usr/local/Cellar/stk/4.5.0/include/"
    "/usr/local/Cellar/fftw/3.3.4_1/include/"
    "/opt/rtaudio"
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(libs
   "/usr/local/Cellar/stk/4.5.0/lib/libstk.a"
    "/usr/local/Cellar/fftw/3.3.4_1/lib/libfftw3.3.dylib"
    "/opt/rtaudio/librtaudio.dylib"
    "/System/Library/Frameworks/CoreAudio.framework"
    "/System/Library/Frameworks/Foundation.framework"
)

add_executable(one one.cc)
target_link_libraries(one ${libs})