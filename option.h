//------------------------------------------
//
//オプションの処理の定義・宣言[option.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _OPTION_H_
#define _OPTION_H_

#include"main.h"

#define OPTION_MAX (10)//オプション数

void InitOption(void);//初期化処理
void UninitOption(void);//終了処理
void UpdateOption(void);//更新処理
void DrawOption(void);//描画処理

#endif _OPTION_H_