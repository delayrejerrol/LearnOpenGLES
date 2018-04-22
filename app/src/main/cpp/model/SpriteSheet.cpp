//
// Created by jerro on 21/04/2018.
//

#include "SpriteSheet.h"

void SpriteSheet::drawSprite(float posX, float posY, int frameIndex,
                             GLuint textureId, Matrix *mMVPMatrix, GLuint positionHandle,
                             GLuint texCoord, GLint matrixHandle, GLint samplerLoc) {
    const float verts[] = {
            posX, posY,
            posX + spriteWidth, posY,
            posX + spriteWidth, posY + spriteHeight,
            posX, posY + spriteHeight
    };
    const float tw = float(spriteWidth) / texWidth;
    const float th = float(spriteHeight) / texHeight;
    const int numPerRow = texWidth / spriteWidth;
    const float tx = (frameIndex % numPerRow) * tw;
    const float ty = (frameIndex / numPerRow + 1) * th;
    const float texVerts[] = {
            tx, ty,
            tx + tw, ty,
            tx + tw, ty + th,
            tx, ty + th
    };

    const GLshort mIndices[] = {
            0, 1, 2,
            0, 2, 3
    };

    // Enable generic vertex attribute array
    glEnableVertexAttribArray(positionHandle);
    glEnableVertexAttribArray(texCoord);

    // Prepare the triangle coordinate data
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, verts);
    // Prepare the texture coordinates
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, texVerts);

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
