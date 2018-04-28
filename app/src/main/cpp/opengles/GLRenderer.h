//
// Created by jerro on 4/20/2018.
//

#ifndef LEARNOPENGLES_GLRENDERER_H
#define LEARNOPENGLES_GLRENDERER_H


#include <GLES2/gl2.h>
#include <vector>
#include "../graphics/Matrix.h"
#include "../model/TextureGenerator.h"

using namespace std;

class GLRenderer {
public:
    // The constructor
    GLRenderer ();
    // The destructor
    ~GLRenderer();

    void create();

    void change(int width, int height);

    void draw();

    void drawCoin(int pieces);

private:
    // Our matrices
    Matrix *mViewMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVPMatrix;

    // Aspect Ratio
    float mAspectRatio;

    GLsizei screenWidth = 1280;
    GLsizei screenHeight = 768;

    static const int TEXTURE_SIZE = 8;

    int mCurrentTime;
    int spriteNum = 0;

    bool mClearSurface;
    bool isUseFirstCloud;
    bool isUseSecondCloud;

    // GL handle
    GLuint imageProgramHandle;
    GLuint mPositionHandle;
    GLuint mTexCoord;
    GLuint mMatrixHandle;
    GLuint mSamplerLoc;
    GLuint textures[TEXTURE_SIZE];
    GLuint* mTextures;

    vector<TextureGenerator *> mTextureCollection;
    TextureGenerator* mSpriteChicken;

    void dirtFrame();
};


#endif //LEARNOPENGLES_GLRENDERER_H
