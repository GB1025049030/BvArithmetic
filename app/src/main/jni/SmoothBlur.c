//
// Created by azmohan on 16-9-2.
//

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LOG_TAG "SmoothBlur"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


JNIEXPORT void JNICALL Java_com_bvirtual_SmoothBlurJni_smoothRender(JNIEnv *env, jobject obj,
                                                                    jobject blurBitmap,
                                                                    jobject oriBitmap) {
    AndroidBitmapInfo blurInfo;
    AndroidBitmapInfo oriInfo;
    void *blurPixels;
    void *oriPixels;
    int ret;

    if ((ret = AndroidBitmap_getInfo(env, blurBitmap, &blurInfo)) < 0) {
        LOGE("blurBitmap getInfo failed! error = %d", ret);
        return;
    }

    if ((ret = AndroidBitmap_getInfo(env, oriBitmap, &oriInfo)) < 0) {
        LOGE("oriBitmap getInfo failed! error = %d", ret);
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, blurBitmap, &blurPixels)) < 0) {
        LOGE("blurBitmap lockPixels failed ! error = %d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, oriBitmap, &oriPixels)) < 0) {
        LOGE("oriBitmap lockPixels failed ! error = %d", ret);
    }
    LOGI("handle smooth blur");
    AndroidBitmap_unlockPixels(env, blurBitmap);
    AndroidBitmap_unlockPixels(env, oriBitmap);

}