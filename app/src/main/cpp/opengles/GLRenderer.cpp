//
// Created by jerro on 4/20/2018.
//

#include <android/log.h>
#include <android/legacy_stdlib_inlines.h>
#include <stdlib.h>
#include "GLRenderer.h"
#include "../graphics/GLUtils.h"

#define LOG_TAG "GLRenderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

const char *VERTEXT_SHADER_IMAGE = "uniform mat4 uMVPMatrix; \n"
        "attribute vec4 vPosition; \n"
        "attribute vec2 a_texCoord; \n"
        "varying vec2 v_texCoord; \n"
        "void main() { \n"
        "    gl_Position = uMVPMatrix * vPosition; \n"
        "    v_texCoord = a_texCoord; \n"
        "}";

const char *FRAGMENT_SHADER_IMAGE = "precision mediump float; \n"
        "varying vec2 v_texCoord; \n"
        "uniform sampler2D s_texture; \n"
        "void main() { \n"
        "    gl_FragColor = texture2D( s_texture, v_texCoord ); \n"
        "}";

GLRenderer::GLRenderer() {
    // The constructor
    mMVPMatrix = NULL;
    mProjectionMatrix = NULL;
    mViewMatrix = NULL;
}

GLRenderer::~GLRenderer() {
    // The destructor
    delete mMVPMatrix;
    mMVPMatrix = NULL;
    delete mProjectionMatrix;
    mProjectionMatrix = NULL;
    delete mViewMatrix;
    mViewMatrix = NULL;
}

void GLRenderer::create() {
    LOGD("create");
    setupScaling(0, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    imageProgramHandle = GLUtils::createProgram(&VERTEXT_SHADER_IMAGE, &FRAGMENT_SHADER_IMAGE);
    if (!imageProgramHandle) {
        LOGD("Could not create a program.");
    }

    // Get handle to vertex shader's vPosition number
    mPositionHandle = (GLuint) glGetAttribLocation(imageProgramHandle, "vPosition");
    // Get handle to texture coordinates location
    mTexCoord = (GLuint) glGetAttribLocation(imageProgramHandle, "a_texCoord");
    // Get handle to shape's transformation matrix
    mMatrixHandle = (GLuint) glGetUniformLocation(imageProgramHandle, "uMVPMatrix");
    // Get handle to textures locations
    mSamplerLoc = (GLuint) glGetUniformLocation(imageProgramHandle, "s_texture");

    mMVPMatrix = new Matrix();

    mClearSurface = true;

    glUseProgram(imageProgramHandle);


    // createTextures
    //textures[0] = GLUtils::loadTexture("coin/coin_1.png");
    textures[0] = GLUtils::loadTexture("bg.jpg");
    textures[1] = GLUtils::loadTexture("coin/coin_2.png");
    textures[2] = GLUtils::loadTexture("coin/coin_3.png");
    textures[3] = GLUtils::loadTexture("coin/coin_4.png");
    textures[4] = GLUtils::loadTexture("coin/coin_5.png");
    textures[5] = GLUtils::loadTexture("coin/coin_6.png");
    textures[6] = GLUtils::loadTexture("coin/coin_7.png");
    textures[7] = GLUtils::loadTexture("coin/coin_8.png");

    TextureGenerator* textureGenerator = new TextureGenerator();
    textureGenerator->setSize(320.0f, 568.0f);
    textureGenerator->setTranslate(320.0f / 2.0f, 568.0f / 2.0f);
    textureGenerator->setIndex(INDEX_BG);
    textureGenerator->setTextureId(textures[0]);
    mTextureCollection.push_back(textureGenerator);
}

void GLRenderer::change(int width, int height) {
//    screenWidth = width;
//    screenHeight = height;

    glViewport(0, 0, width, height);

    // Create a new perspective projection matrix. The height will stay the same
    // while the width will vary as per aspect ratio.
    float left = FIXED_WIDTH;
    float right = 0.0f;
    float bottom = FIXED_HEIGHT;
    float top = 0.0f;
    float near = 1.0f;
    float far = 50.0f;

    mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 1.0f;

    // We are looking at the origin
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;

    // Set our up vector.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Set the view matrix.
    mViewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

    // model * view * projection
    mMVPMatrix->multiply(*mProjectionMatrix, *mViewMatrix);

    setupScaling(width, height);
}

void GLRenderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    dirtFrame();
    //glDisable(GL_BLEND);
}

