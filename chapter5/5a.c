#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void error_callback(int error, const char *description);
GLuint compile_shaders(void);

int main(int argc, char **argv)
{
  double time;
  float ratio;
  int width, height;
  GLFWwindow *window;
  GLuint rendering_program, vertex_buffer, vao;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(640, 480, "Vertex Attrib Pointer", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  static const GLfloat data[] =
  {
    0.25, -0.25, 0.5, 1.0,
    -0.25, -0.25, 0.5, 1.0,
    0.25, 0.25, 0.5, 1.0
  };

  // Set openGL context to the window object
  glfwMakeContextCurrent(window);
  rendering_program = compile_shaders();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glfwSetKeyCallback(window, key_callback);
  // can also be done using glfwGetFramebufferSize(window, &width, &height)
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // can additionally set glfwSetWindowCloseCallback().
  // Can be manually set with glfwSetWindowShouldClose().
  while(!glfwWindowShouldClose(window)) {
    time = glfwGetTime();
    const GLfloat color[] = { (float) sin(time) * 0.5f + 0.5f,
                              (float) cos(time) * 0.5f + 0.5f,
                              0.f, 1.f };

    glClearBufferfv(GL_COLOR, 0, color);
    glUseProgram(rendering_program);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteProgram(rendering_program);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

GLuint compile_shaders(void)
{
  GLuint vertex_shader, fragment_shader, program;

  static const GLchar *vertex_shader_source[] =
  {
    "#version 410 core                                                 \n"
    "                                                                  \n"
    "layout (location = 0) in vec4 position;                           \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "    gl_Position = position;                                       \n"
    "}                                                                 \n"
  };

  static const GLchar *fragment_shader_source[] =
  {
    "#version 410 core \n"
    "out vec4 color; \n"
    "void main(void) \n"
    "{ \n"
    " color = vec4(0.0, 0.8, 1.0, 1.0); \n"
    "} \n"
  };

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return program;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

static void error_callback(int error, const char *description)
{
  fprintf(stdout, "%s", description);
}
