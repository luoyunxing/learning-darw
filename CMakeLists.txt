cmake_minimum_required(VERSION 3.13)

project(Opengl_lecture)

if (MSVC)
    add_compile_options("/utf-8")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#加入一个全局预编译宏
#add_definitions(-DDEBUG)


#将子文件夹加入进来
add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)


# 本工程所有.cpp文件编译链接，生成exe
file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})


#-------------assets资源复制--------------------
file(GLOB ASSETS "./assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})
#-----------------结束---------------------------


# 头文件路径
target_include_directories(${PROJECT_NAME}
    PRIVATE
        SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
        ${GLAD_ROOT}/include
)

# 库搜索路径：告诉链接器去哪里找 .lib 文件
target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
        ${GLAD_ROOT}/lib
)

#链接lib库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad
        glfw3
        opengl32
        wrapper
        application
        glframework
)
