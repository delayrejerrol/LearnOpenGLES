//
// Created by jerro on 4/20/2018.
//

#include <android/log.h>
#include "TextureGenerator.h"

#define LOG_TAG "TextureGenerator"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

static const GLshort mIndices[] = {
        0, 1, 2,
        0, 2, 3
};

/*static const GLfloat mUVCoord[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
};*/
/*static GLfloat mUVCoord[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
};*/

TextureGenerator::TextureGenerator() {
    mUVCoord[0] = 1.0f;
    mUVCoord[1] = 1.0f;
    mUVCoord[2] = 1.0f;
    mUVCoord[3] = 0.0f;
    mUVCoord[5] = 0.0f;
    mUVCoord[4] = 0.0f;
    mUVCoord[6] = 0.0f;
    mUVCoord[7] = 1.0f;

    mIsSpriteSheet = false;
}

TextureGenerator::~TextureGenerator() {
    /*delete[] mVertices;
    mVertices[0] = NULL;
    mVertices[1] = NULL;
    mVertices[2] = NULL;
    mVertices[3] = NULL;
    mVertices[4] = NULL;
    mVertices[5] = NULL;
    mVertices[6] = NULL;
    mVertices[7] = NULL;
    mVertices[8] = NULL;
    mVertices[9] = NULL;
    mVertices[10] = NULL;
    mVertices[11] = NULL;

    delete[] mTextureSize;
    mTextureSize[0] = NULL;
    mTextureSize[1] = NULL;
    mTextureSize[2] = NULL;
    mTextureSize[3] = NULL;

    delete[] mTranslate;
    mTranslate[0] = NULL;
    mTranslate[1] = NULL;*/
}

GLuint TextureGenerator::getTextureId() {
    GLuint nextTextureId = mTextureId;

    /*if (nextTextureId == 7) {
        nextTextureId = 0;
    } else {
        nextTextureId++;
    }

    mTextureId = nextTextureId;
    return nextTextureId;*/

    return nextTextureId;
}

void TextureGenerator::setTextureId(GLuint textureId) {
    mTextureId = textureId;
}

float TextureGenerator::getTranslateX() {
    return mTranslate[0];
}

float TextureGenerator::getTranslateY() {
    return mTranslate[1];
}

void TextureGenerator::setTranslate(float deltaX, float deltaY) {
    mTranslate[0] = (GLfloat) deltaX; // X
    mTranslate[1] = (GLfloat) deltaY; // Y

    updateTransformedVertices();
}

void TextureGenerator::translate(float deltaX, float deltaY) {
    mTranslate[0] += (GLfloat) deltaX;
    mTranslate[1] += (GLfloat) deltaY;

    updateTransformedVertices();
}

void TextureGenerator::setSize(int width, int height) {
    //float left, float top, float right, float bottom
    /*mTextureSize[0] = 0.0f; // Left
    mTextureSize[1] = 0.0f;  // Top
    mTextureSize[2] = width;  // Right
    mTextureSize[3] = height; // Bottom*/

    mTextureWidth = width;
    mTextureHeight = height;
    mTextureSize[0] = (float)mTextureWidth; // Left
    mTextureSize[1] = (float)mTextureHeight;  // Top
    mTextureSize[2] = 0.0f;  // Right
    mTextureSize[3] = 0.0f; // Bottom

    updateTransformedVertices();
}

void TextureGenerator::setSpriteSize(int width, int height) {
    mSpriteWidth = width;
    mSpriteHeight = height;

    mIsSpriteSheet = true;
}

void TextureGenerator::updateTransformedVertices() {
    // Start with scaling
    /*GLfloat x1 = mTextureSize[0]; // Left
    GLfloat x2 = mTextureSize[2]; // Right
    GLfloat y1 = mTextureSize[3]; // Bottom
    GLfloat y2 = mTextureSize[1]; // Top*/
    GLfloat x1 = mTextureSize[0]; // Left
    GLfloat x2 = mTextureSize[1]; // Top
    GLfloat y1 = mTextureSize[2]; // Right
    GLfloat y2 = mTextureSize[3]; // Bottom

    // We now detach from our Rect because when rotating,
    // we need the seperate points, so we do so in opengl order
    /*GLfloat one[2] = {x1, y2};
    GLfloat two[2] = {x1, y1};
    GLfloat three[2] = {x2, y1};
    GLfloat four[2] = {x2, y2};*/
    GLfloat one[2] = {x1, x2};
    GLfloat two[2] = {x1, y2};
    GLfloat three[2] = {y1, y2};
    GLfloat four[2] = {y1, x2};

    // Finally we translate the texture to its correct position.
    one[0] += mTranslate[0];
    one[1] += mTranslate[1];

    two[0] += mTranslate[0];
    two[1] += mTranslate[1];

    three[0] += mTranslate[0];
    three[1] += mTranslate[1];

    four[0] += mTranslate[0];
    four[1] += mTranslate[1];

    // Left
    mVertices[0] = one[0];
    mVertices[1] = one[1];
    mVertices[2] = 0.0f;

    // Top
    mVertices[3] = two[0];
    mVertices[4] = two[1];
    mVertices[5] = 0.0f;

    // Right
    mVertices[6] = three[0];
    mVertices[7] = three[1];
    mVertices[8] = 0.0f;

    // Bottom
    mVertices[9] = four[0];
    mVertices[10] = four[1];
    mVertices[11] = 0.0f;
}

Index TextureGenerator::getIndex() {
    return mIndex;
}

