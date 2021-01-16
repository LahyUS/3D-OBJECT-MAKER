#pragma once

extern float skyboxVertices[] =
{
    // positions          
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};


extern float GrassTexCoords[] =
{
    0.0f,  0.0f,
    10.0f,  0.0f,
    10.0f, 10.0f,
    10.0f, 10.0f,
    0.0f, 10.0f,
    0.0f,  0.0f
};

extern float GrassVertices[] =
{
    // Vertices             // Texture
  // positions          // texture Coords

        -100.0f,  0.5f, -100.0f,  0.0f, 1.0f,
         100.0f,  0.5f, -100.0f,  1.0f, 1.0f,
         100.0f,  0.5f,  100.0f,  1.0f, 0.0f,
         100.0f,  0.5f,  100.0f,  1.0f, 0.0f,
        -100.0f,  0.5f,  100.0f,  0.0f, 0.0f,
        -100.0f,  0.5f, -100.0f,  0.0f, 1.0f
};

extern glm::vec3 Translate_Var(0.0f, 0.0f, 0.0f);

extern glm::vec3 Rotate_Var(0.0f, 0.0f, 0.0f);

extern glm::vec3 Scale_Var(0.2, 0.2f, 0.2f);

extern float Radian = 0.0;