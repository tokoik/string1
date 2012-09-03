#include <iostream>
#include <cstdlib>
#include <cmath>

// 補助ライブラリ
#include "gg.h"
using namespace gg;

static GLfloat pv[][2] =
{
  { -0.9f, -0.9f },
  {  0.9f, -0.9f },
  {  0.9f,  0.9f },
  { -0.9f,  0.9f }
};

static GLfloat cv[][3] =
{
  { 1.0f, 0.0f, 0.0f },
  { 1.0f, 0.0f, 0.0f },
  { 1.0f, 0.0f, 0.0f },
  { 1.0f, 0.0f, 0.0f }
};

static const GLchar *vs[] =
{
  "#version 150",
  "in vec4 pv;",
  "void main(void)",
  "{",
  "  gl_Position = pv;",
  "}"
};

static const GLchar *fs[] =
{
  "#version 150",
  "void main(void)",
  "{",
  "  gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);",
  "}"
};

static GLuint program;
static GLint pvLoc;

/*
** シェーダの情報を表示する
*/
static void printShaderInfoLog(GLuint shader)
{
  // シェーダのコンパイル時のログの長さを取得する
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1)
  {
    // シェーダのコンパイル時のログの内容を取得する
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** プログラムの情報を表示する
*/
static void printProgramInfoLog(GLuint program)
{
  // シェーダのリンク時のログの長さを取得する
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1)
  {
    // シェーダのリンク時のログの内容を取得する
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** 画面表示
*/
static void display(void)
{
  static int frame = 0;
  
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  glEnableVertexAttribArray(pvLoc);
  glVertexAttribPointer(pvLoc, sizeof pv[0] / sizeof pv[0][0], GL_FLOAT, GL_FALSE, 0, pv);
  glDrawArrays(GL_LINE_LOOP, 0, sizeof pv / sizeof pv[0]);
  glDisableVertexAttribArray(pvLoc);
  glUseProgram(0);

  std::cerr << "display:" << ++frame << std::endl;
}

/*
** OpenGL の初期設定
*/
static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();
  
  // プログラムオブジェクトの作成
  program = glCreateProgram();

  // シェーダオブジェクト
  GLuint shader;
  
  // コンパイル／リンク結果
  GLint status;
  
  // バーテックスシェーダの作成
  shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader, sizeof vs / sizeof vs[0], vs, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Compile Error in Vertex Shader." << std::endl;
  printShaderInfoLog(shader);
  glAttachShader(program, shader);
  glDeleteShader(shader);
  
  // フラグメントシェーダの作成
  shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader, sizeof fs / sizeof fs[0], fs, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Compile Error in Fragment Shader." << std::endl;
  printShaderInfoLog(shader);
  glAttachShader(program, shader);
  glDeleteShader(shader);
  ggError("link");
  // プログラムのリンク
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;
  printProgramInfoLog(program);
  
  // attribute 変数 pv の場所
  pvLoc = glGetAttribLocation(program, "pv");
  
  // OpenGL の初期設定
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

/*
** プログラム終了時の処理
*/
static void term(void)
{
  // GLFW の後処理
  glfwTerminate();
}

/*
** ウィンドウのリサイズ
*/
static void resize(int w, int h)
{
  // ウィンドウ全体をビューポート（表示領域）にする
  glViewport(0, 0, w, h);
  
  std::cerr << w << "," << h << std::endl;
}

/*
** キーボードをタイプした時の処理
*/
static void keyboard(int key, int state)
{
  if (state)
  {
    switch (key)
    {
      case GLFW_KEY_ESC:
      case 'Q':
        // ESC キー, q, Q をタイプしたら終了する
        exit(EXIT_SUCCESS);
      default:
        break;
    }
  }
}

/*
** 左ボタンドラッグの処理
*/
static void motionLeft(int x, int y)
{
  std::cerr << x << "," << y << std::endl;
}

/*
** マウスのボタンを押したときの処理
*/
static void mouse(int button, int state)
{
  switch (button)
  {
    case GLFW_MOUSE_BUTTON_LEFT:
      if (state)
      {
        // 左ボタンを押したときの処理
        int x, y;
        glfwGetMousePos(&x, &y);
        glfwSetMousePosCallback(motionLeft);
      }
      else
      {
        // 左ボタンを離したときの処理
        glfwSetMousePosCallback(NULL);
      }
      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      if (state)
      {
        // 中ボタンを押したときの処理
      }
      else
      {
        // 中ボタンを離したときの処理
      }
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      if (state)
      {
        // 右ボタンを押したときの処理
      }
      else
      {
        // 右ボタンを離したときの処理
      }
      break;
    default:
      break;
  }
}

/*
** マウスホイールを回した時の処理
*/
static void wheel(int position)
{
  std::cerr << position << std::endl;
}

/*
** メインプログラム
*/
int main(int argc, char *argv[])
{
  // GLFW の初期化
  if (!glfwInit())
  {
    std::cerr << "GLFW の初期化に失敗しました" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  // プログラム終了時の処理を設定する
  atexit(term);
  
  // OpenGL の Version 3.2 を選択する
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // OpenGL のウィンドウを開く
  if (!glfwOpenWindow(300, 300, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
  {
    std::cerr << "OpenGL のウィンドウを開けませんでした" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  // ウィンドウタイトルを設定する
  glfwSetWindowTitle("String");
  glfwSetWindowSizeCallback(resize);
  glfwSetKeyCallback(keyboard);
  glfwSetMouseButtonCallback(mouse);
  glfwSetMouseWheelCallback(wheel);
  
  // 初期設定
  init();
  
  // ウィンドウが開いている間繰り返す
  while (glfwGetWindowParam(GLFW_OPENED))
  {
    // 画面表示
    display();
    
    // ダブルバッファリング
    glfwSwapBuffers();

    // イベント待ち
    glfwWaitEvents();
  }
  
  return EXIT_SUCCESS;
}