void TextureGenerator::setIndex(Index index) {
    mIndex = index;
}

void TextureGenerator::render(GLuint textureId, Matrix *mMVPMatrix, GLuint positionHandle,
                              GLuint texCoord, GLint matrixHandle, GLint samplerLoc) {

    if (isSpriteSheet()) {
        const float tw = float(mSpriteWidth) / mTextureWidth;
        const float th = float(mSpriteHeight) / mTextureHeight;
        const int numPerRow = mTextureWidth / mSpriteWidth;
        const int numPerColumn = mTextureHeight / mSpriteHeight;

        const float tx = (getFrameIndex() % numPerRow) * tw;
        const float ty = (getFrameIndex() / numPerRow + 1) * th;

        mUVCoord[2] = tx;
        mUVCoord[3] = ty;
        mUVCoord[4] = tx + tw;
        mUVCoord[5] = ty;
        mUVCoord[6] = tx + tw;
        mUVCoord[7] = ty + th;
        mUVCoord[0] = tx;
        mUVCoord[1] = ty + th;
    }
    // Enable generic vertex attribute array
    glEnableVertexAttribArray(positionHandle);
    glEnableVertexAttribArray(texCoord);

    // Prepare the triangle coordinate data
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, mVertices);
    // Prepare the texture coordinates
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, mUVCoord);

    // Apply the projection and view transformation
    glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, mMVPMatrix->mData);
    //  Set the sampler texture unit to 0, where we have saved the texture
    glUniform1i(samplerLoc, 0);

    // Pass in the texture information
    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to this unit
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Draw the triangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, mIndices);

    // Disable generic vertex attribute array
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(texCoord);

}

void TextureGenerator::hideX() {
    mTranslate[0] = -(mTextureWidth); // X
    // mTranslate[1] = 0.0f; // Y

    updateTransformedVertices();
}

void TextureGenerator::hideY() {
    mTranslate[0] = -(mTextureWidth); // X
    mTranslate[1] = 0.0f; // Y

    updateTransformedVertices();
}

void TextureGenerator::hide() {
    mTranslate[0] = -(mTextureWidth); // X
    mTranslate[1] = -(mTextureHeight); // Y

    updateTransformedVertices();
}

float TextureGenerator::getHeight() {
    return mTextureHeight;
}

float TextureGenerator::getWidth() {
    return mTextureWidth;
}

void TextureGenerator::renderSprite(float posX, float posY, int frameIndex, GLuint textureId,
                                    Matrix *mMVPMatrix, GLuint positionHandle, GLuint texCoord,
                                    GLint matrixHandle, GLint samplerLoc) {

    /*const float verts[] = {
            posX, posY,
            posX + spriteWidth, posY,
            posX + spriteWidth, posY + spriteHeight,
            posX, posY + spriteHeight
    };
    const float tw = float(spriteWidth) / texWidth;
    const float th = float(spriteHeight) / texHeight;
    const int numPerRow = texWidth / spriteWidth;
    const int numPerColumn = texHeight / spriteHeight;

    //LOGI("numPerRow: %d", numPerRow);
    //LOGI("numPerColumn: %d", numPerColumn);

    const float tx = (frameIndex % numPerRow) * tw;
    const float ty = (frameIndex / numPerRow + 1) * th;

    LOGI("tx: %.2f", tx);
    LOGI("ty: %.2f", ty);
    *//*const float texVerts[] = {
            tx, ty,
            tx + tw, ty,
            tx + tw, ty + th,
            tx, ty + th
    };*//*

    *//*static GLfloat mUVCoord[] = {
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
    };*//*

    *//*mUVCoord[0] = tx;
    mUVCoord[1] = ty;
    mUVCoord[2] = tx + tw;
    mUVCoord[3] = ty;
    mUVCoord[4] = tx + tw;
    mUVCoord[5] = ty + th;
    mUVCoord[6] = tx;
    mUVCoord[7] = ty + th;*//*

    mUVCoord[2] = tx;
    mUVCoord[3] = ty;
    mUVCoord[4] = tx + tw;
    mUVCoord[5] = ty;
    mUVCoord[6] = tx + tw;
    mUVCoord[7] = ty + th;
    mUVCoord[0] = tx;
    mUVCoord[1] = ty + th;


    // Enable generic vertex attribute array
    glEnableVertexAttribArray(positionHandle);
    glEnableVertexAttribArray(texCoord);

    // Prepare the triangle coordinate data
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, mVertices);
    // Prepare the texture coordinates
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, mUVCoord);

    // Apply the projection and view transformation
    glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, mMVPMatrix->mData);
    //  Set the sampler texture unit to 0, where we have saved the texture
    glUniform1i(samplerLoc, 0);

    // Pass in the texture information
    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to this unit
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Draw the triangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, mIndices);

    // Disable generic vertex attribute array
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(texCoord);*/
}

bool TextureGenerator::isSpriteSheet() {
    return mIsSpriteSheet;
}

int TextureGenerator::getFrameIndex() {

    return mCurrentFrameIndex;
}

void TextureGenerator::setMaxFrames(int maxNumFrames) {
    mMaxFrames = maxNumFrames;
}

void TextureGenerator::changeFrameIndex() {
    if (isSpriteSheet()) {
        mCurrentFrameIndex += 1;
        if (mCurrentFrameIndex > mMaxFrames) {
            mCurrentFrameIndex = 1;
        }
        LOGI("mMaxFrames: %d", mMaxFrames);
        LOGI("mCurrentFrameIndex: %d", mCurrentFrameIndex);
    }
}
