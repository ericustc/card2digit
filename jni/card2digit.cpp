#include <jni.h>
#include <vector>
#include <utility>
#include <string>
#include <android/log.h>
#include "card2digit.h"
#include "mat.h"

using namespace std;

JNIEXPORT jstring JNICALL Java_com_example_card2digit_CameraPreview_ocr
  (JNIEnv *env, jobject obj, jbyteArray data, jint width, jint height, jint left, jint right, jint top, jint bottom) {
  // ROI: region of interest, where the alphanumeric text is located
  int roiWidth = right - left;
  int roiHeight = bottom - top;
  mat area(roiWidth, roiHeight);
  int row = top;
  int column = left;
  int cur = width * top + left;
  jbyte *pixel = env->GetByteArrayElements(data, 0);
  // The first width*height bytes are from Y Channel, which are what we need
  for (int i = 0; i < area.size(); ++i) {
	  area[i] = (pixel[cur] & 0xFF) < 64; // The Y channel ranges from 0 to 255. We use 64 as the threshold
	  ++column;
	  if (column == right) {
		  column = left;
		  ++row;
		  cur = width * row + column;
	  } else {
		  ++cur;
	  }
  }
  for (int row = 0; row < roiHeight; ++row) {
	  string s;
	  __android_log_print(ANDROID_LOG_VERBOSE, "xxx", "%s", s.c_str());
  }
  env->ReleaseByteArrayElements(data, pixel, JNI_ABORT);
  return 0;
}
