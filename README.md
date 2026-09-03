# string1

## 1. 概要

ゴムシミュレータ（第1回）の完成版プログラムです。頂点バッファオブジェクト (VBO) とシェーダ (GLSL) を用いて質点群およびそれらを結ぶ線分を描画し、マウスドラッグにより質点を対話的に選択・移動できます。

本プログラムは、以下のブログ記事の解説のために提供されています。

- [ひものシミュレーション 2](https://tokoik.github.io/blog/2011/12/07/seminar.html)


## 動作環境

- CMake 3.22 以上
- C++17 に対応した C++ コンパイラ (MSVC, GCC, Clang など)
- OpenGL 2.1 以上 (GLSL 1.20 以上)
- 依存ライブラリ: GLEW, FreeGLUT (CMake の FetchContent により自動取得されます)

## ビルド手順

### Windows (Visual Studio)

```powershell
cmake -B build
cmake --build build --config Release
```

### Linux / macOS

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## 起動方法

ビルド完了後、実行ファイルと同じディレクトリに必要なシェーダファイル（`point.vert`, `point.frag`, `force.vert`）が自動的にコピーされます。

### Windows

```powershell
.\build\Release\string1.exe
```

### Linux / macOS

```bash
./build/string1
```

## 操作方法

- **マウス左ドラッグ**: 点の選択および位置の移動
- **q / Q / Esc**: プログラムの終了
