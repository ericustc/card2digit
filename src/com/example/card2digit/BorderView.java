package com.example.card2digit;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;

public class BorderView extends View {

  public static final int WIDTH = 214;
  public static final int HEIGHT = 135;

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
    int centerX = getWidth() / 2;
    int centerY = getHeight() / 2;
    centerX -= WIDTH;
    centerY -= HEIGHT;
    canvas.drawLine(centerX, centerY, centerX + 50, centerY, mPaint);
    canvas.drawLine(centerX, centerY, centerX, centerY + 50, mPaint);
    centerX += WIDTH * 2;
    centerY += HEIGHT * 2;
    canvas.drawLine(centerX, centerY, centerX - 50, centerY, mPaint);
    canvas.drawLine(centerX, centerY, centerX, centerY - 50, mPaint);
  }

}
