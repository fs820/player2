//------------------------------------------
//
//ゲームメインの処理の定義・宣言[game.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"

#define INTER (60)//終了猶予
#define SPLIT_INA (0.2f)//画面分割サイズ

//ゲーム状態の列挙型定義
typedef enum
{
	GAMESTATE_NONE=0,//ゲームでない
	GAMESTATE_NORMAL,//ゲーム中
	GAMESTATE_PAUSE,//ポーズ中
	GAMESTATE_END,//終了
	GAMESTATE_MAX//数
}GAMESTATE;

//分割POSの列挙型定義
typedef enum
{
	SENTER = 0,//中央
	LEFTUP,//左上
	RIGHTUP,//右上
	LEFTDOWN,//左下
	RIGHTDOWN,//右下
	POS_MAX
}POS;

void InitGame(void);//ゲームの初期化処理
void UninitGame(void);//ゲームの終了処理
void UpdateGame(void);//ゲームの更新処理
void DrawGame(void);//ゲームの描画処理
void SetGameState(GAMESTATE state);//ゲームの状態設定
GAMESTATE GetGameState(void);//ゲーム状態取得
void SetBossNumber(int Boss);
int GetBossNumber(void);
bool GetClear(void);

#endif _GAME_H_
