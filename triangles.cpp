#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

GLFWwindow* glWindow = NULL;
const char *vertexShaderCode =
    "#version 150\n"
    "in vec3 vert;\n"
    "void main() {\n"
    "    gl_Position = vec4(vert, 1);\n"
    "}";
GLuint vertexShaderObject;
const char *fragmentShaderCode =
    "#version 150\n"
    "out vec4 finalColor;\n"
    "void main() {\n"
    "finalColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}";
GLuint fragmentShaderObject;
GLuint programObject;
GLuint vertexArrayObject;
GLuint vertexBufferObject;

int main() {

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

    glWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL From Scratch", NULL, NULL);

    if (!glWindow) {
        printf("glfwCreateWindow failed.\n");
        return 1;
    }

    glfwMakeContextCurrent(glWindow);
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

    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderObject, 1, (GLchar **)&vertexShaderCode, NULL);
    glCompileShader(vertexShaderObject);

    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObject, 1, (GLchar **)&fragmentShaderCode, NULL);
    glCompileShader(fragmentShaderObject);

    programObject = glCreateProgram();
    glAttachShader(programObject, vertexShaderObject);
    glAttachShader(programObject, fragmentShaderObject);
    glLinkProgram(programObject);

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

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
    GLint vertAttribute = glGetAttribLocation(programObject, "vert");

    glEnableVertexAttribArray(vertAttribute);
    glVertexAttribPointer(vertAttribute, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while(!glfwWindowShouldClose(glWindow)) {
        glfwPollEvents();

        glClearColor(1, 0, 0, 1); // black
        glClear(GL_COLOR_BUFFER_BIT);

        // bind the program (the shaders)
        glUseProgram(programObject);


        glBindVertexArray(vertexArrayObject);

        // draw the VAO
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // unbind the VAO
        glBindVertexArray(0);

        // unbind the program
        glUseProgram(0);

        // swap the display buffers (displays what was just drawn)
        glfwSwapBuffers(glWindow);
    }

    glfwTerminate();
}
