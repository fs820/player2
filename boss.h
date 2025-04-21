//---------------------------------------
//
//ボス表示処理の定義・宣言[boss.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BOSS_H_

#define _BOSS_H_

#include"main.h"

//マクロ定義
#define MAX_BOSS (8)//最大数
#define NUM_BOSS (4)//種類数
#define BOSS_WIDTH (400)//幅
#define BOSS_HEIGHT (360)//高さ
#define BOSS_WIDTH2 (450)//幅
#define BOSS_HEIGHT2 (450)//高さ
#define BOSS_WIDTH3 (500)//幅
#define BOSS_HEIGHT3 (440)//高さ
#define BOSS_WIDTH4 (400)//幅
#define BOSS_HEIGHT4 (400)//高さ

//ボスの状態の列挙型定義
typedef enum
{
	BOSSSTATE_NORMAL = 0,//通常
	BOSSSTATE_DAMAGE,//ダメージ
	BOSSSTATE_MAX,//状態数
}BOSSSTATE;

//ボス構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//動き
	float fSpeed;
	int nType;//種類
	BOSSSTATE state;//状態
	int nCounterState;//ダメージカウンター
	float fLength;//大きさ
	int nLife;//体力
	bool bUse;//使用しているか
}Boss;

void InitBoss(void);//ボスの初期化処理
void UninitBoss(void);//ボスの終了処理
void UpdateBoss(void);//ボスの更新処理
void DrawBoss(void);//ボスの描画処理
void SetBoss(D3DXVECTOR3 pos, int nType);//ボス設置
void HitBoss(int nCntEnemy, int nDamage);//ボスヒット処理
Boss* GetBoss(void);//ボス情報取得
int GetNumBoss(void);//ボスの数取得

#endif _BOSS_H_

