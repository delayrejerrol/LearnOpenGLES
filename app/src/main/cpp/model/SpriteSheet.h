//
// Created by jerro on 21/04/2018.
//

#ifndef LEARNOPENGLES_SPRITESHEET_H
#define LEARNOPENGLES_SPRITESHEET_H


#include <GLES2/gl2.h>
#include <graphics/Matrix.h>

class SpriteSheet {
    int spriteWidth, spriteHeight;
    int texWidth, texHeight;

    int tex;

public:
    SpriteSheet(int t, int tW, int tH, int sW, int sH)
            : tex(t), texWidth(tW), texHeight(tH), spriteWidth(sW), spriteHeight(sH)
    {}

    void drawSprite(float posX, float posY, int frameIndex, GLuint textureId, Matrix *mMVPMatrix, GLuint positionHandle,
                    GLuint texCoord, GLint matrixHandle, GLint samplerLoc);
};


#endif //LEARNOPENGLES_SPRITESHEET_H
