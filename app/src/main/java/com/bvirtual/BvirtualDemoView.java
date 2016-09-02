package com.bvirtual;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;


/**
 * Created by azmohan on 16-9-1.
 */
public class BvirtualDemoView extends View {
    private Bitmap mPreviewBitmap;
    private int mOnSingleX;
    private int mOnSingleY;
    private GaussianBlur mGaussianBlur;
    private final static int IN_SHARPNESS_SIZE = 200;
    private final static int MID_SHARPNESS_SIZE = 400;
    private final static int OUT_SHARPNESS_SIZE = 300;

    public static class SharpnessRect {
        public int top;
        public int bottom;
        public int left;
        public int right;

        public SharpnessRect(int x, int y, int size, int bitmapW, int bitmapH) {
            top = y - size / 2;
            left = x - size / 2;
            bottom = y + size / 2;
            right = x + size / 2;
            if (top < 0 || top > bitmapH) {
                top = 0;
            }
            if (left < 0 || left > bitmapW) {
                left = 0;
            }
            if (bottom > bitmapH) {
                bottom = bitmapH;
            }
            if (right > bitmapW) {
                right = bitmapW;
            }
        }

        public int getWidth() {
            return right - left;
        }

        public int getHeight() {
            return bottom - top;
        }

        @Override
        public String toString() {
            StringBuilder str = new StringBuilder();
            str.append("left:").append(left).append(",top:").append(top).append(",right:").append(right)
                    .append(",bottom:").append(bottom).append(",width:").append(getWidth())
                    .append(",height:").append(getHeight());
            return str.toString();
        }
    }

    public BvirtualDemoView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mGaussianBlur = new GaussianBlur(context);
    }

    public void destroy() {
        if (mGaussianBlur != null) {
            mGaussianBlur.destoryBlur();
        }
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        mOnSingleX = (right - left) / 2;
        mOnSingleY = (bottom - top) / 2;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        drawTrueBgVirtualWithCanvas(canvas);
    }

    public void setPreviewBitmap(Bitmap bitmap) {
        mPreviewBitmap = bitmap;
        if (mPreviewBitmap != null) {
            invalidate();
        }
    }

    private void drawTrueBgVirtualWithCanvas(Canvas canvas) {
        if (mPreviewBitmap != null && mGaussianBlur != null) {
            SharpnessRect inRect = new SharpnessRect(mOnSingleX, mOnSingleY, IN_SHARPNESS_SIZE, mPreviewBitmap.getWidth(), mPreviewBitmap.getHeight());
            Bitmap inBitmap = Bitmap.createBitmap(mPreviewBitmap, inRect.left, inRect.top, inRect.getWidth(), inRect.getHeight());
            SharpnessRect outRect = new SharpnessRect(mOnSingleX, mOnSingleY, OUT_SHARPNESS_SIZE, mPreviewBitmap.getWidth(), mPreviewBitmap.getHeight());
            Bitmap outBitmap = Bitmap.createBitmap(mPreviewBitmap, outRect.left, outRect.top, outRect.getWidth(), outRect.getHeight());
            Bitmap bgBlurBitmap = mGaussianBlur.blurBitmap(mPreviewBitmap, 8);
            Bitmap outBlurBitmap = mGaussianBlur.blurBitmap(outBitmap, 3);
            canvas.drawBitmap(bgBlurBitmap, 0, 0, null);
            canvas.drawBitmap(outBlurBitmap, outRect.left, outRect.top, null);
            canvas.drawBitmap(inBitmap, inRect.left, inRect.top, null);
            mPreviewBitmap.recycle();
            bgBlurBitmap.recycle();
            outBitmap.recycle();
            outBlurBitmap.recycle();
            inBitmap.recycle();
        }

    }
}
