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
  GLuint rendering_program, vertex_array_object;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(640, 480, "Rotating Triangle", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Set openGL context to the window object
  glfwMakeContextCurrent(window);

  rendering_program = compile_shaders();
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glfwSetKeyCallback(window, key_callback);

  // can also be done using glfwGetFramebufferSize(window, &width, &height)
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // can additionally set glfwSetWindowCloseCallback().
  // Can be manually set with glfwSetWindowShouldClose().
  while(!glfwWindowShouldClose(window)) {
    time = glfwGetTime();
    const GLfloat color[] = { 0.5f,
                              0.5f,
                              0.5f, 1.f };
    glClearBufferfv(GL_COLOR, 0, color);

    glUseProgram(rendering_program);

    glDrawArrays(GL_PATCHES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteProgram(rendering_program);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

GLuint compile_shaders(void)
{
  GLuint vertex_shader, tess_control_shader, tess_eval_shader,
         fragment_shader, program;

  static const GLchar *vertex_shader_source[] =
  {
    "#version 410 core                                                 \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
    "                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
    "                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
    "                                                                  \n"
    "    gl_Position = vertices[gl_VertexID];                          \n"
    "}                                                                 \n"
  };

  static const GLchar *tess_control_shader_source[] =
  {
    "#version 410 core                                                 \n"
    "                                                                  \n"
    "layout (vertices = 3) out;                                        \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "   if (gl_InvocationID == 0) {                                    \n"
    "     gl_TessLevelInner[0] = 5.0;                                  \n"
    "     gl_TessLevelOuter[0] = 5.0;                                  \n"
    "     gl_TessLevelOuter[1] = 5.0;                                  \n"
    "     gl_TessLevelOuter[2] = 5.0;                                  \n"
    "   }                                                              \n"
    "   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
    "}                                                                 \n"
  };

  static const GLchar *tess_eval_shader_source[] =
  {
    "#version 410 core                                                 \n"
    "                                                                  \n"
    "layout (triangles, equal_spacing, cw) in;                         \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "   gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +         \n"
    "                  gl_TessCoord.y * gl_in[1].gl_Position +         \n"
    "                  gl_TessCoord.z * gl_in[2].gl_Position);         \n"
    "}                                                                 \n"
  };

  static const GLchar *fragment_shader_source[] =
  {
    "#version 410 core                                                 \n"
    "                                                                  \n"
    "out vec4 color;                                                   \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "   color = vec4(1.f, 0.f, 0.f, 1.f);                              \n"
    "}                                                                 \n"
  };

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  tess_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
  glShaderSource(tess_control_shader, 1, tess_control_shader_source, NULL);
  glCompileShader(tess_control_shader);

  tess_eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
  glShaderSource(tess_eval_shader, 1, tess_eval_shader_source, NULL);
  glCompileShader(tess_eval_shader);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, tess_control_shader);
  glAttachShader(program, tess_eval_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glDeleteShader(vertex_shader);
  glDeleteShader(tess_control_shader);
  glDeleteShader(tess_eval_shader);
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
