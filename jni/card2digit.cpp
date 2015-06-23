#include <jni.h>
#include <bitset>
#include "card2digit.h"

using namespace std;

JNIEXPORT jstring JNICALL Java_com_example_card2digit_CameraPreview_ocr
  (JNIEnv *env, jobject obj, jbyteArray data, jint width, jint height, jint left, jint right, jint top, jint bottom) {
  bitset<30> area;
  jboolean isCopy;
  jbyte *a = env->GetByteArrayElements(data, &isCopy);
  env->ReleaseByteArrayElements(data, a, JNI_ABORT);
  return env->NewStringUTF("");
}

