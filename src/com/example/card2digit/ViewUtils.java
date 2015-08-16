package com.example.card2digit;

import android.content.Context;
import android.util.TypedValue;

public abstract class ViewUtils {

  public static float convertDpToPixels(float dp, Context context) {
    return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp,
        context.getResources().getDisplayMetrics());
  }
}