void GLRenderer::dirtFrame() {
    /*if (!mClearSurface) {
        //long slowTime = GLUtils::currentTimeMillis() % 100000L;
        // int elapse = (int) ((0.01f) * (slowTime));
        int elapse = GLUtils::getElapseRealtime();

//  LOGI("getElapseReal Time: %d", GLUtils::getElapseRealtime());
//  LOGI("elapse Time: %d", elapse);

        for (unsigned i = 0; i < mTextureCollection.size(); i++) {

            TextureGenerator* prevCoin = mTextureCollection.at(i);
            if (mCurrentTime < elapse) {
                prevCoin->setTextureId(prevCoin->getTextureId());
                prevCoin->translate(0.0f, 10.0f);
            }
            prevCoin->render(prevCoin->getTextureId(), mMVPMatrix, mPositionHandle, mTexCoord, mMatrixHandle, mSamplerLoc);
        }
        *//*if (!hasVisibleCoin()) {
            LOGD("All coins y < 0");
            clearSurface(true);
        }*//*

        mCurrentTime = elapse;
    }*/
    for (unsigned i = 0; i < mTextureCollection.size(); i++) {
        TextureGenerator* texture = mTextureCollection.at(i);

        // if (i == 0) continue;

        if (texture->getIndex() == INDEX_COIN) {
            texture->translate(0.0f, 10.0f);

            if (texture->getTranslateY() > 410.0f) {
                // mTextureCollection.pop_back();
                mTextureCollection.erase(mTextureCollection.begin() + i);
            }
        }

        texture->render(textures[0], mMVPMatrix, mPositionHandle, mTexCoord, mMatrixHandle, mSamplerLoc);
    }
}

void GLRenderer::drawCoin(int pieces) {
    LOGI("drawNewCoin called");
    // LOGI()("Size of textures %d", (int) sizeof(textures));
    LOGI("screenWidth %d", screenWidth);
    LOGI("screenHeight %d", screenHeight);
    LOGI("FIXED_WIDTH %d", FIXED_WIDTH);
    LOGI("FIXED_HEIGHT %d", FIXED_HEIGHT);
    for (int i = 0; i < pieces; i++) {
        GLuint r = GLuint (i % TEXTURE_SIZE);
        LOGI("r = %d", r);
        float pointX = rand() % (int)(((FIXED_WIDTH - 48) - 48) + 1) + 48;
        //float pointX = rand() % (FIXED_WIDTH) + 48;;
        LOGI("PointX %.2f", pointX);
        float pointY = rand() % 310 + 1;
        LOGI("PointY %.2f", pointY);
        // Coin* coin = new Coin(r, pointX, pointY + screenHeight);
        //Coin* coin = new Coin(r, pointX - 50.0f, pointY + (screenHeight / 2)); // Same all height
        TextureGenerator* coin = new TextureGenerator(); // Same all height
        coin->setSize(40.0f, 40.0f);
        coin->setTranslate(pointX, pointY);
        coin->setTextureId(textures[0]);
        coin->setIndex(INDEX_COIN);
        mTextureCollection.push_back(coin);
        // LOGI("drawNewCoin %d", i);
    }
    LOGI("mCoinCollectionSize = %d", (int) mTextureCollection.size());
    LOGI("drawNewCoin end");
}

void GLRenderer::setupScaling(int width, int height) {
// The screen resolution
    widthPixel = width;
    heightPixel = height;

    // Orientation is assumed portrait
    //scaleX = widthPixel / 320.0f;
    //scaleY = heightPixel / 480.0f;
    scaleX = widthPixel / FIXED_WIDTH;
    scaleY = heightPixel / FIXED_HEIGHT;

    // Get our uniform scalier
    if (scaleX > scaleY)
        scaleValue = scaleY;
    else
        scaleValue = scaleX;

    LOGD("scaleValue %.2f", scaleValue);
}
