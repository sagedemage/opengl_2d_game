#include <array>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "audio/audio.hpp"

// Window Size
constexpr unsigned int WINDOW_WIDTH = 640;
constexpr unsigned int WINDOW_HEIGHT = 640;
// Matrix Size
constexpr float IDENTITY_MATRIX = 750.0F;
// Player Speed
constexpr float PLAYER_SPEED = 0.005F;

bool GetShaderCode(const char *shader_file_path, std::string *shader_source);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);
void PlayerBoundaries(glm::vec4 vec, glm::mat4 *trans);

int main(void) {
    /* Shader File Path */
    const char *vertex_shader_path = "shader/shader.vert";
    const char *fragment_shader_path = "shader/shader.frag";

    /* Shader Source Code (GLSL code) */
    std::string vertex_shader_s;
    if (!GetShaderCode(vertex_shader_path, &vertex_shader_s)) {
        std::cout << "Unable to get vertex shader source code!" << std::endl;
        return -1;
    }

    std::string fragment_shader_s;
    if (!GetShaderCode(fragment_shader_path, &fragment_shader_s)) {
        std::cout << "Unable to get fragment shader source code!" << std::endl;
        return -1;
    }

    const char *vertex_shader_source = vertex_shader_s.c_str();
    const char *fragment_shader_source = fragment_shader_s.c_str();

    /* SDL_mixer */
    /*const int music_volume = 64;
    const int channels = 2;
    const int chunksize = 1024;
    const char *music_path = "music/square.ogg";*/

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, false);

    // Set the refresh rate to 60 frames per second
    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    // Enable VSync
    glfwSwapInterval(1);

    // Create GLFW Window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "OpenGL 2D Game", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW which loads all OpenGL functions
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    } else {
        std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION)
                  << std::endl;
    }

    glfwSetKeyCallback(window, KeyCallback);

    /* Build and compile the Shader */
    // vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int success = false;
    std::array<char, 512> info_log = {};
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log.data());
        std::cout << "Error: Vertex Shader Compilation Failed: "
                  << info_log.data() << std::endl;
    }

    // frament shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log.data());
        std::cout << "Error: Fragment Shader Compilation Failed: "
                  << info_log.data() << std::endl;
    }

    // link shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log.data());
        std::cout << "Error: Program Shader Compilation Failed: "
                  << info_log.data() << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /*
     * setup vertex data and buffers
     * configure vertex attributes
     */

    std::array<float, 36> vertices = {
        // positions        // colors
        -0.1F, 0.1F,  0.0F, 0.0F, 0.0F, 1.0F,  // left
        0.1F,  -0.1F, 0.0F, 1.0F, 0.0F, 0.0F,  // right
        0.1F,  0.1F,  0.0F, 1.0F, 0.0F, 1.0F,  // top

        -0.1F, 0.1F,  0.0F, 0.0F, 0.0F, 1.0F,  // left
        0.1F,  -0.1F, 0.0F, 1.0F, 0.0F, 0.0F,  // right
        -0.1F, -0.1F, 0.0F, 1.0F, 0.0F, 1.0F,  // top
    };

    unsigned int vbo = 0;
    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader_program);

    /*
     * Audio
     */

    // Open audio using SDL_mixer
    /*Audio audio(channels, chunksize);

    // Load and play music
    audio.LoadMusic(music_path);
    audio.PlayMusic();

    Audio::ChangeVolume(music_volume);*/

    glm::vec4 ori_vec(0.0F, 0.0F, 0.0F, 1.0F);
    glm::mat4 trans = glm::mat4(IDENTITY_MATRIX);

    while (!glfwWindowShouldClose(window)) {
        /* Game loop */
        /* Render here */
        glClearColor(0.0F, 0.725F, 0.098F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Player keybindings */
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // move the player left
            trans = glm::translate(trans, glm::vec3(-PLAYER_SPEED, 0.0F, 0.0F));
        } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // move the player right
            trans = glm::translate(trans, glm::vec3(PLAYER_SPEED, 0.0F, 0.0F));
        } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // move the player up
            trans = glm::translate(trans, glm::vec3(0.0F, PLAYER_SPEED, 0.0F));
        } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // move the player down
            trans = glm::translate(trans, glm::vec3(0.0F, -PLAYER_SPEED, 0.0F));
        }

        glm::vec4 vec = trans * ori_vec;
        PlayerBoundaries(vec, &trans);

        GLint transform_loc = glGetUniformLocation(shader_program, "transform");
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Dealocate all resources once they have outlived their purpose
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    // Dealocate Music and Audio
    // audio.FreeResources();

    // Terminates the GLFW library
    glfwTerminate();

    return 0;
}

bool GetShaderCode(const char *shader_file_path, std::string *shader_source) {
    /* Get the shader source code (GLSL) from a file as a string */
    std::ifstream read_shader_file(shader_file_path);

    if (!read_shader_file.is_open()) {
        std::cout << "failed to open shader file: " +
                         static_cast<std::string>(shader_file_path)
                  << std::endl;
        return false;
    }

    std::string s_code;
    std::string line;

    while (getline(read_shader_file, line)) {
        // add line to the string
        s_code += line + "\n";
    }

    read_shader_file.close();

    *shader_source = s_code;

    return true;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
    /* Keyboard input for the GLFW window */
    // when a user presses the escape key, we set the WindowShouldClose property
    // to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void PlayerBoundaries(glm::vec4 vec, glm::mat4 *trans) {
    /* Player matrix boundaries */
    const float width = IDENTITY_MATRIX / 10;
    if (vec.x - width < -IDENTITY_MATRIX) {
        // left boundary
        *trans = glm::translate(*trans, glm::vec3(PLAYER_SPEED, 0.0F, 0.0F));
    }
    if (vec.x + width > IDENTITY_MATRIX) {
        // right boundary
        *trans = glm::translate(*trans, glm::vec3(-PLAYER_SPEED, 0.0F, 0.0F));
    }
    if (vec.y - width < -IDENTITY_MATRIX) {
        // bottom boundary
        *trans = glm::translate(*trans, glm::vec3(0.0F, PLAYER_SPEED, 0.0F));
    }
    if (vec.y + width > IDENTITY_MATRIX) {
        // top boundary
        *trans = glm::translate(*trans, glm::vec3(0.0F, -PLAYER_SPEED, 0.0F));
    }
}
