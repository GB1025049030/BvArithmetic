/*
 * File name: mirror_jni.c
 * Description: jni for mirror process
 *
 * Author: Kimi Wu, contact with wuqizhi@droi.com
 * Date: 2016/06/29
 * Copyright (C) 2016 Shanghai Droi Technology Co.,Ltd.
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <android/log.h>
#include <android/bitmap.h>

#define MIRROR_DBG 0
#define LOG_TAG "libmirror"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/* return current time in milliseconds */
double now_ms(void) {
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}

void mirror_bitmap(AndroidBitmapInfo *info, void *pixel, int oriention) {
    if (oriention % 180 == 0) {
        char *charPixels = (char *) pixel;
        int yy = 0;
        for (yy = 0; yy < info->height; yy++) {
            uint32_t *pixelLine = (uint32_t *) charPixels;
            int xx = 0;
            for (xx = 0; xx < info->width / 2; xx++) {
                uint32_t tmp = pixelLine[xx];
                pixelLine[xx] = pixelLine[info->width - xx - 1];
                pixelLine[info->width - xx - 1] = tmp;
            }
            charPixels = charPixels + info->stride;
        }
    } else {
        char *beginCharPixels = (char *) pixel;
        char *endCharPixels = (char *) pixel + info->stride * (info->height - 1);
        int yy = 0;
        for (yy = 0; yy < info->height / 2; yy++) {
            uint32_t *beginLine = (uint32_t *) beginCharPixels;
            uint32_t *endLine = (uint32_t *) endCharPixels;
            int xx = 0;
            for (xx = 0; xx < info->width; xx++) {
                uint32_t tmp = beginLine[xx];
                beginLine[xx] = endLine[xx];
                endLine[xx] = tmp;
            }
            beginCharPixels = beginCharPixels + info->stride;
            endCharPixels = endCharPixels - info->stride;
        }
    }
}

void Java_com_freeme_camera_PhotoModule_mirrorBitmap(JNIEnv *env, jobject object,
                                                     jobject srcBitmap, jint oriention) {
    AndroidBitmapInfo srcInfo;
    void *srcPixels;
    int ret;

    if ((ret = AndroidBitmap_getInfo(env, srcBitmap, &srcInfo)) < 0) {
        LOGE("srcBitmap getInfo failed! error = %d", ret);
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, srcBitmap, &srcPixels)) < 0) {
        LOGE("blurBitmap lockPixels failed ! error = %d", ret);
    }

    LOGI("call mirror..., image width = %d, height = %d, oriention = %d", srcInfo.width,
         srcInfo.height, oriention);
    double t0, t_c;
    t0 = now_ms();
    mirror_bitmap(&srcInfo, srcPixels, oriention);
    t_c = now_ms() - t0;
    LOGI("spent %g ms! process on c.", t_c);
    AndroidBitmap_unlockPixels(env, srcBitmap);
}

