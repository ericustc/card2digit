#include <jni.h>
#include <vector>
#include <utility>
#include <string>
#include <android/log.h>
#include "card2digit.h"
#include "mat.h"

using namespace std;

float compare(char glyph[], mat &pixel, int l, int r, int t, int b, float ratio);

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

  // get bounding box height
  int boundingBoxTop = -1;
  int boundingBoxBottom = -1;
  for (int i = 0; i < roiHeight; ++i) {
	  for (int j = 0; j < roiWidth; ++j) {
		  if (area.at(j, i)) {
			  if (boundingBoxTop == -1) {
				  boundingBoxTop = i;
			  }
			  boundingBoxBottom = i;
			  break;
		  }
	  }
  }
  if (boundingBoxTop == -1) return 0;
  float glyphBoundingBoxHeight = 13.0f;
  float ratio = (boundingBoxBottom - boundingBoxTop + 1) / glyphBoundingBoxHeight;

  char glyphZero[] = {
		  0x3C,
		  0x7E,
		  0xC3,
		  0xC3,
		  0xC3,
		  0xC3,
		  0xC3,
		  0xC3,
		  0xC3,
		  0xC3,
		  0xC3,
		  0x7E,
		  0x3C};

  int l = -1;
  int r = -1;
  int t = -1;
  int b = -1;

  for (int i = 0; i < roiWidth; ++i) {
	  bool allWhite = true;
  	  for (int j = 0; j < roiHeight; ++j) {
  		  if (area.at(i, j)) {
  			  allWhite = false;
  			  r = i;
  			  if (l == -1) {
  				  l = t = b = i;
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
  	  if (allWhite && l != -1) break;
  }

  __android_log_print(ANDROID_LOG_VERBOSE, "xxx", "roi w*h: %d*%d, %d %d %d %d", roiWidth, roiHeight, l, r, t, b);


  float grade = compare(glyphZero, area, l, r + 1, t, b + 1, ratio);
  __android_log_print(ANDROID_LOG_VERBOSE, "xxx", "grade: %f", grade);


  for (int i = 0; i < roiHeight; ++i) {
	  string s;
	  for (int j = 0; j < 50; ++j) {
		  s += area.at(j, i) ? "0" : "-";
	  }
	  __android_log_print(ANDROID_LOG_VERBOSE, "xxx", "%s", s.c_str());
  }


  env->ReleaseByteArrayElements(data, pixel, JNI_ABORT);
  return 0;
}

float compare(char glyph[], mat &pixel, int l, int r, int t, int b, float ratio) {
	int hit = 0;
	for (int i = t; i < b; ++i) {
		for (int j = l; j < r; ++j) {
			bool flag = (glyph[(int)((i - t) / ratio)] & (1 << (7 - (int)((j - l) / ratio))));
			if (!(pixel.at(j, i) ^ flag)) {
				++hit;
			}
		}
	}
	__android_log_print(ANDROID_LOG_VERBOSE, "xxx", "hit: %d", hit);
	return hit / ((float) (r - l) * (b - t));
}
