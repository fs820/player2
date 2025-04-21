//------------------------------------
//
//プレイヤー方向マーク処理の定義・宣言[mark.h]
//Author fuma sato
//
//------------------------------------
#include"main.h"

#define VS_MARK_INA (0.02f)
#define VS_MARK_SIZE_INA (0.2f)

void InitMark(void);//マークの初期化処理
void UninitMark(void);//マークの終了処理
void UpdateMark(void);//マークの更新処理
void DrawMark(void);//マークの描画処理