package com.bvirtual;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import com.azmohan.bvarithmetic.R;


/**
 * Created by azmohan on 16-9-1.
 */
public class BvirtualDemoView extends View {
    private Bitmap mPreviewBitmap;
    private int mOnSingleX;
    private int mOnSingleY;
    private GaussianBlur mGaussianBlur;
    private final static int IN_SHARPNESS_RADIUS = 150;
    private final static int OUT_SHARPNESS_RADIUS = 250;
    private final static int FULL_RADIUS = 300;
    private Bitmap mIndicator;
    private boolean mIsPressed;

    public static class SharpnessRect {
        public int top;
        public int bottom;
        public int left;
        public int right;

        public SharpnessRect(int x, int y, int r, int bitmapW, int bitmapH) {
            top = y - r;
            left = x - r;
            bottom = y + r;
            right = x + r;
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
        BitmapDrawable drawable = (BitmapDrawable) context.getResources().getDrawable(R.drawable.lijiang);
        mPreviewBitmap = drawable.getBitmap();
        BitmapDrawable indicatorDrawable = (BitmapDrawable) context.getResources().getDrawable(R.drawable.freeme_bvirtual_indicator);
        mIndicator = indicatorDrawable.getBitmap();
    }


    public void destroy() {
        if (mGaussianBlur != null) {
            mGaussianBlur.destoryBlur();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        int action = ev.getAction();
        mOnSingleX = (int) ev.getX();
        mOnSingleY = (int) ev.getY();
        if (action == MotionEvent.ACTION_DOWN) {
            mIsPressed = true;
        } else if (action == MotionEvent.ACTION_UP) {
            mIsPressed = false;
        } else if (action == MotionEvent.ACTION_MOVE) {
        }
        this.invalidate();
        return true;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        this.setMeasuredDimension(mPreviewBitmap.getWidth(), mPreviewBitmap.getHeight());
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
        if (mIsPressed) {
            canvas.drawBitmap(mIndicator, mOnSingleX - mIndicator.getWidth() / 2, mOnSingleY - mIndicator.getHeight() / 2, null);
        }
    }

    public void setPreviewBitmap(Bitmap bitmap) {
        mPreviewBitmap = bitmap;
        if (mPreviewBitmap != null) {
            invalidate();
        }
    }

    private void drawTrueBgVirtualWithCanvas(Canvas canvas) {
        long time = System.currentTimeMillis();
        if (mPreviewBitmap != null && mGaussianBlur != null) {
            Bitmap bgBlurBitmap = mGaussianBlur.blurBitmap(mPreviewBitmap, 8);
            BlurInfo info = new BlurInfo();
            info.x = mOnSingleX;
            info.y = mOnSingleY;
            info.inRadius = IN_SHARPNESS_RADIUS;
            info.outRadius = OUT_SHARPNESS_RADIUS;
            SmoothBlurJni.smoothRender(bgBlurBitmap, mPreviewBitmap, info);
            canvas.drawBitmap(bgBlurBitmap, 0, 0, null);
            bgBlurBitmap.recycle();
        }
        Log.e("SmoothBlur", "drawTrueBgVirtualWithCanvas : " + (System.currentTimeMillis() - time) + " ms");
    }
}
