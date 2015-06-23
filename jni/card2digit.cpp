#include <jni.h>
#include <bitset>
#include "card2digit.h"
#include <android/log.h>

using namespace std;

JNIEXPORT jstring JNICALL Java_com_example_card2digit_CameraPreview_ocr
  (JNIEnv *env, jobject obj, jbyteArray data, jint width, jint height, jint left, jint right, jint top, jint bottom) {
  bitset<500> area;
  __android_log_print(ANDROID_LOG_VERBOSE, "xxx", "left: %d, right: %d", left, right);
  jbyte *a = env->GetByteArrayElements(data, NULL);
  int row = top;
  int column = left;
  int cur = width * top + left;
  for (int i = 0; i < 5; ++i) {
	  area.set(i, a[cur] > 0 ? true : false);
	  ++column;
	  if (column == right) {
		  column = left;
		  ++row;
		  cur = width * row + column;
	  } else {
		  ++cur;
	  }
  }
  env->ReleaseByteArrayElements(data, a, JNI_ABORT);
  return 0;
}

