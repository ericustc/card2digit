package com.example.card2digit;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;

public class BorderView extends View {
  private Paint mPaint;

  private int width = 214;
  private int height = 135;

  public BorderView(Context context) {
    super(context);
    mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    mPaint.setColor(Color.GREEN);
    mPaint.setStrokeWidth(2f);
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
    canvas.drawLine(50, 50, 100, 50, mPaint);
    canvas.drawLine(50, 50, 50, 100, mPaint);
    canvas.drawLine(478, 320, 428, 320, mPaint);
    canvas.drawLine(478, 320, 478, 270, mPaint);
  }

}
