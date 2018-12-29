emmm 这里面的图挂掉了 懒得再去弄图床 想看图的话可以去我的博客看

https://blog.csdn.net/qq_24889575/article/details/85339741



内容如其名，纯小白从零开始搞这个。原因是学长说要用as链接linux下ndk编译的so库，而不能直接用as生成，那么好的工具，说不用就不用了。不过既然有要求，那咱也不能含糊，脱下裤子，不不不，撸起袖子就是干。      



[TOC]



# 1 搭建的环境

windows10 1803版

VirtualBox 5.2.14版本下的debian-9.5.0-armd64虚拟机

Android studio 3.2        



# 2 linux方面

首先配置好NDK环境

**1下载ndk** https://developer.android.google.cn/ndk/downloads/

**2 解压** unzip <filename.zip>

**3 配置环境变量**

#vim /etc/profile 在文件末尾添加如下

export ANDROID_NDK=“ndk路径”

export PATH="$ANDROID_NDK:$PATH"    

**4.更新系统变量**

#source /etc/profile    

**5.检查ndk环境配置正确与否**

#ndk-build 

//出现如下界面即配置成功

AndroidNDK: Could not find application project directory !

Android NDK: Please define the NDK_PROJECT_PATH variable topoint to it.

/android-ndk-r16b/build/core/build-local.mk:151: *** Android NDK: Aborting    .  Stop.



# 3 linux下用ndk编译成so

新建一个文件夹jni，然后在jni文件夹下

首先需要cpp和h文件，我的只是简单做个例子，如下    

testC.h

```c++
#ifndef TESTC_H
#define TESTC_H
int testC();
#endif
```

testC.cpp

```C++
#include "testC.h"
int testC()
{
	return 6;
}
```

Android.mk

```makefile
LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE :=testC
LOCAL_SRC_FILES:=testC.cpp
include $(BUILD_SHARED_LIBRARY)

APP_ABI := all             // 编译类型为适配所有架构的cpu
APP_PLATFORM := android-28 // 对应版本是28

```

编译命令

```shell
ndk-build APP_ABI=all
```

会在jni同级目录下生成两个文件夹，libs和obj，libs文件夹下即为刚编译的so库    





另： 如果你的cpp或者h文件引入了标准库文件的函数，如

```C++
#include <string>
```

这样执行编译命令的时候会报一个错

```shell
ndk-build error string No such file or directory
```

解决方法是在jni目录下新建一个文件Application.mk

里面添加

```c++
APP_STL := gnustl_static
```

即可找到标准库



# 4 as方面

新建一个项目，记得勾选include C++ support

![1](D:\pic\1.png)

然后一路下一步就可以了，然后把linux下编译生成的so文件 放到app下的libs文件夹下

![2](D:\pic\2.png)

本图已经放进来了



然后编写Cmakelist.txt，里面已经有新建项目是自动生成的东西，注意区分

```shell
# 动态方式加载 libtestC.so
add_library(testC SHARED IMPORTED)
# 设置链接so的路径，${ANDROID_ABI}为so文件的cpu架构类型
set_target_properties(testC  PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libtestC.so)

# 这个原先中有，在其中添加上testC即可
target_link_libraries(testC)
```



因为没有主函数，所以直接在系统自动生成的样例函数中测试自己的so库，也就是native-lib.cpp文件，记得把h问价传进as

![3](D:\pic\3.png)

注意，红框的地方是修改了的



然后编译运行就可以了。    



这里有一个问题，如果没报错但是app闪退，那么就在buiild.gradle android里添加

```makefile
    sourceSets{
        main{
            jniLibs.srcDirs = ['libs']
        }
    }
```



# 5 博客地址

https://blog.csdn.net/qq_24889575/article/details/85339741