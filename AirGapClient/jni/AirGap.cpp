#include <jni.h>

extern "C"
{
	JNIEXPORT jstring JNICALL Java_de_nachregenkommtsonne_airgap_MainActivity_messageFromNativeCode (JNIEnv * env, jobject jObj);
}

JNIEXPORT jstring JNICALL Java_de_nachregenkommtsonne_airgap_MainActivity_messageFromNativeCode (JNIEnv * env, jobject jObj)
{
	return env->NewStringUTF("Hello World!");
}
