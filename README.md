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

如果加入了新的文件，请在CMakeLists.txt文件中的`set(SOURCES Main.cpp parser.cpp)`加入新的文件名
