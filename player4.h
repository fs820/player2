//---------------------------------------
//
//プレイヤー4表示処理の定義・宣言[player4.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER4_H_
#define _PLAYER4_H_

#include"main.h"
#include"player.h"

void InitPlayer4(void);//プレイヤー4の初期化処理
void UninitPlayer4(void);//プレイヤー4の終了処理
void UpdatePlayer4(void);//プレイヤー4の更新処理
void DrawPlayer4(void);//プレイヤー4の描画処理
void HitPlayer4(int nDamage);//プレイヤー4ヒット処理
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer4(void);//プレイヤー4バッファの取得
Player* GetPlayer4(void);//プレイヤー4情報取得
bool bOberP4(void);//プレイヤー4が画面にいるか

#endif _PLAYER4_H_