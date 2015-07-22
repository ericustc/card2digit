package com.example.card2digit;

public class MatrixUtils {

  /**
   * 
   * @param matrix
   * @param width
   *          转置前的宽度
   * @param height
   *          转置前的高度
   * @return
   */
  public static byte[] rotate(byte[] matrix, int width, int height) {
    byte[] result = new byte[width * height];
    int row = height - 1;
    int column = 0;
    int cur = row * width;
    int n = width * height;
    for (int i = 0; i < n; ++i) {
      result[i] = matrix[cur];
      --row;
      if (row >= 0) {
        cur -= width;
      } else {
        row = height - 1;
        ++column;
        cur = column + row * width;
      }
    }
    return result;
  }

  public static int[] crop(byte[] data, int width, int l, int r, int t, int b) {
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
    return pixels;
  }
}
