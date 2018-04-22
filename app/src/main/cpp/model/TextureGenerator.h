//
// Created by jerro on 4/20/2018.
//

#ifndef LEARNOPENGLES_TEXTUREGENERATOR_H
#define LEARNOPENGLES_TEXTUREGENERATOR_H


#include <GLES2/gl2.h>
#include "../graphics/Matrix.h"

typedef enum {
    INDEX_BG,
    INDEX_CLOUD_FIRST,
    INDEX_CLOUD_SECOND,
    INDEX_FRAME_CHICKEN,
    INDEX_COIN
} Index;

class TextureGenerator {
    /*int spriteWidth, spriteHeight;
    int texWidth, texHeight;

    int tex;*/
public:
    // The Constructor
    TextureGenerator();
    /*TextureGenerator(int t, int tW, int tH, int sW, int sH)
            : tex(t), texWidth(tW), texHeight(tH), spriteWidth(sW), spriteHeight(sH)
    {}*/

    // The Destructor
    ~TextureGenerator();

    // Getter
    GLuint getTextureId();
    float getTranslateX();
    float getTranslateY();
    Index getIndex();

    float getHeight();
    float getWidth();

    // Setter
    void setIndex(Index index);
    void setMaxFrames(int maxNumFrames);
    void setTextureId(GLuint textureId);
    void setSize(int width, int height);
    void setSpriteSize(int width, int height);
    void setTranslate(float deltaX, float deltaY);

    bool isSpriteSheet();

    void changeFrameIndex();
    void hideX();
    void hideY();
    void hide();
    void render(GLuint textureId, Matrix *mMVPMatrix, GLuint positionHandle, GLuint texCoord, GLint matrixHandle, GLint samplerLoc);
    void renderSprite(float posX, float posY, int frameIndex, GLuint textureId, Matrix *mMVPMatrix, GLuint positionHandle, GLuint texCoord, GLint matrixHandle, GLint samplerLoc);
    void translate(float deltaX, float deltaY);

private:
    // Rect - The Texture scale size
    GLfloat mTextureSize[4];
    // Vertex Buffer
    GLfloat mVertices[12];
    // Point - The texture position
    GLfloat mTranslate[2];
    // UVCoord
    GLfloat mUVCoord[8];

    GLuint *mTextures;
    GLuint mTextureId;

    Index mIndex;

    bool mIsSpriteSheet;

    int mCurrentFrameIndex = 1;
    int mMaxFrames;
    int mTextureWidth;
    int mTextureHeight;
    int mSpriteWidth;
    int mSpriteHeight;

    int getFrameIndex();
    void updateTransformedVertices();
};


#endif //LEARNOPENGLES_TEXTUREGENERATOR_H
