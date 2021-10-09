#include "lvgl.h"
static JNINativeMethod methods [] = {
        {"native_create", "(Landroid/view/Surface;)V", (void*)native_create},
        {"native_changed", "(Landroid/view/Surface;III)V", (void*)native_changed},
        {"native_destroy", "(Landroid/view/Surface;)V", (void*)native_destroy},
        {"native_touch", "(IIZ)V", (void*)native_touch}
};
static JavaVM *vm;
jint JNI_OnLoad(JavaVM *v, void *_unused) {
    vm = v;
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    jclass clazz;
    clazz = env->FindClass("lvgl/LVGL");
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, 4) < 0) {
        return JNI_FALSE;
    }
    return JNI_VERSION_1_4;
}