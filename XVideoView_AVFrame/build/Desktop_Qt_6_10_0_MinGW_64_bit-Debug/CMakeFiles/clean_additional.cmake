# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\XVideoView_AVFrame_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\XVideoView_AVFrame_autogen.dir\\ParseCache.txt"
  "XVideoView_AVFrame_autogen"
  )
endif()
