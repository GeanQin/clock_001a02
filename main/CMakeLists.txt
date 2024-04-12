file(GLOB UI_SRC ./display_task/*)
file(GLOB FS_SRC ./littlefs_util/*)
file(GLOB AP_SRC ./http_server/*)
file(GLOB PARA_SRC ./param/*)
file(GLOB BTN_SRC ./button/*)
file(GLOB STA_SRC ./http_client/*)
file(GLOB JSON_SRC ./cjson/*)
set(srcs "main.c")

idf_component_register(SRCS ${srcs} ${UI_SRC} ${FS_SRC} ${AP_SRC} ${PARA_SRC} ${BTN_SRC} ${STA_SRC} ${JSON_SRC}
                       INCLUDE_DIRS "." "display_task" "littlefs_util" "http_server" "param" "button" "http_client" "cjson")

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    littlefs_create_partition_image(storage ./res FLASH_IN_PROJECT)
else()
    fail_at_build_time(littlefs "Windows does not support LittleFS partition generation")
endif()