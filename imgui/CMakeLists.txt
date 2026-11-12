# 递归收集当前imgui目录所有cpp
file(GLOB_RECURSE imgui CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 编译静态库 imgui
add_library(imgui STATIC ${imgui})

# imgui 头文件，PUBLIC 让链接它的exe自动继承头文件路径
target_include_directories(imgui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
        ${CMAKE_SOURCE_DIR}/thirdParty/include
)