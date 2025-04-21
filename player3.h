//---------------------------------------
//
//プレイヤー3表示処理の定義・宣言[player3.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER3_H_
#define _PLAYER3_H_

#include"main.h"
#include"player.h"

void InitPlayer3(void);//プレイヤー3の初期化処理
void UninitPlayer3(void);//プレイヤー3の終了処理
void UpdatePlayer3(void);//プレイヤー3の更新処理
void DrawPlayer3(void);//プレイヤー3の描画処理
void HitPlayer3(int nDamage);//プレイヤー3ヒット処理
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer3(void);//プレイヤー3バッファの取得
Player* GetPlayer3(void);//プレイヤー3情報取得
bool bOberP3(void);//プレイヤー3が画面にいるか

#endif _PLAYER3_H_