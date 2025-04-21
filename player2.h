//---------------------------------------
//
//プレイヤー2表示処理の定義・宣言[player2.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER2_H_
#define _PLAYER2_H_

#include"main.h"
#include"player.h"

void InitPlayer2(void);//プレイヤー2の初期化処理
void UninitPlayer2(void);//プレイヤー2の終了処理
void UpdatePlayer2(void);//プレイヤー2の更新処理
void DrawPlayer2(void);//プレイヤー2の描画処理
void HitPlayer2(int nDamage);//プレイヤー2ヒット処理
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer2(void);//プレイヤー2バッファの取得
Player* GetPlayer2(void);//プレイヤー2情報取得
bool bOberP2(void);//プレイヤー2が画面にいるか

#endif _PLAYER2_H_