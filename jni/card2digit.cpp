#include <jni.h>
#include <vector>
#include <utility>
#include <string>
#include <android/log.h>
#include "card2digit.h"
#include "mat.h"

using namespace std;

char recognize(mat &pixel, int l, int r, int t, int b);
float compare(unsigned short glyph[], mat &pixel, int l, int r, int t, int b,
		float ratio);

JNIEXPORT jstring JNICALL Java_com_example_card2digit_CameraPreview_ocr(
		JNIEnv *env, jobject obj, jbyteArray data, jint width, jint height,
		jint left, jint right, jint top, jint bottom) {
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
	if (boundingBoxTop == -1) {
		env->ReleaseByteArrayElements(data, pixel, JNI_ABORT);
		return 0;
	}

	int l = -1;
	int r = -1;
	int t = -1;
	int b = -1;

	char res[19] = { };
	res[18] = 0;
	int step = 0;
	for (int i = 0; i < roiWidth; ++i) {
		bool allWhite = true;
		for (int j = 0; j < roiHeight; ++j) {
			if (area.at(i, j)) {
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
			res[step] = recognize(area, l, r + 1, t, b + 1);
			l = -1;
			++step;
			if (step == 18) {
				break;
			}
		}
	}
	if (l != -1) {
		res[step] = recognize(area, l, r + 1, t, b + 1);
		++step;
	}

	for (int i = 0; i < roiHeight; ++i) {
		string s;
		for (int j = 0; j < 50; ++j) {
			s += area.at(j, i) ? "0" : "-";
		}
		__android_log_print(ANDROID_LOG_VERBOSE, "xxx", "%s", s.c_str());
	}

	env->ReleaseByteArrayElements(data, pixel, JNI_ABORT);
	return env->NewStringUTF(res);
}

char recognize(mat &pixel, int l, int r, int t, int b) {
	static unsigned short glyph[11][13] = { { 0x3C, 0x7E, 0xC3, 0xC3, 0xC3,
			0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x7E, 0x3C }, // 0

			{ 0x0C, 0x3C, 0x7C, 0xEC, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
					0x0C, 0x0C }, // 1

			{ 0x3C, 0xFE, 0x43, 0x03, 0x03, 0x06, 0x0E, 0x38, 0x70, 0x60, 0xC0,
					0xFF, 0xFF }, // 2

			{ 0xFF, 0xFF, 0x06, 0x0C, 0x18, 0x3C, 0x0E, 0x03, 0x03, 0x03, 0x07,
					0xFE, 0xFC }, // 3

			{ 0x08, 0x18, 0x18, 0x30, 0x30, 0x64, 0x66, 0xC6, 0xFF, 0xFF, 0x06,
					0x06, 0x06 }, // 4

			{ 0xFE, 0xFE, 0xC0, 0xC0, 0xF0, 0xFC, 0x0C, 0x06, 0x06, 0x06, 0x0C,
					0xF8, 0xF0 }, // 5

			{ 0x04, 0x0C, 0x18, 0x30, 0x70, 0x7C, 0xFE, 0xC3, 0xC3, 0xC3, 0xC3,
					0x7E, 0x3C }, // 6

			{ 0xFF, 0xFF, 0x03, 0x03, 0x06, 0x0C, 0x18, 0x18, 0x30, 0x30, 0x30,
					0x30, 0x30 }, // 7

			{ 0x1C00, 0x7E00, 0x6300, 0x6300, 0x7600, 0x3C00, 0x3E00, 0x6300,
					0xC180, 0xC180, 0xC380, 0x7F00, 0x3E00 }, // 8

			{ 0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xE3, 0x7E, 0x3E, 0x0C, 0x0C,
					0x18, 0x30 }, // 9

			{ 0xC3, 0x43, 0x66, 0x34, 0x3C, 0x18, 0x18, 0x3C, 0x26, 0x66, 0xC3,
					0xC3 } // X

	};

	float ratio = (b - t) / 13.0f;

	float grade = 0.0f;
	int max = -1;
	__android_log_print(ANDROID_LOG_VERBOSE, "xxx", "%d, %d, %d, %d, %f", l, r,
			t, b, ratio);
	for (int i = 0; i < 11; ++i) {
		float g = compare(glyph[i], pixel, l, r, t, b, ratio);
		__android_log_print(ANDROID_LOG_VERBOSE, "xxx", "%d, grade: %f", i, g);
		if (g > grade) {
			grade = g;
			max = i;
		}
	}
	return max == 10 ? 'X' : '0' + max;
}

float compare(unsigned short glyph[], mat &pixel, int l, int r, int t, int b,
		float ratio) {
	int shift = glyph[0] > 0xFF ? 16 : 8;
	int hit = 0;
	for (int i = t; i < b; ++i) {
		string s;
		for (int j = l; j < r; ++j) {
			int x = (int) ((j - l) / ratio);
			bool flag =
					x < shift
							&& (glyph[(int) ((i - t) / ratio)]
									& (1 << (shift - 1 - x)));
			s += flag ? "0" : "-";
			if (!(pixel.at(j, i) ^ flag)) {
				++hit;
			}
		}
		__android_log_print(ANDROID_LOG_VERBOSE, "xxx", "%s", s.c_str());
	}
	return hit / ((float) (r - l) * (b - t));
}
