#pragma once

// GL Includes
#define GLEW_STATIC
#include <glad/glad.h>  
//#include <stb_image.h>
#include <vector>
#include <string>

class TextureLoading
{
public:
    //static GLuint LoadTexture(GLchar* path)
    //{
    //    //Generate texture ID and load texture data
    //    GLuint textureID;
    //    glGenTextures(1, &textureID);

    //    int imageWidth, imageHeight;

    //    unsigned char* image = stbi_load(path, &imageWidth, &imageHeight, 0, 0);

    //    // Assign texture to ID
    //    glBindTexture(GL_TEXTURE_2D, textureID);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //    glGenerateMipmap(GL_TEXTURE_2D);

    //    // Parameters
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glBindTexture(GL_TEXTURE_2D, 0);

    //    stbi_image_free(image);

    //    return textureID;
    //}

    
};