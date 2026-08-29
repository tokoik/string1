/*
** ゲームグラフィックス特論用補助プログラム
**

Copyright (c) 2011 Kohe Tokoi. All Rights Reserved.

Permission is hereby granted, free of charge,  to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction,  including without limitation the rights 
to use, copy,  modify, merge,  publish, distribute,  sublicense,  and/or sell 
copies or substantial portions of the Software.

The above  copyright notice  and this permission notice  shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR 
IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO THE WARRANTIES  OF MERCHANTABILITY, 
FITNESS  FOR  A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN  NO EVENT  SHALL 
KOHE TOKOI  BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY,  WHETHER IN 
AN ACTION  OF CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM,  OUT OF  OR  IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**
*/

#ifndef __GG_H__
#define __GG_H__

#include <cstring>
#include <cmath>

#if defined(_WIN32)
#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#  endif
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN 1
#  endif
#  include <windows.h>
#endif

#include <GL/glew.h>
#if defined(__APPLE__)
#  define GL_SILENCE_DEPRECATION
#  include <GLUT/glut.h>
#else
#  include <GL/freeglut.h>
#endif

#define GG_DEG_TO_RAD(a) ((a) * 0.017453293f)

namespace gg
{
  /*
  ** ゲームグラフィックス特論の都合にもとづく初期化
  **
  **    OpenGL のレンダリングコンテキストを作成後に実行する
  */
  extern void ggInit(void);

  /*
  ** OpenGL のエラーチェック
  **
  **    OpenGL の API を呼び出し直後に実行すればエラーのあるときにメッセージを表示する
  **    msg はメッセージに乗じる文字列
  */
  extern void ggError(const char *msg = 0);

  /*
  ** FBO のエラーチェック
  **
  **    FBO の API を呼び出し直後に実行すればエラーのあるときにメッセージを表示する
  **    msg はメッセージに乗じる文字列
  */
  extern void ggFBOError(const char *msg = 0);

  /*
  ** 変換行列
  */
  class GgMatrix
  {
    // 変換行列の要素
    GLfloat array[16];

    // 行列 a とベクトル b の積をベクトル c に代入する
    void projection(GLfloat *c, const GLfloat *a, const GLfloat *b) const;

    // 行列 a と行列 b の積を行列 c に代入する
    void multiply(GLfloat *c, const GLfloat *a, const GLfloat *b) const;

  public:

    // デストラクタ
    virtual ~GgMatrix(void) {}

    // コンストラクタ
    GgMatrix(void) {}
    GgMatrix(const GLfloat *a)
    {
      load(a);
    }
    GgMatrix(const GgMatrix &m)
    {
      load(m);
    }

    // 演算子
    GgMatrix &multiply(const GgMatrix &m, const GgMatrix &n)
    {
      multiply(array, m.array, n.array);
      return *this;
    }
    GgMatrix &multiply(const GgMatrix &m, const GLfloat *a)
    {
      multiply(array, m.array, a);
      return *this;
    }
    GgMatrix &multiply(const GLfloat *a)
    {
      GLfloat t[16]; multiply(t, array, a);
      memcpy(array, t, sizeof array); return *this;
    }
    GgMatrix &multiply(const GgMatrix &m)
    {
      return multiply(m.array);
    }
    GgMatrix &operator=(const GLfloat *a)
    {
      return load(a);
    }
    GgMatrix &operator=(const GgMatrix &m)
    {
      return load(m);
    }
    GgMatrix operator*(const GLfloat *a) const
    {
      GgMatrix t;
      t.multiply(t.array, this->array, a);
      return t;
    }
    GgMatrix operator*(const GgMatrix &m) const
    {
      GgMatrix t;
      t.multiply(t.array, this->array, m.array);
      return t;
    }
    GgMatrix &operator*=(const GLfloat *a)
    {
      return multiply(a);
    }
    GgMatrix &operator*=(const GgMatrix &m)
    {
      return multiply(m);
    }

    // 変換行列の読み込み
    GgMatrix &load(const GLfloat *a)
    {
      memcpy(array, a, sizeof array);
      return *this;
    }
    GgMatrix &load(const GgMatrix &m)
    {
      return load(m.array);
    }

    // 単位行列を設定する
    GgMatrix &loadIdentity(void);

