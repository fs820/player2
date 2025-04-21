//---------------------------------------
//
//敵表示処理の定義・宣言[enemy.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ENEMY_H_

#define _ENEMY_H_

#include"main.h"

//マクロ定義
#define MAX_ENEMY (128)//敵の最大数
#define NUM_ENEMY (4)//敵の種類
#define ENEMY_WIDTH (200)//幅
#define ENEMY_HEIGHT (200)//高さ
#define ENEMY_WIDTH2 (193)//幅
#define ENEMY_HEIGHT2 (109.5f)//高さ
#define ENEMY_WIDTH3 (246.5f)//幅
#define ENEMY_HEIGHT3 (184.5f)//高さ
#define ENEMY_WIDTH4 (111)//幅
#define ENEMY_HEIGHT4 (139)//高さ

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,//通常
	ENEMYSTATE_DAMAGE,//ダメージ
	ENEMYSTATE_MAX,//数
}ENEMYSTATE;

//敵構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	int nType;//種類
	ENEMYSTATE state;//状態
	int nCounterState;//状態カウンター
	float fLength;//大きさ
	int nLife;//体力
	bool bUse;//つかっているか
}Enemy;

void InitEnemy(void);//敵の初期化処理
void UninitEnemy(void);//敵の終了処理
void UpdateEnemy(void);//敵の更新処理
void DrawEnemy(void);//敵の描画処理
void SetEnemy(D3DXVECTOR3 pos, int nType);//敵設定
void HitEnemy(int nCntEnemy, int nDamage);//敵ヒット
Enemy* GetEnemy(void);//敵情報取得
int GetNumEnemy(void);//敵の数取得

#endif _ENEMY_H_

