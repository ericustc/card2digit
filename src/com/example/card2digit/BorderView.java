package com.example.card2digit;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;

public class BorderView extends View {

  public static final float WIDTH = 214 * 1.5f;
  public static final float HEIGHT = 135 * 1.5f;

  private Paint mPaint;

  public BorderView(Context context) {
    super(context);
    mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    mPaint.setColor(Color.GREEN);
    mPaint.setStrokeWidth(2f);
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
    float centerX = getWidth() / 2;
    float centerY = getHeight() / 2;
    centerX -= WIDTH;
    centerY -= HEIGHT;
    canvas.drawLine(centerX, centerY, centerX + 50, centerY, mPaint);
    canvas.drawLine(centerX, centerY, centerX, centerY + 50, mPaint);
    canvas.drawLine(centerX + CameraPreview.left * BorderView.WIDTH * 2,
        centerY + CameraPreview.top * BorderView.HEIGHT * 2, getWidth(),
        centerY + CameraPreview.top * BorderView.HEIGHT * 2, mPaint);
    canvas.drawLine(centerX + CameraPreview.right * BorderView.WIDTH * 2,
        centerY + CameraPreview.bottom * BorderView.HEIGHT * 2, 0, centerY
            + CameraPreview.bottom * BorderView.HEIGHT * 2, mPaint);
    centerX += WIDTH * 2;
    centerY += HEIGHT * 2;
    canvas.drawLine(centerX, centerY, centerX - 50, centerY, mPaint);
    canvas.drawLine(centerX, centerY, centerX, centerY - 50, mPaint);
  }

}
