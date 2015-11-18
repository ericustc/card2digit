#include <jni.h>
#include <vector>
#include <utility>
#include <string>
#include <android/log.h>
#include "card2digit.h"
#include "mat.h"
#include "svm.h"

using namespace std;

JNIEXPORT jstring JNICALL Java_com_example_card2digit_CameraPreview_ocr(
    JNIEnv *env, jobject obj, jbyteArray data, jint width, jint height) {

  jbyte *pixel = env->GetByteArrayElements(data, 0);

  const int area = width * height;

  mat<int> grayscaleMat(width, height);
  for (int i = 0; i < area; ++i) {
    grayscaleMat[i] = pixel[i] & 0xFF;
  }

  int histogram[256] = { };
  for (int i = 0; i < area; ++i) {
    histogram[pixel[i] & 0xFF]++;
  }

  int threshold = otsu(histogram, area);

  __android_log_print(ANDROID_LOG_VERBOSE, "xxx", "threshold: %d", threshold);

  mat<bool> binaryMat(width, height);

  // The first width*height bytes are from Y Channel, which are what we need

  for (int i = 0; i < area; ++i) {
    binaryMat[i] = (pixel[i] & 0xFF) < threshold; // The Y channel ranges from 0 to 255
  }

  int l = -1;
  int r = -1;
  int t = -1;
  int b = -1;

  /**
   * ocr at most 19 characters, the string is null terminated
   */
  char res[20] = { };
  res[19] = 0;
  int step = 0;
  for (int i = 0; i < width; ++i) {
    bool allWhite = true;
    for (int j = 0; j < height; ++j) {
      if (binaryMat.at(i, j)) {
        allWhite = false;
        r = i;
        if (l == -1) {
          l = i;
          t = b = j;
        } else {
          if (j < t) {
            t = j;
          }
          if (b < j) {
            b = j;
          } else {
            j = b;
          }
        }
      }
    }
    if (allWhite && l != -1) {
      res[step] = recognize(binaryMat, l, r + 1, t, b + 1);
      l = -1;
      ++step;
      if (step == 19) {
        break;
      }
    }
  }
  if (l != -1) {
    res[step] = recognize(binaryMat, l, r + 1, t, b + 1);
    ++step;
  }

  env->ReleaseByteArrayElements(data, pixel, JNI_ABORT);
  return env->NewStringUTF(res);
}

char recognize(mat<bool> &pixel, int l, int r, int t, int b) {

  static svm_model *model = svm_load_model("/sdcard/card2digit.model");

  double prob_estimates[11];

  svm_node input[131];

  int multiplier = b - t;
  const int multiplier_squared = multiplier * multiplier;

  for (int y = 0; y < 13; ++y) {
    for (int x = 0; x < 10; ++x) {
      int sum = 0;
      int base_x = x * multiplier;
      int base_y = y * multiplier;
      for (int i = 0; i < multiplier; ++i) {
        for (int j = 0; j < multiplier; ++j) {
          int ox = (base_x + i) / 13 + l;
          int oy = (base_y + j) / 13 + t;
          if (ox < r && oy < b && pixel.at(ox, oy)) {
            ++sum;
          }
        }
      }
      int index = y * 10 + x;
      input[index].index = index + 1;
      input[index].value = sum >= multiplier_squared ? 1 : 0;
    }
  }
  input[130].index = -1;

  int result = svm_predict_probability(model, input, prob_estimates);

  const int number_of_class = svm_get_nr_class(model);

  int label[number_of_class];

  svm_get_labels(model, label);


  for (int i = 0; i < number_of_class; ++i) {
//  __android_log_print(ANDROID_LOG_VERBOSE, "xxx", "i: %d, label %d, p: %f", i, label[i], prob_estimates[i]);
    if (label[i] == result) {
      if (prob_estimates[i] < 0.5) {
        return '\0';
      }
    } else if (prob_estimates[i] > 0.5) {
      return '\0';
    }
  }
  return '0' + result;
}

int otsu(int histogram[], int total) {
  int sum = 0;
  for (int i = 1; i < 256; ++i)
    sum += i * histogram[i];
  int sumB = 0;
  int wB = 0;
  int wF = 0;
  float mB;
  float mF;
  float max = 0.0;
  float between = 0.0;
  float threshold1 = 0.0;
  float threshold2 = 0.0;
  for (int i = 0; i < 256; ++i) {
    wB += histogram[i];
    if (wB == 0)
      continue;
    wF = total - wB;
    if (wF == 0)
      break;
    sumB += i * histogram[i];
    mB = sumB / wB;
    mF = (sum - sumB) / wF;
    between = wB * wF * (mB - mF) * (mB - mF);
    if (between >= max) {
      threshold1 = i;
      if (between > max) {
        threshold2 = i;
      }
      max = between;
    }
  }
  return (threshold1 + threshold2) / 2.0;
}
