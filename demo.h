//------------------------------------------
//
//デモ画面の定義・宣言[demo.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _DEMO_H_
#define _DEMO_H_

#include"game.h"

void InitDemo(void);//デモ初期化処理
void UninitDemo(void);//デモ終了処理
void UpdateDemo(void);//デモ更新処理
void DrawDemo(void);//デモ描画処理
void SetGameStateDemo(GAMESTATE state);//状態設定
GAMESTATE GetGameStateDemo(void);//状態取得

#endif _DEMO_H_