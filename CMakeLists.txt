# 最小版本
cmake_minimum_required(VERSION 3.10.0)

# 项目名称 版本号 语言
project(FlightShooter VERSION 0.1.0 LANGUAGES C CXX)

# 分平台指定头文件和链接库目录
# if(WIN32)
#     include_directories("C:/Library/SDL2/include")
#     link_directories("C:/Library/SDL2/lib/x64")
# elseif(APPLE)
#     include_directories("/Library/SDL2/include")
#     link_directories("/Library/SDL2/lib/x64")
# elseif(LINUX)
#     include_directories("/usr/include/SDL2")
#     link_directories("/usr/lib/x86_64-linux-gnu")
# endif()

# 查找并载入CMake预设
# 注意：SDL2 系列库的配置文件使用了旧的 cmake_minimum_required 语法
# 如果看到弃用警告，可以运行 fix_sdl2_configs.bat 来修复这些文件
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# 指定C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /std:c++20)
else()
    add_compile_options(-Wall -Wextra -std=c++20)
endif()

#设置编译输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/output/release)

# 根据平台指定程序名称
# 设置变量
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 添加可执行文件
add_executable(${TARGET} 
            "src/main.cpp"
            "src/Game.cpp"
            "src/Scene.cpp"
            "src/SceneMain.cpp"
            "src/SceneTitle.cpp"
            "src/SceneEnd.cpp")

# 指定需要链接的库
# target_link_libraries(FlightShooter SDL2 SDL2main)
target_link_libraries(${TARGET} 
                        ${SDL2_LIBRARIES}
                        SDL2_image::SDL2_image
                        SDL2_mixer::SDL2_mixer
                        SDL2_ttf::SDL2_ttf)

# 复制资源文件到输出目录（根据配置自动选择目标目录）
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${TARGET}>/assets"
    COMMENT "Copying assets to output directory"
)