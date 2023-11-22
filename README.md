# MBIST

使用如下方法对代码进行编译：
1. 创建build目录
`mkdir build`
2. 进入build目录
`cd build`
3. 使用cmake命令
`cmake ..`
4. 编译
`make -j32`

如果加入了新的文件，请在CMakeLists.txt文件中的`set(SOURCES Main.cpp parser.cpp writeInfo.cpp parseFile.cpp genetic.cpp)`加入新的文件名

如果需要调试，请先注释`SET(CMAKE_BUILD_TYPE "RELEASE")`以及`SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fPIC -fpermissive")`，重复第3、4步。

在`push`前，请首先确认是在最新版本基础上的修改，并删除`build`目录以及其他用于测试的临时目录！
