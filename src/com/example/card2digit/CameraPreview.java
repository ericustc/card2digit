package com.example.card2digit;

import java.io.IOException;
import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.Toast;

public class CameraPreview extends FrameLayout implements
    SurfaceHolder.Callback, PreviewCallback {

  static {
    System.loadLibrary("card2digit");
  }

  private SurfaceView mSurfaceView;
  private SurfaceHolder mHolder;
  private Size mPreviewSize;
  private List<Size> mSupportedPreviewSizes;
  private Camera mCamera;

  CameraPreview(Context context) {
    super(context);
    mSurfaceView = new SurfaceView(context);
    addView(mSurfaceView);
    BorderView borderView = new BorderView(context);
    addView(borderView);
    borderView.setOnClickListener(new OnClickListener() {

      @Override
      public void onClick(View v) {
        take = true;
      }
    });

    mHolder = mSurfaceView.getHolder();
    mHolder.addCallback(this);
    mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
  }

  public void setCamera(Camera camera) {
    mCamera = camera;
    if (mCamera != null) {
      mSupportedPreviewSizes = mCamera.getParameters()
          .getSupportedPreviewSizes();
      requestLayout();
    }
  }

  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    final int width = resolveSize(getSuggestedMinimumWidth(), widthMeasureSpec);
    final int height = resolveSize(getSuggestedMinimumHeight(),
        heightMeasureSpec);
    setMeasuredDimension(width, height);
    if (mSupportedPreviewSizes != null) {
      mPreviewSize = getOptimalPreviewSize(mSupportedPreviewSizes, width,
          height);
    }
  }

  @Override
  protected void onLayout(boolean changed, int l, int t, int r, int b) {
    if (changed && getChildCount() > 0) {

      final View child = getChildAt(0);

      final int width = r - l;
      final int height = b - t;

      int previewWidth = width;
      int previewHeight = height;

      getChildAt(1).layout(0, 0, width, height);

      if (mPreviewSize != null) {
        previewWidth = mPreviewSize.width;
        previewHeight = mPreviewSize.height;
      }

      if (width * previewHeight > height * previewWidth) {
        final int scaledChildWidth = previewWidth * height / previewHeight;
        child.layout((width - scaledChildWidth) / 2, 0,
            (width + scaledChildWidth) / 2, height);
      } else {
        final int scaledChildHeight = previewHeight * width / previewWidth;
        child.layout(0, (height - scaledChildHeight) / 2, width,
            (height + scaledChildHeight) / 2);
      }
    }
  }

  @Override
  public void surfaceCreated(SurfaceHolder holder) {
    try {
      if (mCamera != null) {
        mCamera.setPreviewDisplay(holder);
        mCamera.setPreviewCallback(this);
      }
    } catch (IOException exception) {
      Log.e("xxx", "IOException caused by setPreviewDisplay()", exception);
    }
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder) {
    if (mCamera != null) {
      mCamera.stopPreview();
    }
  }

  private Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
    final double ASPECT_TOLERANCE = 0.1;
    double targetRatio = (double) w / h;
    if (sizes == null) {
      return null;
    }

    Size optimalSize = null;
    double minDiff = Double.MAX_VALUE;

    int targetHeight = h;

    // Try to find an size match aspect ratio and size
    for (Size size : sizes) {
      double ratio = (double) size.width / size.height;
      if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) {
        continue;
      }
      if (Math.abs(size.height - targetHeight) < minDiff) {
        optimalSize = size;
        minDiff = Math.abs(size.height - targetHeight);
      }
    }

    // Cannot find the one match the aspect ratio, ignore the requirement
    if (optimalSize == null) {
      minDiff = Double.MAX_VALUE;
      for (Size size : sizes) {
        if (Math.abs(size.height - targetHeight) < minDiff) {
          optimalSize = size;
          minDiff = Math.abs(size.height - targetHeight);
        }
      }
    }
    return optimalSize;
  }

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    Camera.Parameters parameters = mCamera.getParameters();
    parameters.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
    parameters.setFocusMode(Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
    // parameters.setFlashMode(Parameters.FLASH_MODE_TORCH);
    requestLayout();
    mCamera.setParameters(parameters);
    mCamera.startPreview();
  }

  private boolean take;

  @Override
  public void onPreviewFrame(byte[] data, Camera camera) {
    if (take) {
      Log.d("xxx", "length: " + data.length);
      int width = mPreviewSize.width;
      int height = mPreviewSize.height;
      Log.d("xxx", "width: " + width);
      Log.d("xxx", "height: " + height);

      int l = Math.round((left * BorderView.WIDTH * 2 + mSurfaceView.getWidth()
          / 2 - BorderView.WIDTH)
          / mSurfaceView.getWidth() * width);
      int r = Math.round((right * BorderView.WIDTH * 2
          + mSurfaceView.getWidth() / 2 - BorderView.WIDTH)
          / mSurfaceView.getWidth() * width);
      int t = Math.round((top * BorderView.HEIGHT * 2
          + mSurfaceView.getHeight() / 2 - BorderView.HEIGHT)
          / mSurfaceView.getHeight() * height);
      int b = Math.round((bottom * BorderView.HEIGHT * 2
          + mSurfaceView.getHeight() / 2 - BorderView.HEIGHT)
          / mSurfaceView.getHeight() * height);

      int[] pixels = new int[(r - l) * (b - t)];
      int row = t;
      int column = l;
      int cur = width * t + l;
      for (int i = 0; i < pixels.length; ++i) {
        int p = data[cur] & 0xFF;
        pixels[i] = 0xff000000 | p << 16 | p << 8 | p;
        ++column;
        if (column == r) {
          column = l;
          ++row;
          cur = width * row + column;
        } else {
          ++cur;
        }
      }
      Bitmap bm = Bitmap.createBitmap(pixels, r - l, b - t,
          Bitmap.Config.ARGB_8888);
      ImageView iv = new ImageView(getContext());
      iv.setImageBitmap(bm);
      Toast toast = new Toast(getContext());
      toast.setView(iv);
      toast.setDuration(Toast.LENGTH_LONG);
      toast.show();

      String result = ocr(data, width, height, l, r, t, b);
      take = false;
    }
  }

  public static float left = 28f / 85.6f;
  public static float right = 76f / 85.6f;
  public static float top = 44f / 54f;
  public static float bottom = 49f / 54f;

  private native String ocr(byte[] data, int width, int height, int l, int r,
      int t, int b);

}
