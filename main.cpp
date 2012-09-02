#include <iostream>
#include <cstdlib>
#include <GL/glfw.h>

/*
 ** 画面表示
 */
static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT);
}

/*
 ** OpenGL の初期設定
 */
static void initialize(void)
{
  // OpenGL の初期設定
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

/*
 ** プログラム終了時の処理
 */
static void terminate(void)
{
  // GLFW の後処理
  glfwTerminate();
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
  atexit(terminate);
  
  // OpenGL のウィンドウを開く
  if (!glfwOpenWindow(300, 300, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
  {
    std::cerr << "OpenGL のウィンドウを開けませんでした" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  // 初期設定
  initialize();
  
  // ウィンドウが開いている間繰り返す
  while (glfwGetWindowParam(GLFW_OPENED))
  {
    // 画面表示
    display();
    
    // ダブルバッファリング
    glfwSwapBuffers();
  }
  
  return EXIT_SUCCESS;
}
