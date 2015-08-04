package com.example.card2digit;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.View;

public class BorderView extends View {

  public static final float WIDTH = 214f;
  public static final float HEIGHT = 135f;
  private static final int HALF_STROKE_WIDTH = 4;
  private Paint mPaint;
  private Paint mSlimPaint;

  public BorderView(Context context) {
    super(context);
    mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    mPaint.setColor(Color.GREEN);
    mPaint.setStrokeWidth(HALF_STROKE_WIDTH * 2);
    mSlimPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    mSlimPaint.setColor(Color.GREEN);
    mSlimPaint.setStrokeWidth(1f);
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
    int centerX = getWidth() / 2;
    int centerY = getHeight() / 2;
    centerX -= WIDTH;
    centerY -= HEIGHT;

    centerY = 72;

    /**
     * Use drawRect instead of drawLine because the later causes inaccuracies at
     * the pixel level
     */

    canvas.drawRect(expandLineToRect(centerX, centerY, centerX + 50, centerY),
        mPaint);
    canvas.drawRect(expandLineToRect(centerX, centerY, centerX, centerY + 50),
        mPaint);

    canvas.drawLine(centerX + CameraPreview.left * BorderView.WIDTH * 2,
        centerY + CameraPreview.top * BorderView.HEIGHT * 2,
        centerX + CameraPreview.right * BorderView.WIDTH * 2,
        centerY + CameraPreview.top * BorderView.HEIGHT * 2, mSlimPaint);
    canvas.drawLine(centerX + CameraPreview.left * BorderView.WIDTH * 2,
        centerY + CameraPreview.bottom * BorderView.HEIGHT * 2,
        centerX + CameraPreview.right * BorderView.WIDTH * 2,
        centerY + CameraPreview.bottom * BorderView.HEIGHT * 2, mSlimPaint);
    centerX += WIDTH * 2;

    canvas.drawRect(expandLineToRect(centerX, centerY, centerX - 50, centerY),
        mPaint);
    canvas.drawRect(expandLineToRect(centerX, centerY, centerX, centerY + 50),
        mPaint);

    centerX -= WIDTH * 2;

    centerY += HEIGHT * 2;

    canvas.drawRect(expandLineToRect(centerX, centerY, centerX + 50, centerY),
        mPaint);
    canvas.drawRect(expandLineToRect(centerX, centerY, centerX, centerY - 50),
        mPaint);

    centerX += WIDTH * 2;

    canvas.drawRect(expandLineToRect(centerX, centerY, centerX - 50, centerY),
        mPaint);
    canvas.drawRect(expandLineToRect(centerX, centerY, centerX, centerY - 50),
        mPaint);
  }

  private Rect expandLineToRect(int x1, int y1, int x2, int y2) {
    Rect r = new Rect();

    r.left = Math.min(x1, x2) - HALF_STROKE_WIDTH;
    r.right = Math.max(x1, x2) + HALF_STROKE_WIDTH;

    r.top = Math.min(y1, y2) - HALF_STROKE_WIDTH;
    r.bottom = Math.max(y1, y2) + HALF_STROKE_WIDTH;

    return r;
  }

}
