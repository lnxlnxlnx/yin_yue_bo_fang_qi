# 最低CMake版本要求（选常用的3.10，可根据环境修改）
cmake_minimum_required(VERSION 3.10)

# 项目名称 + 编程语言（C语言，若用C++则写CXX）
project(ycx_mp4 C)

# 设置C语言标准（可选，根据项目需求，比如C99）
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 定义可执行文件名称+源文件列表（对应Makefile的OBJ=ycx_mp4、OBJS=*.c）
# 源文件直接写文件名，无目录则无需路径
add_executable(ycx_mp4
    main.c
    do.c
    show.c
)

# 头文件包含（当前目录，因为头文件和源文件同目录，可省略，写了更规范）
target_include_directories(ycx_mp4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ========== 安装规则（对应Makefile的install: sudo cp 到/bin） ==========
# 安装可执行文件到系统/bin目录（CMAKE_INSTALL_PREFIX默认/usr/local，这里强制指定/bin）
install(TARGETS ycx_mp4
    RUNTIME DESTINATION /bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE  # 赋予可执行权限，和系统命令一致
)

# ========== 自定义uninstall目标（对应Makefile的uninstall） ==========
# CMake默认无uninstall，手动定义，实现删除/bin/ycx_mp4
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
        COMMENT "Uninstall ycx_mp4 from /bin"
    )
endif()