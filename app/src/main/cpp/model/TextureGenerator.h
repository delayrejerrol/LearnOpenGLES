//
// Created by jerro on 4/20/2018.
//

#ifndef LEARNOPENGLES_TEXTUREGENERATOR_H
#define LEARNOPENGLES_TEXTUREGENERATOR_H


#include <GLES2/gl2.h>
#include "../graphics/Matrix.h"

typedef enum {
    INDEX_BG,
    INDEX_COIN
} Index;

class TextureGenerator {
public:
    // The Constructor
    TextureGenerator();
    // The Destructor
    ~TextureGenerator();

    // Getter
    GLuint getTextureId();
    float getTranslateX();
    float getTranslateY();
    Index getIndex();

    // Setter
    void setIndex(Index index);
    void setTextureId(GLuint textureId);
    void setSize(float width, float height);
    void setTranslate(float deltaX, float deltaY);

    void render(GLuint textureId, Matrix *mMVPMatrix, GLuint positionHandle, GLuint texCoord, GLint matrixHandle, GLint samplerLoc);
    void translate(float deltaX, float deltaY);

private:
    // Rect - The Texture scale size
    GLfloat mTextureSize[4];
    // Vertex Buffer
    GLfloat mVertices[12];
    // Point - The texture position
    GLfloat mTranslate[2];

    GLuint *mTextures;
    GLuint mTextureId;

    Index mIndex;
    void updateTransformedVertices();
};


#endif //LEARNOPENGLES_TEXTUREGENERATOR_H
