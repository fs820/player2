//------------------------------------------
//
//チュートリアルの定義・宣言[tuto.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _TUTO_H_
#define _TUTO_H_

#include"game.h"

void InitTuto(void);//チュートリアル初期化処理
void UninitTuto(void);//チュートリアル終了処理
void UpdateTuto(void);//チュートリアル更新処理
void DrawTuto(void);//チュートリアル描画処理
void SetTutoState(GAMESTATE state);//チュートリアル状態設定処理
GAMESTATE GetTutoState(void);//チュートリアル状態取得処理

#endif _TUTO_H_