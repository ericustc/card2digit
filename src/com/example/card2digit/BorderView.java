package com.example.card2digit;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.View;

public class BorderView extends View {

  private float boundingBoxWidth = 214f;
  private float boundingBoxHeight = 135f;
  private final int CORNER_SIZE;
  private final int HALF_STROKE_WIDTH;
  private Paint mPaint;
  private Paint mSlimPaint;

  public BorderView(Context context) {
    super(context);
    CORNER_SIZE = Math.round(ViewUtils.convertDpToPixels(32, context));
    HALF_STROKE_WIDTH = Math.round(ViewUtils.convertDpToPixels(2, context));
    mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    mPaint.setColor(Color.GREEN);
    mPaint.setStrokeWidth(HALF_STROKE_WIDTH * 2);
    mSlimPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    mSlimPaint.setColor(Color.GREEN);
    mSlimPaint.setStrokeWidth(1f);
  }

  public float getBoundingBoxWidth() {
    return boundingBoxWidth;
  }

  public float getBoundingBoxHeight() {
    return boundingBoxHeight;
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
    int centerX = getWidth() / 2;
    int centerY = getHeight() / 2;
    if (getWidth() < ViewUtils.convertDpToPixels(324, getContext())) {
      boundingBoxWidth = getWidth()
          - ViewUtils.convertDpToPixels(24, getContext());
    } else {
      boundingBoxWidth = ViewUtils.convertDpToPixels(300, getContext());
    }
    boundingBoxHeight = boundingBoxWidth / 214f * 135f;

    centerX -= boundingBoxWidth / 2;
    centerY -= boundingBoxHeight / 2;

    /**
     * Use drawRect instead of drawLine because the later causes inaccuracies at
     * the pixel level
     */

    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX + CORNER_SIZE, centerY),
        mPaint);
    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX, centerY + CORNER_SIZE),
        mPaint);

    canvas.drawLine(centerX + CameraPreview.left * boundingBoxWidth,
        centerY + CameraPreview.top * boundingBoxHeight,
        centerX + CameraPreview.right * boundingBoxWidth,
        centerY + CameraPreview.top * boundingBoxHeight, mSlimPaint);
    canvas.drawLine(centerX + CameraPreview.left * boundingBoxWidth,
        centerY + CameraPreview.bottom * boundingBoxHeight,
        centerX + CameraPreview.right * boundingBoxWidth,
        centerY + CameraPreview.bottom * boundingBoxHeight, mSlimPaint);
    centerX += boundingBoxWidth;

    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX - CORNER_SIZE, centerY),
        mPaint);
    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX, centerY + CORNER_SIZE),
        mPaint);

    centerX -= boundingBoxWidth;

    centerY += boundingBoxHeight;

    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX + CORNER_SIZE, centerY),
        mPaint);
    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX, centerY - CORNER_SIZE),
        mPaint);

    centerX += boundingBoxWidth;

    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX - CORNER_SIZE, centerY),
        mPaint);
    canvas.drawRect(
        expandLineToRect(centerX, centerY, centerX, centerY - CORNER_SIZE),
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