    // 平行移動の変換行列を設定する
    GgMatrix &loadTranslate(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
    GgMatrix &loadTranslate(const GLfloat *t)
    {
      return loadTranslate(t[0], t[1], t[2], t[3]);
    }

    // 拡大縮小の変換行列を設定する
    GgMatrix &loadScale(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
    GgMatrix &loadScale(const GLfloat *s)
    {
      return loadScale(s[0], s[1], s[2], s[3]);
    }

    // 回転の変換行列を設定する
    GgMatrix &loadRotateX(GLfloat a);
    GgMatrix &loadRotateY(GLfloat a);
    GgMatrix &loadRotateZ(GLfloat a);
    GgMatrix &loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a);
    GgMatrix &loadRotate(const GLfloat *r)
    {
      return loadRotate(r[0], r[1], r[2], r[3]);
    }

    // ビュー変換行列を設定する
    GgMatrix &loadLookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz);
    GgMatrix &loadLookat(const GLfloat *e, const GLfloat *t, const GLfloat *u)
    {
      return loadLookat(e[0], e[1], e[2], t[0], t[1], t[2], u[0], u[1], u[2]);
    }

    // 直交投影変換行列を設定する
    GgMatrix &loadOrthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

    // 透視透視投影変換行列を設定する
    GgMatrix &loadFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

    // 画角を指定して透視投影変換行列を設定する
    GgMatrix &loadPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

    // 転置行列を設定する
    GgMatrix &loadTranspose(const GgMatrix &m);

    // 逆行列を設定する
    GgMatrix &loadInvert(const GgMatrix &m);

    // 法線変換行列を設定する
    GgMatrix &loadNormal(const GgMatrix &m);

    // 平行移動変換を乗じる
    GgMatrix &translate(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f)
    {
      GgMatrix m; m.loadTranslate(x, y, z, w); multiply(m);
      return *this;
    }
    GgMatrix &translate(const GLfloat *t)
    {
      return translate(t[0], t[1], t[2], t[3]);
    }

    // 拡大縮小変換を乗じる
    GgMatrix &scale(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f)
    {
      GgMatrix m;
      m.loadScale(x, y, z, w);
      multiply(m);
      return *this;
    }
    GgMatrix &scale(const GLfloat *s)
    {
      return scale(s[0], s[1], s[2], s[3]);
    }

