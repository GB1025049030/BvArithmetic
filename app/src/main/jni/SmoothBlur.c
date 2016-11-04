//
// Created by azmohan on 16-9-2.
//

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <stdio.h>
#include <math.h>

#define LOG_TAG "SmoothBlur"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define GET_R_FROM_B_ENDIAN_RGBA8888(inVal) ((inVal & 0xff000000)>>24)
#define GET_G_FROM_B_ENDIAN_RGBA8888(inVal) ((inVal & 0x00ff0000)>>16)
#define GET_B_FROM_B_ENDIAN_RGBA8888(inVal) ((inVal & 0x0000ff00)>>8)
#define GET_A_FROM_B_ENDIAN_RGBA8888(inVal) (inVal & 0x000000ff)
#define GET_RGBA888_FROM_R_G_B_A(r, g, b, a) (((r << 24) & 0xff000000) | ((g << 16) & 0x00ff0000) | ((b << 8) & 0x0000ff00) | (a & 0x000000ff))

void smoothBlend(AndroidBitmapInfo *info, void *blurPixels, void *oriPixles, jint x, jint y,
                 jint inRadius, jint outRadius) {
    int top = y - outRadius;
    int bottom = y + outRadius;
    int left = x - outRadius;
    int right = x + outRadius;
    if (top < 0) {
        top = 0;
    }
    if (bottom > info->height) {
        bottom = info->height;
    }
    if (left < 0) {
        left = 0;
    }
    if (right > info->width) {
        right = info->width;
    }
    int yy;
    blurPixels = (char *) blurPixels + info->stride * top;
    oriPixles = (char *) oriPixles + info->stride * top;
    for (yy = top; yy < bottom; yy++) {
        uint32_t *blurLine = (uint32_t *) blurPixels;
        uint32_t *oriLine = (uint32_t *) oriPixles;
        int xx;
        for (xx = left; xx < right; xx++) {
            float powX2 = powf(xx - x, 2);
            float powY2 = powf(yy - y, 2);
            int r = (int) sqrtf(powX2 + powY2);
            if (r < inRadius) {
                blurLine[xx] = oriLine[xx];
            } else if (r < outRadius) {

                float scale = (r - inRadius) / (float) (outRadius - inRadius);
                uint32_t blurPixel = blurLine[xx];
                uint32_t oriPixel = oriLine[xx];
                uint32_t alpha = (uint32_t)(GET_A_FROM_B_ENDIAN_RGBA8888(blurPixel) * scale) +
                                 (uint32_t)(GET_A_FROM_B_ENDIAN_RGBA8888(oriPixel) * (1 - scale));
                uint32_t red = (uint32_t)(GET_R_FROM_B_ENDIAN_RGBA8888(blurPixel) * scale) +
                               (uint32_t)(GET_R_FROM_B_ENDIAN_RGBA8888(oriPixel) * (1 - scale));
                uint32_t green = (uint32_t)(GET_G_FROM_B_ENDIAN_RGBA8888(blurPixel) * scale) +
                                 (uint32_t)(GET_G_FROM_B_ENDIAN_RGBA8888(oriPixel) * (1 - scale));
                uint32_t blue = (uint32_t)(GET_B_FROM_B_ENDIAN_RGBA8888(blurPixel) * scale) +
                                (uint32_t)(GET_B_FROM_B_ENDIAN_RGBA8888(oriPixel) * (1 - scale));
                blurLine[xx] = GET_RGBA888_FROM_R_G_B_A(red, green, blue, alpha);
            }
        }
        blurPixels = (char *) blurPixels + info->stride;
        oriPixles = (char *) oriPixles + info->stride;
    }
}

JNIEXPORT void JNICALL Java_com_bvirtual_SmoothBlurJni_smoothRender(JNIEnv *env, jobject obj,
                                                                    jobject blurBitmap,
                                                                    jobject oriBitmap,
                                                                    jobject info_obj) {
    AndroidBitmapInfo blurInfo;
    AndroidBitmapInfo oriInfo;
    void *blurPixels;
    void *oriPixels;
    int ret;
    jclass objClass = (*env)->GetObjectClass(env, info_obj);
    if (objClass == NULL) {
        LOGE("get object class failed");
    }

    jfieldID xId = (*env)->GetFieldID(env, objClass, "x", "I");
    jfieldID yId = (*env)->GetFieldID(env, objClass, "y", "I");
    jfieldID inRadiusId = (*env)->GetFieldID(env, objClass, "inRadius", "I");
    jfieldID outRadiusId = (*env)->GetFieldID(env, objClass, "outRadius", "I");

    jint x = (*env)->GetIntField(env, info_obj, xId);
    jint y = (*env)->GetIntField(env, info_obj, yId);
    jint inRadius = (*env)->GetIntField(env, info_obj, inRadiusId);
    jint outRadius = (*env)->GetIntField(env, info_obj, outRadiusId);

    LOGE("x = %d,y = %d,radius = %d", x, y, inRadius, outRadius);

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
    LOGE("handle smooth blur info format:%d", blurInfo.format);
    smoothBlend(&blurInfo, blurPixels, oriPixels, x, y, inRadius, outRadius);
    AndroidBitmap_unlockPixels(env, blurBitmap);
    AndroidBitmap_unlockPixels(env, oriBitmap);
}

