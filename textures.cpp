#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

GLenum textureFormat(int channels) {
    switch (channels) {
        case 1: return GL_LUMINANCE;
        case 2: return GL_LUMINANCE_ALPHA;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return -1;
    }
}

int main() {
    GLFWwindow* window = NULL;
    GLuint program;
    const char *vertexShaderCode =
        "#version 150\n"
        "in vec3 vert;\n"
        "in vec2 vertTexCoord;\n"
        "out vec2 fragTexCoord;\n"
        "void main() {\n"
        "    fragTexCoord = vertTexCoord;\n"
        "    gl_Position = vec4(vert, 1);\n"
        "}";
    const char *fragmentShaderCode =
        "#version 150\n"
        "uniform sampler2D tex;\n"
        "in vec2 fragTexCoord;\n"
        "out vec4 finalColor;\n"
        "void main() {\n"
        "    finalColor = texture(tex, fragTexCoord);\n"
        "}";
    GLuint vertexShaderId;
    GLuint fragmentShaderId;
    GLuint vertexArrayId;
    GLuint vertexBufferId;
    GLuint textureId;
    int imageWidth, imageHeight, imageChannels;
    unsigned char* imagePixels;
    GLuint texUniformId;

    printf("Initializing application.\n");

    if (!glfwInit()) {
        printf("glfwInit failed.\n");
        return 1;
    }

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL From Scratch", NULL, NULL);

    if (!window) {
        printf("glfwCreateWindow failed.\n");
        return 1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        printf("glewInit failed.\n");
        return 1;
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));

    if(!GLEW_VERSION_3_2) {
        printf("OpenGL 3.2 API is not available.\n");
        return 1;
    }

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, (GLchar **)&vertexShaderCode, NULL);
    glCompileShader(vertexShaderId);

    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, (GLchar **)&fragmentShaderCode, NULL);
    glCompileShader(fragmentShaderId);

    program = glCreateProgram();
    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);

    imagePixels = stbi_load("hazard.png", &imageWidth, &imageHeight, &imageChannels, 0);

    printf("Loaded %dx%d image\n", imageWidth, imageHeight);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,
                     0,
                     textureFormat(imageChannels),
                     (GLsizei)imageWidth,
                     (GLsizei)imageHeight,
                     0,
                     textureFormat(imageChannels),
                     GL_UNSIGNED_BYTE,
                     imagePixels);
    stbi_image_free(imagePixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    GLfloat vertexData[] = {
        //  X     Y     Z       U     V
         0.0f, 0.8f, 0.0f,   0.5f, 1.0f,
        -0.8f,-0.8f, 0.0f,   0.0f, 0.0f,
         0.8f,-0.8f, 0.0f,   1.0f, 0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLint vertAttribute = glGetAttribLocation(program, "vert");
    glEnableVertexAttribArray(vertAttribute);
    glVertexAttribPointer(vertAttribute, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

    GLint vertTexCoordAttribute = glGetAttribLocation(program, "vertTexCoord");
    glEnableVertexAttribArray(vertTexCoordAttribute);
    glVertexAttribPointer(vertTexCoordAttribute, 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the buffer and array so they "don't accidentally get used somewhere else"
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        texUniformId = glGetUniformLocation(program, "tex");
        glUniform1i(texUniformId, 0);

        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();

}
