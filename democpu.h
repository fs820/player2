//---------------------------------------
//
//デモプレイコンピューター表示処理の定義・宣言[democpu.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _DEMOCPU_H_
#define _DEMOCPU_H_

#include"main.h"
#include"player.h"

//プレイヤー情報
#define DEMOCPU_WIDTH (160)//幅
#define DEMOCPU_HEIGHT (90)//高さ
#define DEMOCPU_SPEED_DEF (0.0f)//基本スピード
#define DEMOCPU_SPEED_X (3.0f)//速さ
#define DEMOCPU_SPEED_Y (3.0f)//速さ
#define DEMOCPU_INA (0.2f)//慣性
#define DEMOCPU_ROT_SPEED (0.05f)//回転速度
#define DEMOCPU_SIZE_SPEED (3.0f)//拡大縮小スピード
#define DEMOCPU_SIZE_MAX (300.0f)//大きさの最大
#define DEMOCPU_SIZE_MIN (5.0f)//大きさの最小

#define U_MAX_C (1)//テクスチャの数(横)
#define V_MAX_C (1)//テクスチャの数(縦)

//CPU構造体
typedef struct
{
	int nCounterAnim;//CPUの更新間隔
	int nPatternAnim;//CPUのアニメーションパターン管理
	D3DXVECTOR3 pos;//CPU座標管理
	D3DXVECTOR3 move;//CPUスピード管理
	D3DXVECTOR3 rot;//CPU向き管理
	D3DXVECTOR3 rotDef;//CPU向き管理
	float fLength;//対角線の長さ(大きさ)
	float fLengthDef;//対角線の長さ(大きさ)の初期値
	float fAngle;//対角線の角度
	int nLife;//体力
	PLAYERSTATE state;//状態
	int nCounterState;//状態カウンター
}DEMOCPU;

void InitDemoCpu(void);//CPUの初期化処理
void UninitDemoCpu(void);//CPUの終了処理
void UpdateDemoCpu(void);//CPUの更新処理
void DrawDemoCpu(void);//ポCPUの描画処理
void HitDemoCpu(int nDamage);//ヒット処理
PDIRECT3DVERTEXBUFFER9 GetDemoCpuBuffer(void);//CPUバッファの取得
DEMOCPU* GetDemoCpu(void);//CPU情報取得

#endif _DEMOCPU_H_