#include <cstdlib>
#include <cmath>
 
// 補助ライブラリ
#include "Gg.h"
using namespace gg;
 
// 頂点データ
static const int pointn = 20;                               // 点の数
static const GLfloat pointSize = 5.0f;                      // 点の大きさ
static const GLfloat point0[] = { -0.9f,  0.0f,  0.0f };    // 始点の位置
static const GLfloat point1[] = {  0.9f,  0.0f,  0.0f };    // 終点の位置
 
// 点データ
static class PointBuffer
{
  // 頂点バッファオブジェクト
  GgBuffer<GLfloat[4]> position;
 
public:
  
  // デストラクタ
  ~PointBuffer(void) {}
  
  // コンストラクタ
  //    n: 頂点数, pos: 頂点の位置
  PointBuffer(unsigned int n)
  {
    // 頂点バッファオブジェクトのメモリを確保する
    position.load(GL_ARRAY_BUFFER, n, 0, GL_DYNAMIC_COPY);
 
    // 作成した頂点バッファオブジェクトに初期値を設定する
    GLfloat (*p)[4] = (GLfloat (*)[4])glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (unsigned int i = 0; i < n; ++i)
    {
      GLfloat t = (GLfloat)i / (GLfloat)(pointn - 1);
      
      p[i][0] = point0[0] * (1.0f - t) + point1[0] * t;
      p[i][1] = point0[1] * (1.0f - t) + point1[1] * t;
      p[i][2] = point0[2] * (1.0f - t) + point1[2] * t;
      p[i][3] = 1.0f;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
 
  // 描画
  void draw(GLint pvLoc, GLenum mode)
  {
    glBindBuffer(GL_ARRAY_BUFFER, position.buf());
    glEnableVertexAttribArray(pvLoc);
    glVertexAttribPointer(pvLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(mode, 0, position.num());
    glDisableVertexAttribArray(pvLoc);
  }
 
  // バッファ中の点から (x, y) に近いものを探す
  int pick(GLfloat x, GLfloat y, GLfloat dx, GLfloat dy) const
  {
    glBindBuffer(GL_ARRAY_BUFFER, position.buf());
    GLfloat (*p)[4] = (GLfloat (*)[4])glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    for (unsigned int i = 0; i < position.num(); ++i)
    {
      if (fabs(p[i][0] - x) <= dx && fabs(p[i][1] - y) <= dy)
      {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        return i;
      }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return -1;
  }
  
  // バッファ中の i 番目の点の位置を (x, y) に設定する
  void move(int i, GLfloat x, GLfloat y) const
  {
    glBindBuffer(GL_ARRAY_BUFFER, position.buf());
    GLfloat p[] = { x, y };
    glBufferSubData(GL_ARRAY_BUFFER, sizeof (GLfloat[4]) * i, sizeof p, p);
  }
 
} *pointBuffer = 0;                           // 頂点バッファオブジェクト
 
// シェーダ
static class PointShader
  : public GgShader
{
  // attribute 変数の場所
  GLint positionLoc;
  
  // uniform 変数の場所
  GLint colorLoc;
 
public:
 
  // デストラクタ
  ~PointShader(void) {}
  
  // コンストラクタ
  PointShader(
    const char *vert = "point.vert",
    const char *frag = "point.frag",
    const char *position = "position",
    const char *color = "color",
    const char *fragcolor = "FragColor"
    )
    : GgShader(vert, frag)
    , positionLoc(glGetAttribLocation(getProgram(), position))
    , colorLoc(glGetUniformLocation(getProgram(), color))
  {
    glBindFragDataLocation(getProgram(), 0, fragcolor);
  }
  
  // 色を設定する
  void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) const
  {
    glUniform4f(colorLoc, r, g, b, a);
  }
  
  // attribute 変数 pos の場所を得る
  GLint getPositionLoc(void) const
  {
    return positionLoc;
  }
  
} *pointShader = 0;
 
// 後始末
static void cleanup(void)
{
  // 頂点バッファオブジェクトを削除する
  delete pointBuffer;
  
  // プログラムオブジェクトを削除する
  delete pointShader;
}
 
// 初期化処理
static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();
  
  // 頂点バッファオブジェクトを作成する
  pointBuffer = new PointBuffer(pointn);
  
  // プログラムオブジェクト（シェーダ）を作成する
  pointShader = new PointShader();
 
  // OpenGL の初期設定
  glClearColor(1.0, 1.0, 1.0, 1.0);
  
  // 後始末
  atexit(cleanup);
}
 
// 画面に図形を描画する
static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT);
  
  // 点の描画
  pointShader->use();
  pointShader->setColor(0.0f, 0.0f, 0.0f);
  pointBuffer->draw(pointShader->getPositionLoc(), GL_LINE_STRIP);
  pointShader->setColor(1.0f, 0.0f, 0.0f);
  glPointSize(pointSize);
  pointBuffer->draw(pointShader->getPositionLoc(), GL_POINTS);
 
  // ダブルバッファリング
  glutSwapBuffers();
}
 
// ウィンドウの中心位置
static GLfloat cx, cy;
 
// ウィンドウのオープン・リサイズ時の処理
static void resize(int w, int h)
{
  // ウィンドウ全体をビューポート（表示領域）にする
  glViewport(0, 0, w, h);
  
  // ウィンドウの幅と高さを覚えておく
  cx = w * 0.5f;
  cy = h * 0.5f;
}
 
// 押されたマウスボタン
static int pressed;
 
// ドラッグされている点
static int hit = -1;
 
// 何もすることがなくなった時の処理
static void idle(void)
{
  // 画面の再描画を行う
  glutPostRedisplay();
}
 
// マウスのボタンを押したときの処理
static void mouse(int button, int state, int x, int y)
{
  // 押されたボタンを覚えておく
  pressed = button;
  
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
      // 左ボタンを押したときの処理
      hit = pointBuffer->pick((GLfloat)x / cx - 1.0f, 1.0f - (GLfloat)y / cy, pointSize / cx, pointSize / cy);
      if (hit >= 0) glutIdleFunc(idle);
    }
    else
    {
      // 左ボタンを離したときの処理
      pointBuffer->move(hit, (GLfloat)x / cx - 1.0f, 1.0f - (GLfloat)y / cy);
      hit = -1;
      glutIdleFunc(0);
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if (state == GLUT_DOWN)
    {
      // 中ボタンを押したときの処理
    }
    else
    {
      // 中ボタンを離したときの処理
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN)
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
 
// マウスのドラッグ中の処理
static void motion(int x, int y)
{
  switch (pressed)
  {
  case GLUT_LEFT_BUTTON:
    // 左ボタンでドラッグ中の処理
    if (hit >= 0)
    {
      pointBuffer->move(hit, (GLfloat)x / cx - 1.0f, 1.0f - (GLfloat)y / cy);
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    // 中ボタンでドラッグ中の処理
    break;
  case GLUT_RIGHT_BUTTON:
    // 右ボタンでドラッグ中の処理
    break;
  default:
    break;
  }
}
 
// キーボードをタイプしたときの処理
static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case '\033':
  case 'q':
  case 'Q':
    // ESC キー, q, Q をタイプしたら終了する
    exit(0);
  default:
    break;
  }
}
 
// メインプログラム
int main(int argc, char *argv[])
{
  glutInitWindowSize(500, 500);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("physics");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
 
  return 0;
}
