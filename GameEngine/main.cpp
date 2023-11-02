#include <GLFW/glfw3.h>
bool loadShaders(GLuint& program)
{
    char infoLog[512];
    GLint success;

    std::string tmp = "";
    std::string src = "";
    std::ifstream in_file;
    
    // Vertex shader creation
    in_file.open("shaders/vertex_shader.glsl");

    if (in_file.is_open())
    {
        while (std::getline(in_file, tmp))
        {
            src += tmp + "\n";
        }
    }
    else
    {
        std::cout << "Error while opening vertex shader file" << std::endl;
        return false;
    }

    in_file.close();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertexShaderSrc = src.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: Could not compile vertex shader" << std::endl;
        std::cout << infoLog << std::endl;
        return false;
    }


    // Fragment shader creation
    tmp = "";
    src = "";

    in_file.open("shaders/fragment_shader.glsl");

    if (in_file.is_open())
    {
        while (std::getline(in_file, tmp))
        {
            src += tmp + "\n";
        }
    }
    else
    {
        std::cout << "Error while opening fragment shader file" << std::endl;
        return false;
    }

    in_file.close();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragmentShaderSrc = src.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR: Could not compile fragment shader" << std::endl;
        std::cout << infoLog << std::endl;
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR: Unable to link program" << std::endl;
        std::cout << infoLog << std::endl;
        return false;
    }
    
    glUseProgram(0);
    glDeleteProgram(vertexShader);
    glDeleteProgram(fragmentShader);

    std::cout << "Program created correctly" << std::endl;
    return true;

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Unable to initialize OpenGL screen" << std::endl;
        return -1;
    }

    // OpenGL Initialization
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Unable to create OpenGL window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error while initializing glew" << std::endl;
        return -1;
    }

    // Rendering settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}