

cmake_minimum_required(VERSION 3.4.1)

set( OpenCV_DIR /home/wma/sdk/OpenCV-android-sdk/sdk/native/jni )

find_package(OpenCV REQUIRED )
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV library status:")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
else(OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV library not found")
endif(OpenCV_FOUND)


include(/home/wma/project/openCVTest/app/src/main/dlib/cmake)


add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )


find_library(
              log-lib

              log )



target_link_libraries( # Specifies the target library.
                       native-lib
                       dlib::dlib
                       -ljnigraphics
                       ${OpenCV_LIBS}
                       ${log-lib} )