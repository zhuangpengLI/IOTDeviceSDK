set(include_dirs core core/sysdep core/utils)
file(GLOB c_sdk_srcs
    "core/*.c"
    "core/utils/*.c"
    "core/sysdep/*.c"
    "portfiles/aiot_port/posix_port.c"
    "external/*.c")
idf_component_register(SRCS ${c_sdk_srcs}
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES mbedtls)