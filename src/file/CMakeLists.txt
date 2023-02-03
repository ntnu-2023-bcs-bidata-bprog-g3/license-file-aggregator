
target_sources(app
PRIVATE
"fileHandler.cpp" 
"fileHandler.h"
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})