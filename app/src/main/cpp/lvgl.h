//
// Created by a4455jkjh on 2021/10/9.
//

#ifndef LVGL_LVGL_H
#define LVGL_LVGL_H
#include <jni.h>


void native_create(JNIEnv *env, jclass cls, jobject surface);
void native_changed(JNIEnv *env, jclass cls, jobject surface, jint width, jint height, jint type);
void native_destroy(JNIEnv *env, jclass cls, jobject surface);
void native_touch(JNIEnv *env, jclass cls, jint, jint, jboolean);

#endif //LVGL_LVGL_H
