# 递归收集当前目录所有cpp，存入变量 framework
file(GLOB_RECURSE framework CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 编译静态库 framework
add_library(glframework STATIC ${framework})

#framework需要能找到 glad、glfw 的头文件
target_include_directories(glframework
    PUBLIC
        ${CMAKE_SOURCE_DIR}/thirdParty/include
)