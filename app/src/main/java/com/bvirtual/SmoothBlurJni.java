package com.bvirtual;

import android.graphics.Bitmap;
import android.graphics.Rect;

/**
 * Created by azmohan on 16-9-2.
 */
public class SmoothBlurJni {
    static {
        System.loadLibrary("SmoothBlur");
    }

    public static native void smoothRender(Bitmap blurBitmap, Bitmap oriBitmap,BlurInfo info);
}
