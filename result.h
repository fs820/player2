//------------------------------------------
//
//リザルトの処理の定義・宣言[result.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _RESULT_H_
#define _RESULT_H_

#include"main.h"

#define RESULT_MAX (4)//リザルト最大数

void InitResult(void);//初期化処理
void UninitResult(void);//終了処理
void UpdateResult(void);//更新処理
void DrawResult(void);//描画処理

#endif _RESULT_H_