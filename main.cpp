#include <iostream>
#include <cstdlib>
#include <GL/glfw.h>

static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT);
  
}
static void leave(void)
{
  // GLFW の後処理
  glfwTerminate();
}

int main(int argc, char *argv[])
{
  // GLFW の初期化
  if (!glfwInit())
  {
    std::cerr << "GLFW の初期化に失敗しました" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  // プログラム終了時の処理を設定する
  atexit(leave);
  
  // OpenGL のウィンドウを開く
  if (!glfwOpenWindow(300, 300, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
  {
    std::cerr << "OpenGL のウィンドウを開けませんでした" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  // OpenGL の初期設定
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  // メインループ
  for (int running = GL_TRUE; running != GL_FALSE; running = glfwGetWindowParam(GLFW_OPENED))
  {
    // 画面表示
    display();
    
    // ダブルバッファリング
    glfwSwapBuffers();
  }
  
  exit( EXIT_SUCCESS );
}
