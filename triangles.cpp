#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

int main() {
    GLFWwindow* window = NULL;
    GLuint program;
    const char *vertexShaderCode =
        "#version 150\n"
        "in vec3 vert;\n"
        "void main() {\n"
        "    gl_Position = vec4(vert, 1);\n"
        "}";
    const char *fragmentShaderCode =
        "#version 150\n"
        "out vec4 finalColor;\n"
        "void main() {\n"
        "finalColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}";
    GLuint vertexShaderId;
    GLuint fragmentShaderId;
    GLuint vertexArrayId;
    GLuint vertexBufferId;

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

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    GLfloat vertexData[] = {
        //  X     Y     Z
         -1.0f, -1.0f,
         0.0f, 0.0f,
         -1.0f, 0.0f,

         0.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    GLint vertAttribute = glGetAttribLocation(program, "vert");

    glEnableVertexAttribArray(vertAttribute);
    glVertexAttribPointer(vertAttribute, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
