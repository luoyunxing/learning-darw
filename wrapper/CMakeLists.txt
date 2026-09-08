# 递归收集当前目录所有cpp，存入变量 wrapper
file(GLOB_RECURSE wrapper CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 编译静态库 wrapper
add_library(wrapper STATIC ${wrapper})

# wrapper需要能找到 glad、glfw 的头文件
target_include_directories(wrapper
    PUBLIC
        ${CMAKE_SOURCE_DIR}/thirdParty/include
)