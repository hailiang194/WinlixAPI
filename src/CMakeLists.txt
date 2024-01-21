set(API_LIB_PUBLIC_HEADERS
    "${PROJECT_SOURCE_DIR}/include/api/basetype.h"
    "${PROJECT_SOURCE_DIR}/include/api/string/safe.h"
    "${PROJECT_SOURCE_DIR}/include/api/string/validate.h"
    "${PROJECT_SOURCE_DIR}/include/api/directory/directory.h"
    "${PROJECT_SOURCE_DIR}/include/api/directory/path.h"
    "${PROJECT_SOURCE_DIR}/include/api/process/process.h"   
    "${PROJECT_SOURCE_DIR}/include/api/network/interface.h" 
    "${PROJECT_SOURCE_DIR}/include/api/ipc/shared-memory.h"
    "${PROJECT_SOURCE_DIR}/include/api/ipc/message-queue.h"
)

set(API_LIB_SOURCES
    "string/safe.c"
    "string/validate.c"
    "directory/directory.c"
    "directory/path.c"
    "process/process.c"
    "network/interface.c"
    "ipc/shared-memory.c"
    "ipc/message-queue.c"
)

add_library(ExtendedAPI ${API_LIB_SOURCES} ${API_LIB_PUBLIC_HEADERS})

if(BUILD_SHARED_LIBS)
    message(STATUS "Build shared lib for extended api")
    if (WIN32)
        target_compile_definitions(ExtendedAPI
                                   PRIVATE $<BUILD_INTERFACE:BUILD_LIBTYPE_SHARED>
                                   INTERFACE $<INSTALL_INTERFACE:USE_LIBTYPE_SHARED>
                                   )
    endif ()
    set_property(TARGET ExtendedAPI PROPERTY POSITION_INDEPENDENT_CODE ON)
else()
    message(STATUS "Build static lib for extended api")
endif()

target_include_directories(ExtendedAPI
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>

)