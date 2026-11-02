# 递归收集当前目录cpp/h
file(GLOB_RECURSE app_src CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# 静态库目标名统一为 application，和顶层link对应上
add_library(application STATIC ${app_src})

# PUBLIC：库自己用，同时链接这个库的exe也继承头文件路径
target_include_directories(application
    PUBLIC
        ${CMAKE_SOURCE_DIR}/thirdParty/include
)