    // 回転変換を乗じる
    GgMatrix &rotateX(GLfloat a)
    {
      GgMatrix m;
      m.loadRotateX(a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotateY(GLfloat a)
    {
      GgMatrix m;
      m.loadRotateY(a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotateZ(GLfloat a)
    {
      GgMatrix m;
      m.loadRotateZ(a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
    {
      GgMatrix m;
      m.loadRotate(x, y, z, a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotate(const GLfloat *r)
    {
      return rotate(r[0], r[1], r[2], r[3]);
    }

    // ビュー変換を乗じる
    GgMatrix &lookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz);
    GgMatrix &lookat(const GLfloat *e, const GLfloat *t, const GLfloat *u)
    {
      return lookat(e[0], e[1], e[2], t[0], t[1], t[2], u[0], u[1], u[2]);
    }

    // 直交投影変換を乗じる
    GgMatrix &orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

    // 透視投影変換を乗じる
    GgMatrix &frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

    // 画角を指定して透視投影変換を乗じる
    GgMatrix &perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

    // 転置行列を得る
    GgMatrix transpose(void) const
    {
      GgMatrix t;
      return t.loadTranspose(*this);
    }

    // 逆行列を得る
    GgMatrix invert(void) const
    {
      GgMatrix t;
      return t.loadInvert(*this);
    }

    // 法線変換行列を得る
    GgMatrix normal(void) const
    {
      GgMatrix t;
      return t.loadNormal(*this);
    }

    // ベクトルに対して投影変換を行う
    void projection(GLfloat *c, const GLfloat *v) const
    {
      projection(c, array, v);
    }

    // 変換行列を得る
    const GLfloat *get(void) const
    {
      return array;
    }
  };

  /*
  ** 四元数
  */
  class GgQuaternion
  {
    // 四元数の要素
    GLfloat array[4];

    // 四元数 p と四元数 q の和を四元数 r に求める
    void add(GLfloat *r, const GLfloat *p, const GLfloat *q) const;

    // 四元数 p と四元数 q の差を四元数 r に求める
    void subtract(GLfloat *r, const GLfloat *p, const GLfloat *q) const;

    // 四元数 p と四元数 q の積を四元数 r に求める
    void multiply(GLfloat *r, const GLfloat *p, const GLfloat *q) const;

    // 四元数 q が表す回転の変換行列を m に求める
    void toMatrix(GLfloat *m, const GLfloat *q) const;

    // 回転の変換行列 m が表す四元数を q に求める
    void toQuaternion(GLfloat *q, const GLfloat *m) const;

    // 球面線形補間 q と r を t で補間した四元数を p に求める
    void slerp(GLfloat *p, const GLfloat *q, const GLfloat *r, GLfloat t) const;

  public:

    // デストラクタ
    virtual ~GgQuaternion(void) {}

    // コンストラクタ
    GgQuaternion(void) {}
    GgQuaternion(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
      load(x, y, z, w);
    }
    GgQuaternion(const GLfloat *a)
    {
      load(a);
    }
    GgQuaternion(const GgQuaternion &q)
    {
      load(q);
    }

    // 演算子
    GgQuaternion &operator=(const GLfloat *a)
    {
      return load(a);
    }
    GgQuaternion &operator=(const GgQuaternion &q)
    {
      return load(q);
    }
    GgQuaternion operator+(const GLfloat *a) const
    {
      GgQuaternion t;
      t.add(t.array, this->array, a);
      return t;
    }
    GgQuaternion operator+(const GgQuaternion &q) const
    {
      GgQuaternion t;
      t.add(t.array, this->array, q.array);
      return t;
    }
    GgQuaternion &operator+=(const GLfloat *a)
    {
      return add(a);
    }
    GgQuaternion &operator+=(const GgQuaternion &q)
    {
      return add(q);
    }
    GgQuaternion operator-(const GLfloat *a) const
    {
      GgQuaternion t;
      t.subtract(t.array, this->array, a);
      return t;
    }
    GgQuaternion operator-(const GgQuaternion &q) const
    {
      GgQuaternion t;
      t.subtract(t.array, this->array, q.array);
      return t;
    }
    GgQuaternion &operator-=(const GLfloat *a)
    {
      return subtract(a);
    }
    GgQuaternion &operator-=(const GgQuaternion &q)
    {
      return subtract(q);
    }
    GgQuaternion operator*(const GLfloat *a) const
    {
      GgQuaternion t;
      t.multiply(t.array, this->array, a);
      return t;
    }
    GgQuaternion operator*(const GgQuaternion &q) const
    {
      GgQuaternion t;
      t.multiply(t.array, this->array, q.array);
      return t;
    }
    GgQuaternion &operator*=(const GLfloat *a)
    {
      return multiply(a);
    }
    GgQuaternion &operator*=(const GgQuaternion &q)
    {
      return multiply(q);
    }

    // 四元数を設定する
    GgQuaternion &load(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
      array[0] = x;
      array[1] = y;
      array[2] = z;
      array[3] = w;
      return *this;
    }
    GgQuaternion &load(const GLfloat *a)
    {
      return load(a[0], a[1], a[2], a[3]);
    }
    GgQuaternion &load(const GgQuaternion &q)
    {
      return load(q.array);
    }

    // 回転の変換行列 m を表す四元数を設定する
    GgQuaternion &loadMatrix(const GLfloat *m)
    {
      toQuaternion(array, m); return *this;
    }

    // 単位元を設定する
    GgQuaternion &loadIdentity(void)
    {
      return load(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // (x, y, z) を軸として角度 a 回転する資源数を設定する
    GgQuaternion &loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a);
    GgQuaternion &loadRotate(const GLfloat *v, GLfloat a)
    {
      return loadRotate(v[0], v[1], v[2], a);
    }

    // オイラー角 (h, p, r) で与えられた回転を表す四元数を設定する
    GgQuaternion &loadEuler(GLfloat h, GLfloat p, GLfloat r);
    GgQuaternion &loadEuler(const GLfloat *e)
    {
      return loadEuler(e[0], e[1], e[2]);
    }

    // 四元数の和を求める
    GgQuaternion &add(const GgQuaternion &p, const GgQuaternion &q)
    {
      add(array, p.array, q.array);
      return *this;
    }
    GgQuaternion &add(const GgQuaternion &q, const GLfloat *a)
    {
      add(array, q.array, a);
      return *this;
    }
    GgQuaternion &add(const GLfloat *a)
    {
      GLfloat t[4];
      add(t, array, a);
      return load(t);
    }
    GgQuaternion &add(const GgQuaternion &q)
    {
      return add(q.array);
    }

    // 四元数の差を求める
    GgQuaternion &subtract(const GgQuaternion &p, const GgQuaternion &q)
    {
      subtract(array, p.array, q.array);
      return *this;
    }
    GgQuaternion &subtract(const GgQuaternion &q, const GLfloat *a)
    {
      subtract(array, q.array, a);
      return *this;
    }
    GgQuaternion &subtract(const GLfloat *a)
    {
      GLfloat t[4];
      subtract(t, array, a);
      return load(t);
    }
    GgQuaternion &subtract(const GgQuaternion &q)
    {
      return subtract(q.array);
    }

    // 四元数の積を求める
    GgQuaternion &multiply(const GgQuaternion &p, const GgQuaternion &q)
    {
      multiply(array, p.array, q.array);
      return *this;
    }
    GgQuaternion &multiply(const GgQuaternion &q, const GLfloat *a)
    {
      multiply(array, q.array, a);
      return *this;
    }
    GgQuaternion &multiply(const GLfloat *a)
    {
      GLfloat t[4]; multiply(t, array, a);
      return load(t);
    }
    GgQuaternion &multiply(const GgQuaternion &q)
    {
      return multiply(q.array);
    }

    // 球面線形補間
    GgQuaternion &slerp(const GgQuaternion &q, const GgQuaternion &r, GLfloat t)
    {
      slerp(array, q.array, r.array, t);
      return *this;
    }
    GgQuaternion &slerp(const GgQuaternion &q, const GLfloat *b, GLfloat t)
    {
      slerp(array, q.array, b, t);
      return *this;
    }
    GgQuaternion &slerp(const GLfloat *a, const GgQuaternion &r, GLfloat t)
    {
      slerp(array, a, r.array, t);
      return *this;
    }
    GgQuaternion &slerp(const GLfloat *a, const GLfloat *b, GLfloat t)
    {
      slerp(array, a, b, t);
      return *this;
    }
    GgQuaternion &slerp(const GgQuaternion &r, GLfloat t)
    {
      slerp(array, array, r.array, t);
      return *this;
    }
    GgQuaternion &slerp(GLfloat *b, GLfloat t)
    {
      slerp(array, array, b, t);
      return *this;
    }

    // 四元数のノルムを求める
    GLfloat norm(void) const;

    // 共役四元数を求める
    GgQuaternion conjugate(void) const;

    // 四元数の逆を求める
    GgQuaternion invert(void) const;

    // 四元数を正規化する
    GgQuaternion normalize(void) const;

    // 四元数を求める
    const GLfloat *get(void) const
    {
      return array;
    }
    void get(GLfloat *a) const
    {
      a[0] = array[0];
      a[1] = array[1];
      a[2] = array[2];
      a[3] = array[3];
    }

    // 四元数が表す回転の行列を m に求める
    void getMatrix(GLfloat *m) const
    {
      toMatrix(m, array);
    }
  };

  /*
  ** テクスチャ
  **
  **    拡散反射色テクスチャを読み込んでテクスチャオブジェクトを作成する
  */
  class GgTexture
  {
    // テクスチャ名
    GLuint texture;

  public:

    // デストラクタ
    virtual ~GgTexture(void)
    {
      glDeleteTextures(1, &texture);
    }

    // コンストラクタ
    GgTexture(void)
    {
      glGenTextures(1, &texture);
    }

    // コンストラクタ
    GgTexture(
      const char *filename,               // 画像ファイル名（3/4 チャネルの RAW 画像）
      int width, int height,              // 画像の幅と高さ（2^n 画素）
      GLenum format = GL_RGB              // 読み込む画像の書式 (GL_RGB/GL_RGBA)
      )
    {
      glGenTextures(1, &texture);
      load(filename, width, height, format);
    }

    // コピーコンストラクタ
    GgTexture(const GgTexture &o)
      : texture(o.texture) {}

    // 代入演算子
    GgTexture &operator=(const GgTexture &o)
    {
      if (this != &o)
      {
        glDeleteTextures(1, &texture);
        texture = o.texture;
      }

      return *this;
    }

    // 拡散反射色テクスチャを読み込む
    //    filename: ファイル名, width, height: 幅と高さ (2^n), format: GL_RGB か GL_RGBA
    void load(const char *filename, int width, int height, GLenum format = GL_RGB) const;

    // テクスチャオブジェクトを使用する
    void bind(void) const
    {
      glBindTexture(GL_TEXTURE_2D, texture);
    }
    
    // テクスチャオブジェクト名を取得する
    GLuint tex(void) const
    {
      return texture;
    }
  };

  /*
  ** 法線マップ
  **
  **    高さマップ（グレイスケール画像）を読み込んで法線マップを作成する
  */
  class GgNormalTexture
    : public GgTexture
  {
  public:

    // デストラクタ
    virtual ~GgNormalTexture(void) {}

    // コンストラクタ
    GgNormalTexture(void) {}

    // コンストラクタ
    GgNormalTexture(
      const char *filename,               // 画像ファイル名（1 チャネルの RAW 画像）
      int width, int height,              // 画像の幅と高さ（2^n 画素）
      float nz = 1.0f                     // 法線マップの z 成分の値
      ) : GgTexture()                     // テクスチャオブジェクトを作成する
    {
      load(filename, width, height, nz);
    }

    // コピーコンストラクタ
    GgNormalTexture(const GgNormalTexture &o)
      : GgTexture(o) {}

    // 代入演算子
    GgNormalTexture &operator=(const GgNormalTexture &o)
    {
      if (this != &o)
      {
        GgTexture::operator=(o);
      }

      return *this;
    }

    // 高さマップを読み込んで法線マップを作成する
    //    filename: ファイル名, width, height: 幅と高さ (2^n), nz: 法線マップの z 成分の値
    void load(const char *filename, int width, int height, float nz = 1.0f) const;
  };

  /*
  ** シェーダ
  **
  **    シェーダの基底クラス
  */
  class GgShader
  {
    // プログラム名
    GLuint program;

    // シェーダのソースプログラムを読み込む
    bool readShaderSource(GLuint shader, const char *filename) const;

    // シェーダのソースプログラムのコンパイル情報を表示する
    void printShaderInfoLog(GLuint shader) const;

    // シェーダのオブジェクトプログラムのリンク情報を表示する
    void printProgramInfoLog(GLuint program) const;

  public:

    // デストラクタ
    virtual ~GgShader(void)
    {
      glDeleteProgram(program);
    }

    // コンストラクタ
    GgShader(void)
      : program(glCreateProgram()) {}

    // コンストラクタ
    GgShader(
      const char *vert,                   // バーテックスシェーダのソースファイル名
      const char *frag = 0,               // フラグメントシェーダのソースファイル名（0 なら不使用）
      const char *geom = 0,               // ジオメトリシェーダのソースファイル名（0 なら不使用）
      GLenum input = GL_TRIANGLES,        // ジオメトリシェーダの入力プリミティブ
      GLenum output = GL_TRIANGLE_STRIP,  // ジオメトリシェーダの出力プリミティブ
      int nvarying = 0,                   // フィードバックする varying 変数の数（0 なら不使用）
      const char **varyings = 0           // フィードバックする varying 変数のリスト
      )
      : program(glCreateProgram())
    {
      load(vert, frag, geom, input, output, nvarying, varyings);
    }

    // コピーコンストラクタ
    GgShader(const GgShader &o)
      : program(o.program) {}

    // 代入演算子
    GgShader &operator=(const GgShader &o)
    {
      if (this != &o)
      {
        glDeleteProgram(program);
        program = o.program;
      }

      return *this;
    }

    // シェーダのソースプログラムを読み込んでコンパイル・リンクしてプログラムオブジェクトを作成する
    bool load(const char *vert, const char *frag = 0, const char *geom = 0,
      GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP,
      int nvarying = 0, const char **varyings = 0) const;

    // プログラムオブジェクトを使用する
    void use(void) const
    {
      glUseProgram(program);
    }
    
    // プログラムオブジェクト名を取得する
    GLuint getProgram(void) const
    {
      return program;
    }
  };

  /*
  ** バッファオブジェクト
  **
  **    頂点／インデックスバッファオブジェクトの基底クラス
  */
  template <typename T> class GgBuffer
  {
    // バッファオブジェクト
    GLuint buffer;

    // データ数
    unsigned int number;

  public:

    // デストラクタ
    virtual ~GgBuffer<T>(void)
    {
      glDeleteBuffers(1, &buffer);
    }

    // コンストラクタ
    GgBuffer<T>(void)
      : number(0)
    {
      glGenBuffers(1, &buffer);
    }

    // コンストラクタ
    GgBuffer<T>(GLenum target, unsigned int n, const T *data, GLenum usage = GL_STATIC_DRAW)
    {
      glGenBuffers(1, &buffer);
      load(target, n, data, usage);
    }

    // コピーコンストラクタ
    GgBuffer<T>(const GgBuffer<T> &o)
    {
      buffer = o.buffer;
      number = o.number;
    }

    // 代入演算子
    GgBuffer<T> &operator=(const GgBuffer<T> &o)
    {
      if (this != &o)
      {
        glDeleteBuffers(1, &buffer);
        buffer = o.buffer;
        number = o.number;
      }

      return *this;
    }

    // データの取得
    void load(GLenum target, unsigned int n, const T *data, GLenum usage = GL_STATIC_DRAW)
    {
      number = n;
      glBindBuffer(target, buffer);
      glBufferData(target, sizeof (T) * n, data, usage);
    }

    // バッファオブジェクト名を得る
    GLuint buf(void) const
    {
      return buffer;
    }

    // データの数を得る
    unsigned int num(void) const
    {
      return number;
    }
  };

  /*
  ** 点群
  **
  **    頂点バッファオブジェクトを使用する形状データのための基底クラス
  */
  class GgPoints
  {
    // 頂点バッファオブジェクト
    GgBuffer<GLfloat[3]> position;
    
  protected:

    // バッファオブジェクト名を得る
    GLuint pbuf(void) const
    {
      return position.buf();
    }

    // データの数を得る
    unsigned int pnum(void) const
    {
      return position.num();
    }

  public:
    
    // デストラクタ
    virtual ~GgPoints(void) {}
    
    // コンストラクタ
    GgPoints(void) {}
    
    // コンストラクタ
    //    n: 頂点数, pos: 頂点の位置
    GgPoints(int n, const GLfloat (*pos)[3], GLenum usage = GL_STATIC_DRAW)
    {
      load(n, pos, usage);
    }

    // コピーコンストラクタ
    GgPoints(const GgPoints &o)
      : position(o.position) {}

    // 代入演算子
    GgPoints &operator=(const GgPoints &o)
    {
      if (this != &o)
      {
        position = o.position;
      }

      return *this;
    }
    
    // バッファオブジェクトを確保して頂点を格納する
    //    n: 頂点数, pos: 頂点の位置
    void load(int n, const GLfloat (*pos)[3], GLenum usage = GL_STATIC_DRAW)
    {
      position.load(GL_ARRAY_BUFFER, n, pos, usage);
    }

    // 点群を描画する手続き
    virtual void draw(GLint pvLoc, GLenum mode = GL_POINTS);
  };
  
  /*
  ** 三角形群
  **
  **    頂点属性のみで表された図形のための基底クラス
  */
  class GgTriangles
    : public GgPoints
  {
    // 法線の頂点バッファオブジェクト
    GgBuffer<GLfloat[3]> normal;

    // 頂点属性の場所
    GLint location;

  protected:

    // バッファオブジェクト名を得る
    GLuint nbuf(void) const
    {
      return normal.buf();
    }

    // データの数を得る
    unsigned int nnum(void) const
    {
      return normal.num();
    }

  public:

    // デストラクタ
    virtual ~GgTriangles(void) {}

    // コンストラクタ
    GgTriangles(void) {}

    // コンストラクタ
    //    n: 頂点数, pos: 頂点の位置, norm: 頂点の法線
    GgTriangles(int n, const GLfloat (*pos)[3], const GLfloat (*norm)[3], GLenum usage = GL_STATIC_DRAW)
      : GgPoints(n, pos, usage)
    {
      normal.load(GL_ARRAY_BUFFER, n, norm, usage);
    }

    // コピーコンストラクタ
    GgTriangles(const GgTriangles &o)
      : GgPoints(o), normal(o.normal) {}

    // 代入演算子
    GgTriangles &operator=(const GgTriangles &o)
    {
      if (this != &o)
      {
        GgPoints::operator=(o);
        normal = o.normal;
      }

      return *this;
    }
    
    // バッファオブジェクトを確保して位置と法線を格納する
    //    n: 頂点数, pos: 頂点の位置, norm: 頂点の法線
    void load(int n, const GLfloat (*pos)[3], const GLfloat (*norm)[3], GLenum usage = GL_STATIC_DRAW)
    {
      GgPoints::load(n, pos, usage);
      normal.load(GL_ARRAY_BUFFER, n, norm, usage);
    }

    // 三角形群を描画する手続き
    virtual void draw(GLint pvLoc, GLint nvLoc, GLenum mode = GL_TRIANGLES);
  };

  /*
  ** 矩形
  **
  **    xy 平面上の矩形の形状データ
  */
  class GgRectangle
    : public GgTriangles
  {
  public:
    
    // デストラクタ
    virtual ~GgRectangle(void) {}
    
    // コンストラクタ
    GgRectangle(GLfloat w = 2.0f, GLfloat h = 2.0f)
    {
      resize(w, h);
    }
    
    // コピーコンストラクタ
    GgRectangle(const GgRectangle &o)
      : GgTriangles(o) {}
    
    // 代入演算子
    GgRectangle &operator=(const GgRectangle &o)
    {
      if (this != &o)
      {
        GgTriangles::operator=(o);
      }

      return *this;
    }
    
    // 矩形のサイズを変更する
    //    w, h: 矩形の幅と高さ
    void resize(GLfloat w, GLfloat h);

    // 三角形群を描画する手続き
    virtual void draw(GLint pvLoc, GLint nvLoc, GLenum mode = GL_TRIANGLE_FAN)
    {
      GgTriangles::draw(pvLoc, nvLoc, mode);
    }
  };

  /*
  ** 楕円形
  **
  **    三角形の形状データで表した xy 平面上の楕円形
  */
  class GgEllipse
    : public GgTriangles
  {
  public:

    // デストラクタ
    virtual ~GgEllipse(void) {}

    // コンストラクタ
    GgEllipse(GLfloat w = 2.0f, GLfloat h = 2.0f, int slices = 16)
    {
      resize(w, h, slices);
    }

    // コピーコンストラクタ
    GgEllipse(const GgEllipse &o)
      : GgTriangles(o) {}

    // 代入演算子
    GgEllipse &operator=(const GgEllipse &o)
    {
      if (this != &o)
      {
        GgTriangles::operator=(o);
      }

      return *this;
    }

    // 楕円形のサイズと分割数を変更する
    //    w: 楕円形の幅, h: 楕円形の高さ, slices: 辺の数（分割数）
    void resize(GLfloat w, GLfloat h, int slices = 16);

    // 三角形群を描画する手続き
    virtual void draw(GLint pvLoc, GLint nvLoc, GLenum mode = GL_TRIANGLE_FAN)
    {
      GgTriangles::draw(pvLoc, nvLoc, mode);
    }
  };

  /*
  ** 三角形メッシュ
  **
  **    頂点インデックスを使用する図形のための基底クラス
  */
  class GgMesh
    : public GgTriangles
  {
    // インデックスバッファオブジェクト
    GgBuffer<GLuint[3]> index;

  protected:

    // バッファオブジェクト名を得る
    GLuint fbuf(void) const
    {
      return index.buf();
    }

    // データの数を得る
    unsigned int fnum(void) const
    {
      return index.num();
    }

  public:

    // デストラクタ
    virtual ~GgMesh(void) {}

    // コンストラクタ
    GgMesh(void) {}

    // コンストラクタ
    //    n: 頂点数, pos: 頂点の位置, norm:頂点の法線
    //    f: 面数, face: 頂点のインデックスデータ
    GgMesh(int n, int f, const GLfloat (*pos)[3], const GLfloat (*norm)[3],
      const GLuint (*face)[3], GLenum usage = GL_STATIC_DRAW)
      : GgTriangles(n, pos, norm, usage)
    {
      index.load(GL_ELEMENT_ARRAY_BUFFER, f, face);
    }

    // コピーコンストラクタ
    GgMesh(const GgMesh &o)
      : GgTriangles(o), index(o.index) {}

    // 代入演算子
    GgMesh &operator=(const GgMesh &o)
    {
      if (this != &o)
      {
        GgTriangles::operator=(o);
        index = o.index;
      }

      return *this;
    }

    // バッファオブジェクトを確保して位置と法線とインデックスを格納する
    //    n: 頂点数, pos: 頂点の位置, norm:頂点の法線
    //    f: 面数, face: 頂点のインデックスデータ
    void load(int n, int f, const GLfloat (*pos)[3], const GLfloat (*norm)[3],
      const GLuint (*face)[3], GLenum usage = GL_STATIC_DRAW)
    {
      GgTriangles::load(n, pos, norm, usage);
      index.load(GL_ELEMENT_ARRAY_BUFFER, f, face);
    }

    // 三角形メッシュを描画する手続き
    virtual void draw(GLint pvLoc, GLint nvLoc, GLenum mode = GL_TRIANGLES);
  };

  /*
  ** 球
  **
  **    三角形の形状データで表した原点にある球
  */
  class GgSphere
    : public GgMesh
  {
  public:

    // デストラクタ
    virtual ~GgSphere(void) {}

    // コンストラクタ
    //    サイズを指定して球のインスタンスを作成する
    //    diameter: 直径, slices: 経度方向の分割数, stacks: 緯度方向の分割数
    GgSphere(GLfloat diameter = 1.0f, int slices = 16, int stacks = 8)
    {
      resize(diameter, slices, stacks);
    }

    // コピーコンストラクタ
    GgSphere(const GgSphere &o)
      : GgMesh(o) {}
    
    // 代入演算子
    GgSphere &operator=(const GgSphere &o)
    {
      if (this != &o)
      {
        GgMesh::operator=(o);
      }
      
      return *this;
    }

    // 球のサイズと分割数を変更する
    //    diameter: 直径, slices: 経度方向の分割数, stacks: 緯度方向の分割数
    void resize(GLfloat diameter = 1.0f, int slices = 16, int stacks = 8);
  };
  
  /*
  ** 三角形分割された Alias OBJ 形式のデータ
  **
  **    三角形分割された Alias OBJ 形式のデータによる三角形の形状データ
  */
  class GgObj
    : public GgMesh
  {
  public:

    // デストラクタ
    virtual ~GgObj(void) {}

    // コンストラクタ
    GgObj(void) {}

    // コンストラクタ
    //    Alias OBJ 形式のファイルを読み込んでインスタンスを作成する
    //    filename: ファイル名, scale: 0 でなければ中心を原点にして各軸方向の大きさの最大値を scale にする
    GgObj(const char *filename, GLfloat scale = 0.0f)
    {
      loadfile(filename, scale);
    }

     // コピーコンストラクタ
    GgObj(const GgObj &o)
      : GgMesh(o) {}

    // 代入演算子
    GgObj &operator=(const GgObj &o)
    {
      if (this != &o)
      {
        GgMesh::operator=(o);
      }
      
      return *this;
    }
    
    // Alias OBJ 形式のファイルを読み込む
    //    filename: ファイル名, scale: 0 でなければ中心を原点にして各軸方向の大きさの最大値を scale にする
    bool loadfile(const char *filename, GLfloat scale = 0.0f);
  };

  /*
  ** 簡易トラックボール処理
  */
  class GgTrackball
  {
    int cx, cy;       // ドラッグ開始位置
    bool drag;        // ドラッグ中か否か
    float sx, sy;     // マウスの絶対位置→ウィンドウ内での相対位置の換算係数
    GgQuaternion cq;  // 回転の初期値 (四元数)
    GgQuaternion tq;  // ドラッグ中の回転 (四元数)
    GLfloat rt[16];   // 回転の変換行列

  public:

    // デストラクタ
    ~GgTrackball(void) {}

    // コンストラクタ
    GgTrackball(void);

    // トラックボール処理の範囲指定
    //    ウィンドウのリサイズ時に呼び出す
    void region(int w, int h);

    // トラックボール処理の開始
    //    マウスのドラッグ開始時（マウスボタンを押したとき）に呼び出す
    void start(int x, int y);

    // 回転の変換行列の計算
    //    マウスのドラッグ中に呼び出す
    void motion(int x, int y);

    // トラックボール処理の停止
    //    マウスのドラッグ終了時（マウスボタンを離したとき）に呼び出す
    void stop(int x, int y);

    // 現在の回転の変換行列を得る
    const GLfloat *get(void) const
    {
      return rt;
    }
  };
}

#endif