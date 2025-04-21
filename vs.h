//------------------------------------------
//
//対戦メインの処理の定義・宣言[vs.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _VS_H_
#define _VS_H_

#include"main.h"

#define INTER (60)//終了猶予
#define SPLIT_INA_VS (0.5f)//画面分割サイズ

//バーサスの列挙型定義
typedef enum
{
	VSSTATE_NONE = 0,//バーサスでない
	VSSTATE_NORMAL,//VS中
	VSSTATE_PAUSE,//ポーズ中
	VSSTATE_END,//終了
	VSSTATE_MAX//数
}VSSTATE;

void InitVs(void);//バーサス初期化処理
void UninitVs(void);//バーサス終了処理
void UpdateVs(void);//バーサス更新処理
void DrawVs(void);//バーサス描画処理
void SetVsState(VSSTATE state);//バーサス状態設定処理
VSSTATE GetVsState(void);//バーサス状態取得処理

#endif _VS_H_
