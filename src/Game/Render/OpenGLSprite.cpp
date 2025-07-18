#include "Utility/PreLibrary.h"
#include "OpenGLSprite.h"

OpenGLSprite::OpenGLSprite() 
{
	Init();
}

OpenGLSprite::~OpenGLSprite()
{
}

uint32_t OpenGLSprite::GetVertexArray()
{
    return mVAO;
}

void OpenGLSprite::Free()
{
    ::glDeleteVertexArrays(1, &mVAO);
}

void OpenGLSprite::Init()
{
    uint32_t VBO;
    float vertices[] = 
    {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(mVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mCameraData.Projection = glm::ortho(0.0f, static_cast<float>(800), static_cast<float>(600), 0.0f, -1.0f, 1.0f);

    mOpenGLUniformBuffer.Init<Camera>(mCameraData);
    mOpenGLUniformBuffer.Update<Camera>(mCameraData, mCameraData.Projection);
}
