# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/sakura-assistant_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/sakura-assistant_autogen.dir/ParseCache.txt"
  "sakura-assistant_autogen"
  )
endif()
