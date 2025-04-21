//---------------------------------------
//
//弾表示処理の定義・宣言[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"

#define MAX_BULLET (512)//最大数
#define BULLET_WIDTH (25.0f)//大きさ
#define BULLET_HEIGHT (25.0f)//大きさ
#define BOM_SIZE_INA (20.0f)//巨大弾サイズ
#define SP_SIZE_INA (5.0f)//スペシャル砲サイズ
#define BULLET_SPEED (15.0f)//スピード
#define BULLET_SPEED_SP (25.0f)//スピード
#define BULLET_INTER (20)//発射間隔
#define ALL_BULLET (10)//全方位弾弾数
#define ALL_BULLETS (6)//誘導弾弾数
#define ALL_BULLETSP (200)//スペシャル砲弾数
#define BULLET_LIFE (500)//体力
#define HIT_INA (0.25f)

//種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,//プレイヤー1
	BULLETTYPE_PLAYER2,//プレイヤー2
	BULLETTYPE_PLAYER3,//プレイヤー3
	BULLETTYPE_PLAYER4,//プレイヤー4
	BULLETTYPE_ENEMY,//敵
	BULLETTYPE_BOSS,//ボス
	BULLETTYPE_MAX//種類数
}BULLETTYPE;

void InitBullet(void);//弾の初期化処理
void UninitBullet(void);//弾の終了処理
void UpdateBullet(void);//弾の更新処理
void DrawBullet(void);//弾の描画処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLengthPlayer, int nLife, BULLETTYPE type,int nInter);//通常弾
void Set3Bullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//3wey弾
void SetAllBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//全方位弾
void SetSearchBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//誘導弾
void SetBomBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//巨大弾
void SetSPBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//巨大弾

#endif _BULLET_H_
