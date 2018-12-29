#include <jni.h>
#include <string>
#include "testC.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_immortals_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    int a = testC();
    if(a == 6)
        hello += "成功调用linux下的so";
    return env->NewStringUTF(hello.c_str());
}
