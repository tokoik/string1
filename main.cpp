#include <iostream>
#include <cstdlib>
#include <cmath>

// 補助ライブラリ
#include "gg.h"
using namespace gg;

/*
** 画面表示
*/
static void display(void)
{
  static int frame = 0;
  
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT);
  
  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_LINE_LOOP);
  glVertex2d(-0.9, -0.9);
  glVertex2d( 0.9, -0.9);
  glVertex2d( 0.9,  0.9);
  glVertex2d(-0.9,  0.9);
  glEnd();
  
  std::cerr << "display:" << ++frame << std::endl;
}

/*
** OpenGL の初期設定
*/
static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();
  
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
    for (;;)
    {
      int w, h, x, y;

      glfwGetWindowSize(&w, &h);
      glfwGetMousePos(&x, &y);
      if (x >= 0 && x < w && y >= 0 && y < h)
      {
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) break;
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) break;
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE)) break;
      }

      glfwWaitEvents();
    }
  }
  
  return EXIT_SUCCESS;
}
