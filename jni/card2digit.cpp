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

	int histogram[256] = {};
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

	svm_node x[131];

	int result = svm_predict_probability(model, x, prob_estimates);

	if (prob_estimates[result] > 0.9) {
		for (int i = 0; i < 11; ++i) {
			if (i != result && prob_estimates[i] > 0.5) {
				return 0;
			}
		}
		return result == 10 ? 'X' : ('0' + result);
	}
	return 0;
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
