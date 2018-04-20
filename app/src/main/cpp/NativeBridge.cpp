//
// Created by jerro on 4/20/2018.
//

#include <jni.h>
#include <android/log.h>
#include "NativeBridge.h"
#include "opengles/GLRenderer.h"
#include "graphics/GLUtils.h"

#define LOG_TAG "NativeBridge"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

NativeBridge::NativeBridge() {

}

NativeBridge::~NativeBridge() {

}

GLRenderer *glRenderer;

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_nda_jerrol_learnopengles_MainActivity_nativeSurfaceCreated(
            JNIEnv *env,
            jclass type,
            jobject assetManager
    ) {
        GLUtils::setEnvAndAssetManager(env, assetManager);
        if (glRenderer) {
            delete glRenderer;
            glRenderer = NULL;
        }
        glRenderer = new GLRenderer();
        glRenderer->create();
    }

    JNIEXPORT void JNICALL
    Java_com_nda_jerrol_learnopengles_MainActivity_nativeSurfaceChanged(
            JNIEnv *env,
            jclass type,
            int width,
            int height
    ) {
        if (glRenderer != nullptr) {
            glRenderer->change(width, height);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_nda_jerrol_learnopengles_MainActivity_nativeDrawFrame(
            JNIEnv *env,
            jclass type
    ) {
        if (glRenderer != nullptr) {
            glRenderer->draw();
        }
    }

    JNIEXPORT void JNICALL
    Java_com_nda_jerrol_learnopengles_MainActivity_nativeDrawCoin(
            JNIEnv *env,
            jclass type,
            int pieces
    ) {
        LOGI("nativeDrawCoin called");
        if (glRenderer != nullptr) {
            glRenderer->drawCoin(pieces);
        }
    }
}
