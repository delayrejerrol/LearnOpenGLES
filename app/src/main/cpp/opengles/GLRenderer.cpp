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
    textures[0] = GLUtils::loadTexture("coin/coin_1.png");
    //textures[0] = GLUtils::loadTexture("bg.jpg");
    textures[1] = GLUtils::loadTexture("coin/coin_2.png");
    textures[2] = GLUtils::loadTexture("coin/coin_3.png");
    textures[3] = GLUtils::loadTexture("coin/coin_4.png");
    textures[4] = GLUtils::loadTexture("coin/coin_5.png");
    textures[5] = GLUtils::loadTexture("coin/coin_6.png");
    textures[6] = GLUtils::loadTexture("coin/coin_7.png");
    textures[7] = GLUtils::loadTexture("coin/coin_8.png");

    TextureGenerator* textureGenerator = new TextureGenerator();
    textureGenerator->setSize(1, 1); // Update size in change();
    textureGenerator->setTranslate(-1.0f, 1.0f);
    //textureGenerator->setTranslate(0.0f, 0.0f);
    textureGenerator->setIndex(INDEX_BG);
    textureGenerator->setTextureId(GLUtils::loadTexture("bg.png"));
    mTextureCollection.push_back(textureGenerator);

    /*textureGenerator = new TextureGenerator();
    textureGenerator->setSize(1024.0f, 150.0f);
    textureGenerator->setTranslate(0.0f, 30.0f);
    textureGenerator->hideX();
    textureGenerator->setIndex(INDEX_CLOUD_FIRST);
    textureGenerator->setTextureId(GLUtils::loadTexture("clouds.png"));
    mTextureCollection.push_back(textureGenerator);

    textureGenerator = new TextureGenerator();
    textureGenerator->setSize(1024.0f, 150.0f);
    textureGenerator->setTranslate(0.0f, 30.0f);
    textureGenerator->hideX();
    textureGenerator->setIndex(INDEX_CLOUD_SECOND);
    textureGenerator->setTextureId(GLUtils::loadTexture("clouds.png"));
    mTextureCollection.push_back(textureGenerator);*/

    textureGenerator = new TextureGenerator();
    textureGenerator->setSize(80, 80);
    textureGenerator->setTranslate(0.0f, 30.0f);
    textureGenerator->setIndex(INDEX_COIN);
    textureGenerator->setTextureId(GLUtils::loadTexture("coin/coin_1.png"));
    mTextureCollection.push_back(textureGenerator);

    mSpriteChicken = new TextureGenerator();
    // mSpriteChicken->setTextureId(GLUtils::loadTexture("chicken.png"));

    isUseFirstCloud = true;
    isUseSecondCloud = false;

}

void GLRenderer::change(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    glViewport(0, 0, width, height);

    // Create a new perspective projection matrix. The height will stay the same
    // while the width will vary as per aspect ratio.
    if (width > height) {
        mAspectRatio = (float)width / (float)height;
    } else {
        mAspectRatio = (float)height / (float)width;
    }
    // mAspectRatio = (float)width / (float)height;
    LOGI("Aspect Ratio: %.2f", mAspectRatio);

    /*float left = screenWidth;
    float right = 0.0f;
    float bottom = screenHeight;
    float top = 0.0f;
    float near = 1.0f;
    float far = 50.0f;*/

    float left = -mAspectRatio;
    float right = mAspectRatio;
    float bottom = -1.0f;
    float top = 1.0f;
    float near = 3.0f;
    float far = 7.0f;

    // mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);
    mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);

    /*// Position the eye in front of the origin.
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
    float upZ = 0.0f;*/

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = -3.0f;

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

    /*for (int i = 0; i < mTextureCollection.size(); i++) {
        if (mTextureCollection[i]->getIndex() == INDEX_BG) {
            mTextureCollection[i]->setSize((int)screenWidth, (int)screenHeight);
            break;
        }
    }*/

    /*mSpriteChicken->setSize(80 * mAspectRatio, 80 *mAspectRatio);
    mSpriteChicken->setMaxFrames(42);
    mSpriteChicken->setTranslate(0.0f, 30.0f);
    mSpriteChicken->setIndex(INDEX_FRAME_CHICKEN);*/
}

void GLRenderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);

    dirtFrame();
    //glDisable(GL_BLEND);
}

void GLRenderer::dirtFrame() {
    int elapse = GLUtils::getElapseRealtime();
    for (unsigned i = 0; i < mTextureCollection.size(); i++) {
        TextureGenerator* texture = mTextureCollection.at(i);
        // if (i == 0) continue;

        if (texture->getIndex() == INDEX_COIN) {
            /*texture->translate(0.0f, 10.0f);
            if (texture->getTranslateY() > 410.0f) {
                // mTextureCollection.pop_back();
                mTextureCollection.erase(mTextureCollection.begin() + i);
            }*/
        } else if (texture->getIndex() == INDEX_CLOUD_FIRST) {
            if (isUseFirstCloud) {
                texture->translate(2.0f, 0.0f);
                if (texture->getTranslateX() >= 30.0f) {
                    isUseSecondCloud = true;
                }

                if (texture->getTranslateX() > screenWidth) {
                    texture->hideX();
                    isUseFirstCloud = false;
                }
            }
        } else if (texture->getIndex() == INDEX_CLOUD_SECOND) {
            if (isUseSecondCloud) {
                texture->translate(2.0f, 0.0f);
                if (texture->getTranslateX() >= 30.0f) {
                    isUseFirstCloud = true;
                }

                if (texture->getTranslateX() > screenWidth) {
                    texture->hideX();
                    isUseSecondCloud = false;
                }
            }
        }
        texture->render(texture->getTextureId(), mMVPMatrix, mPositionHandle, mTexCoord, mMatrixHandle, mSamplerLoc);
    }

    /*if (mCurrentTime < elapse) {
        mSpriteChicken->changeFrameIndex();
    }
    mSpriteChicken->render(mSpriteChicken->getTextureId(), mMVPMatrix, mPositionHandle, mTexCoord, mMatrixHandle, mSamplerLoc);*/

    mCurrentTime = elapse;
}

void GLRenderer::drawCoin(int pieces) {
    LOGI("drawNewCoin called");
    // LOGI()("Size of textures %d", (int) sizeof(textures));
    LOGI("screenWidth %d", screenWidth);
    LOGI("screenHeight %d", screenHeight);
    for (int i = 0; i < pieces; i++) {
        GLuint r = GLuint (i % TEXTURE_SIZE);
        LOGI("r = %d", r);
        float pointX = rand() % (int)(((screenWidth - 48) - 48) + 1) + 48;
        //float pointX = rand() % (FIXED_WIDTH) + 48;;
        LOGI("PointX %.2f", pointX);
        float pointY = rand() % 310 + 1;
        LOGI("PointY %.2f", pointY);
        TextureGenerator* coin = new TextureGenerator(); // Same all height
        coin->setSize(40, 40);
        coin->setTranslate(pointX, pointY);
        coin->setTextureId(textures[0]);
        coin->setIndex(INDEX_COIN);
        mTextureCollection.push_back(coin);
        // LOGI("drawNewCoin %d", i);
    }
    LOGI("mCoinCollectionSize = %d", (int) mTextureCollection.size());
    LOGI("drawNewCoin end");
}
