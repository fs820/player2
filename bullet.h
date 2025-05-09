//---------------------------------------
//
//e\¦Ìè`Eé¾[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"

#define MAX_BULLET (512)//Åå
#define BULLET_WIDTH (25.0f)//å«³
#define BULLET_HEIGHT (25.0f)//å«³
#define BOM_SIZE_INA (20.0f)//åeTCY
#define SP_SIZE_INA (5.0f)//XyVCTCY
#define BULLET_SPEED (15.0f)//Xs[h
#define BULLET_SPEED_SP (25.0f)//Xs[h
#define BULLET_INTER (20)//­ËÔu
#define ALL_BULLET (10)//SûÊee
#define ALL_BULLETS (6)//U±ee
#define ALL_BULLETSP (200)//XyVCe
#define BULLET_LIFE (500)//ÌÍ
#define HIT_INA (0.25f)

//íÞ
typedef enum
{
	BULLETTYPE_PLAYER = 0,//vC[1
	BULLETTYPE_PLAYER2,//vC[2
	BULLETTYPE_PLAYER3,//vC[3
	BULLETTYPE_PLAYER4,//vC[4
	BULLETTYPE_ENEMY,//G
	BULLETTYPE_BOSS,//{X
	BULLETTYPE_MAX//íÞ
}BULLETTYPE;

void InitBullet(void);//eÌú»
void UninitBullet(void);//eÌI¹
void UpdateBullet(void);//eÌXV
void DrawBullet(void);//eÌ`æ
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLengthPlayer, int nLife, BULLETTYPE type,int nInter);//Êíe
void Set3Bullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//3weye
void SetAllBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//SûÊe
void SetSearchBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//U±e
void SetBomBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//åe
void SetSPBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//åe

#endif _BULLET_H_
