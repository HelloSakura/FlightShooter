# CMake配置

## C++编译流程

**预处理**
把文件内容找到插入到代码对应位置

得知道头文件在那里，cmake指定文件目录
include_directories("xx")

**编译**
指定哪些需要编译的文件

**链接**

链接库也需要指定位置
link_directories("xx")
同时需要指定需要链接的库
target_link_libraries(xxx)

## 最基本CMake
```CMake

# 最小版本
cmake_minimum_required(VERSION 3.10.0)
# 项目名称 版本号 语言
project(FlightShooter VERSION 0.1.0 LANGUAGES C CXX)

# 分平台指定头文件和链接库目录
if(WIN32)
    include_directories("C:/Library/SDL2/include")
    link_directories("C:/Library/SDL2/lib/x64")
elseif(APPLE)
    include_directories("/Library/SDL2/include")
    link_directories("/Library/SDL2/lib/x64")
elseif(LINUX)
    include_directories("/usr/include/SDL2")
    link_directories("/usr/lib/x86_64-linux-gnu")
endif()

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)

# 添加可执行文件
add_executable(FlightShooter main.cpp)

# 指定需要链接的库
target_link_libraries(FlightShooter SDL2 SDL2main)

```

当第三方库有Cmake文件夹时，比较简单的搞法
内置了很多东西，不需要自己再去手动指定平台相关的设置
```Cmake
# 最小版本
cmake_minimum_required(VERSION 3.10.0)
# 项目名称 版本号 语言
project(FlightShooter VERSION 0.1.0 LANGUAGES C CXX)
# 查找并载入CMake预设
find_package(SDL2 REQUIRED)
# 指定C++标准
set(CMAKE_CXX_STANDARD 17)

# 添加可执行文件
add_executable(FlightShooter main.cpp)

# 指定需要链接的库
# target_link_libraries(FlightShooter SDL2 SDL2main)
target_link_libraries(FlightShooter 
                        SDL2::SDL2 
                        SDL2::SDL2main)
```

## CMake框架

**必备**
标题
查找并载入预设
创建可执行文件
指定链接库

**可补充完善**

C++标准
编译器提示
生成程序路径
根据平台名称指定程序名称

cmake关于文件路径的引入和使用