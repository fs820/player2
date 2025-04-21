//---------------------------------------
//
//弾表示処理[bullet.cpp]
//Author fuma sato
//
//---------------------------------------

#include"bullet.h"
#include"effect.h"
#include"exef.h"
#include"explosion.h"
#include"player.h"
#include"player2.h"
#include"player3.h"
#include"player4.h"
#include"democpu.h"
#include"enemy.h"
#include"boss.h"
#include"game.h"
#include"vs.h"

#define BULLET_DAMAGE (10)//通常弾ダメージ
#define BULLET3_DAMAGE (12)//3WEY弾ダメージ
#define BULLETALL_DAMAGE (2)//全方位弾ダメージ
#define BULLETSEARCH_DAMAGE (1)//誘導弾ダメージ
#define BULLETBOM_DAMAGE (500)//巨大弾ダメージ
#define BULLETSP_DAMAGE (50)//スペシャル砲ダメージ
#define BULLETSEARCH_INA_P (0.3f)//プレイヤー誘導弾精度
#define BULLETSEARCH_INA_B (0.1f)//ボス誘導弾精度
#define BULLETSEARCH_INA_V (0.1f)//VS誘導弾精度
#define SEARCH_TIME (20)//ボス誘導弾誘導時間
#define NOSEARCH_TIME (10)//プレイヤー誘導弾誘導するまでの時間
#define BULLET_EFFECT_LIFE (50)

//弾構造体
typedef struct
{
	D3DXVECTOR3 pos[POS_MAX];//位置
	float fLength;//大きさ
	D3DXVECTOR3 move;//移動量
	int nLife;//寿命
	BULLETTYPE Type;//誰の玉か
	int nType;//どんな玉か
	bool bDisp;
	bool bUse[POS_MAX];//使用しているかどうか
}Bullet;

//グローバル
LPDIRECT3DTEXTURE9 g_apTextureBullet = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//バッファのポインタ
Bullet g_aBullet[MAX_BULLET];//弾の情報

//-----------------
//弾の初期化処理
//-----------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//デバイスを取得
	pDevice = GetDevice();

	for (int i = 0; i < MAX_BULLET; i++)
	{//情報初期化
		for (int i2 = 0; i2 < POS_MAX; i2++)
		{
			g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		g_aBullet[i].fLength = 0.0f;
		g_aBullet[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[i].nLife = BULLET_LIFE;
		g_aBullet[i].Type = BULLETTYPE_PLAYER;
		g_aBullet[i].nType = 0;
		for (int i2 = 0; i2 < POS_MAX; i2++)
		{
			g_aBullet[i].bUse[i2] = false;//使用していない状態にする
		}
	}

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BULLET * POS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BULLET,
		&g_apTextureBullet
	);

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BULLET * POS_MAX; i++)
	{//弾の数だけ繰り返す
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x - BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y - BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x + BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y - BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x - BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y + BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x + BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y + BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;//ポリゴン1つ分進める
	}
	g_pVtxBuffBullet->Unlock();//バッファのアンロック
}

//----------------
//弾の終了処理
//----------------
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_apTextureBullet != NULL)
	{
		g_apTextureBullet->Release();
		g_apTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//----------------
//弾の更新処理
//----------------
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_BULLET; i++)
	{//弾の数だけ繰り返す
		if (g_aBullet[i].bUse[SENTER] == true)
		{//弾が使用されている
			if (g_aBullet[i].Type == BULLETTYPE_PLAYER)
			{//プレイヤーの弾
				Enemy* pEnemy;
				Boss* pBoss;
				Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;

				//情報取得
				pEnemy = GetEnemy();
				pBoss = GetBoss();
				pPlayer = GetPlayer();
				pPlayer2 = GetPlayer2();
				pPlayer3 = GetPlayer3();
				pPlayer4 = GetPlayer4();
				float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{//敵の数だけ繰り返す
					if (pEnemy->bUse == true)
					{//敵が使用されている
						switch (pEnemy->nType)
						{//敵の種類によって変える
						case 0:
							EnemyWidth = ENEMY_WIDTH;
							EnemyHeight = ENEMY_HEIGHT;
							break;
						case 1:
							EnemyWidth = ENEMY_WIDTH2;
							EnemyHeight = ENEMY_HEIGHT2;
							break;
						case 2:
							EnemyWidth = ENEMY_WIDTH3;
							EnemyHeight = ENEMY_HEIGHT3;
							break;
						case 3:
							EnemyWidth = ENEMY_WIDTH4;
							EnemyHeight = ENEMY_HEIGHT4;
							break;
						}

						if (pEnemy->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pEnemy->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pEnemy->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pEnemy->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{//敵に当たった
							int nDamage;

							switch (g_aBullet[i].nType)
							{//ダメージ計算
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

							HitEnemy(i2, nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
					}
				}

				pEnemy = GetEnemy();

				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{//敵の数だけ繰り返す
					if (pEnemy->bUse == true)
					{//敵が使用されている

						if (g_aBullet[i].nType == 3)
						{//誘導弾
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{//サーチ中でない
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{//サーチ開始
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{//サーチ
								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);//今の方向
								Xlong[i] = pEnemy->pos.x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pEnemy->pos.y - g_aBullet[i].pos[SENTER].y;
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);//敵の方向

								fRotDiff[i] = fRotDest[i] - fRotMove[i];//差の角度
								if (fRotDiff[i] > D3DX_PI)
								{//修正
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{//修正
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;//角度を補正

								if (fRotMove[i] > D3DX_PI)
								{//修正
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{//修正
									fRotMove[i] += D3DX_PI * 2;
								}

								//移動量の設定
								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{//弾の数だけ繰り返す
								if (g_aBullet[i2].bUse[SENTER] == false)
								{//使用していない弾のサーチを切る
									bSa[i2] = false;
								}
							}
						}
						break;
					}
				}

				for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
				{//ボスの数だけ繰り返す
					if (pBoss->bUse == true)
					{//ボスが使用されている
						switch (pBoss->nType)
						{//種類によって設定
						case 0:
							EnemyWidth = BOSS_WIDTH;
							EnemyHeight = BOSS_HEIGHT;
							break;
						case 1:
							EnemyWidth = BOSS_WIDTH2;
							EnemyHeight = BOSS_HEIGHT2;
							break;
						case 2:
							EnemyWidth = BOSS_WIDTH3;
							EnemyHeight = BOSS_HEIGHT3;
							break;
						case 3:
							EnemyWidth = BOSS_WIDTH4;
							EnemyHeight = BOSS_HEIGHT4;
							break;
						}

						if (pBoss->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pBoss->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pBoss->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pBoss->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{//ボスに当たった
							int nDamage;

							switch (g_aBullet[i].nType)
							{//ダメージ計算
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

							HitBoss(i2, nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
					}
				}

				pBoss = GetBoss();

				for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
				{//ボスの数だけ繰り返す
					if (pBoss->bUse == true)
					{//ボスが使用されている
						if (g_aBullet[i].nType == 3)
						{//誘導弾
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{//サーチしていない
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{//サーチ開始
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{//サーチ
								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);//今の角度
								Xlong[i] = pBoss->pos.x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pBoss->pos.y - g_aBullet[i].pos[SENTER].y;
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);//敵の角度

								fRotDiff[i] = fRotDest[i] - fRotMove[i];//角度の差
								if (fRotDiff[i] > D3DX_PI)
								{//修正
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{//修正
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;//角度を補正

								if (fRotMove[i] > D3DX_PI)
								{//修正
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{//修正
									fRotMove[i] += D3DX_PI * 2;
								}

								//移動量の設定
								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{//弾の数だけ繰り返す
								if (g_aBullet[i2].bUse[SENTER] == false)
								{//使用していない弾のサーチを切る
									bSa[i2] = false;
								}
							}
						}
						break;
					}
				}

				if (GetMode() == MODE_VS)
				{
					if ((pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE) || (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE) || (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE))
					{
						if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false }, bSa2[MAX_BULLET] = { true };
							if (!bSa[i] && bSa2[i])
							{
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else if (bSa[i] && bSa2[i])
							{
								SCount[i]++;
								if (SCount[i] >= SEARCH_TIME)
								{
									bSa[i] = false;
									bSa2[i] = false;
								}

								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
								switch (rand()%3)
								{
								case 0:
									Xlong[i] = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
									Ylong[i] = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
									break;
								case 1:
									Xlong[i] = pPlayer3->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
									Ylong[i] = pPlayer3->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
									break;
								case 2:
									Xlong[i] = pPlayer4->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
									Ylong[i] = pPlayer4->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
									break;
								}
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

								fRotDiff[i] = fRotDest[i] - fRotMove[i];
								if (fRotDiff[i] > D3DX_PI)
								{
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_V;

								if (fRotMove[i] > D3DX_PI)
								{
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{
									fRotMove[i] += D3DX_PI * 2;
								}

								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{
								if (g_aBullet[i2].bUse[SENTER] == false)
								{
									bSa[i2] = false;
									bSa2[i2] = true;
									SCount[i2] = 0;
								}
							}
						}

						if (pPlayer2->pos[SENTER].x - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].x + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

							HitPlayer2(nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}

						if (pPlayer3->pos[SENTER].x - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].x + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

							HitPlayer3(nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}

						if (pPlayer4->pos[SENTER].x - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].x + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

							HitPlayer4(nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}

					}
				}
			}
			else if (g_aBullet[i].Type == BULLETTYPE_PLAYER2)
			{//プレイヤーの弾
				Enemy* pEnemy;
				Boss* pBoss;
				Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;

				pEnemy = GetEnemy();
				pBoss = GetBoss();
				pPlayer = GetPlayer();
				pPlayer2 = GetPlayer2();
				pPlayer3 = GetPlayer3();
				pPlayer4 = GetPlayer4();
				float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						switch (pEnemy->nType)
						{
						case 0:
							EnemyWidth = ENEMY_WIDTH;
							EnemyHeight = ENEMY_HEIGHT;
							break;
						case 1:
							EnemyWidth = ENEMY_WIDTH2;
							EnemyHeight = ENEMY_HEIGHT2;
							break;
						case 2:
							EnemyWidth = ENEMY_WIDTH3;
							EnemyHeight = ENEMY_HEIGHT3;
							break;
						case 3:
							EnemyWidth = ENEMY_WIDTH4;
							EnemyHeight = ENEMY_HEIGHT4;
							break;
						}

						if (pEnemy->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pEnemy->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pEnemy->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pEnemy->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							}

							HitEnemy(i2, nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
					}
				}

				pEnemy = GetEnemy();

				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{
								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
								Xlong[i] = pEnemy->pos.x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pEnemy->pos.y - g_aBullet[i].pos[SENTER].y;
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

								fRotDiff[i] = fRotDest[i] - fRotMove[i];
								if (fRotDiff[i] > D3DX_PI)
								{
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;

								if (fRotMove[i] > D3DX_PI)
								{
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{
									fRotMove[i] += D3DX_PI * 2;
								}

								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{
								if (g_aBullet[i2].bUse[SENTER] == false)
								{
									bSa[i2] = false;
								}
							}
						}
						break;
					}
				}

				for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
				{
					if (pBoss->bUse == true)
					{//敵が使用されている
						switch (pBoss->nType)
						{
						case 0:
							EnemyWidth = BOSS_WIDTH;
							EnemyHeight = BOSS_HEIGHT;
							break;
						case 1:
							EnemyWidth = BOSS_WIDTH2;
							EnemyHeight = BOSS_HEIGHT2;
							break;
						case 2:
							EnemyWidth = BOSS_WIDTH3;
							EnemyHeight = BOSS_HEIGHT3;
							break;
						case 3:
							EnemyWidth = BOSS_WIDTH4;
							EnemyHeight = BOSS_HEIGHT4;
							break;
						}

						if (pBoss->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pBoss->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pBoss->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pBoss->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							}

							HitBoss(i2, nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
					}
				}

				pBoss = GetBoss();

				for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
				{
					if (pBoss->bUse == true)
					{//敵が使用されている
						if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{
								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
								Xlong[i] = pBoss->pos.x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pBoss->pos.y - g_aBullet[i].pos[SENTER].y;
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

								fRotDiff[i] = fRotDest[i] - fRotMove[i];
								if (fRotDiff[i] > D3DX_PI)
								{
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;

								if (fRotMove[i] > D3DX_PI)
								{
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{
									fRotMove[i] += D3DX_PI * 2;
								}

								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{
								if (g_aBullet[i2].bUse[SENTER] == false)
								{
									bSa[i2] = false;
								}
							}
						}
						break;
					}
				}

				if (GetMode() == MODE_VS)
				{
					if ((pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE) || (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE) || (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE))
					{
						if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false }, bSa2[MAX_BULLET] = { true };
							if (!bSa[i] && bSa2[i])
							{
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else if (bSa[i] && bSa2[i])
							{
								SCount[i]++;
								if (SCount[i] >= SEARCH_TIME)
								{
									bSa[i] = false;
									bSa2[i] = false;
								}

								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
								switch (rand() % 3)
								{
								case 0:
									Xlong[i] = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
									Ylong[i] = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
									break;
								case 1:
									Xlong[i] = pPlayer3->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
									Ylong[i] = pPlayer3->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
									break;
								case 2:
									Xlong[i] = pPlayer4->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
									Ylong[i] = pPlayer4->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
									break;
								}
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

								fRotDiff[i] = fRotDest[i] - fRotMove[i];
								if (fRotDiff[i] > D3DX_PI)
								{
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_V;

								if (fRotMove[i] > D3DX_PI)
								{
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{
									fRotMove[i] += D3DX_PI * 2;
								}

								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{
								if (g_aBullet[i2].bUse[SENTER] == false)
								{
									bSa[i2] = false;
									bSa2[i2] = true;
									SCount[i2] = 0;
								}
							}
						}

						if (pPlayer->pos[SENTER].x - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].x + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							}

							HitPlayer(nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}

						if (pPlayer3->pos[SENTER].x - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].x + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							}

							HitPlayer3(nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}

						if (pPlayer4->pos[SENTER].x - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].x + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 4:
								nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 5:
								nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							}

							HitPlayer4(nDamage);
							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
					}
				}
			}
			else if (g_aBullet[i].Type == BULLETTYPE_PLAYER3)
			{//プレイヤーの弾
			Enemy* pEnemy;
			Boss* pBoss;
			Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;

			pEnemy = GetEnemy();
			pBoss = GetBoss();
			pPlayer = GetPlayer();
			pPlayer2 = GetPlayer2();
			pPlayer3 = GetPlayer3();
			pPlayer4 = GetPlayer4();
			float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
			for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{//敵が使用されている
					switch (pEnemy->nType)
					{
					case 0:
						EnemyWidth = ENEMY_WIDTH;
						EnemyHeight = ENEMY_HEIGHT;
						break;
					case 1:
						EnemyWidth = ENEMY_WIDTH2;
						EnemyHeight = ENEMY_HEIGHT2;
						break;
					case 2:
						EnemyWidth = ENEMY_WIDTH3;
						EnemyHeight = ENEMY_HEIGHT3;
						break;
					case 3:
						EnemyWidth = ENEMY_WIDTH4;
						EnemyHeight = ENEMY_HEIGHT4;
						break;
					}

					if (pEnemy->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pEnemy->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pEnemy->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pEnemy->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						}

						HitEnemy(i2, nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
			}

			pEnemy = GetEnemy();

			for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{//敵が使用されている
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false };
						if (!bSa[i])
						{
							SCount[i]++;
							if (SCount[i] >= NOSEARCH_TIME)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else
						{
							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							Xlong[i] = pEnemy->pos.x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pEnemy->pos.y - g_aBullet[i].pos[SENTER].y;
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = false;
							}
						}
					}
					break;
				}
			}

			for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
			{
				if (pBoss->bUse == true)
				{//敵が使用されている
					switch (pBoss->nType)
					{
					case 0:
						EnemyWidth = BOSS_WIDTH;
						EnemyHeight = BOSS_HEIGHT;
						break;
					case 1:
						EnemyWidth = BOSS_WIDTH2;
						EnemyHeight = BOSS_HEIGHT2;
						break;
					case 2:
						EnemyWidth = BOSS_WIDTH3;
						EnemyHeight = BOSS_HEIGHT3;
						break;
					case 3:
						EnemyWidth = BOSS_WIDTH4;
						EnemyHeight = BOSS_HEIGHT4;
						break;
					}

					if (pBoss->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pBoss->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pBoss->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pBoss->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						}

						HitBoss(i2, nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
			}

			pBoss = GetBoss();

			for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
			{
				if (pBoss->bUse == true)
				{//敵が使用されている
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false };
						if (!bSa[i])
						{
							SCount[i]++;
							if (SCount[i] >= NOSEARCH_TIME)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else
						{
							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							Xlong[i] = pBoss->pos.x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pBoss->pos.y - g_aBullet[i].pos[SENTER].y;
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = false;
							}
						}
					}
					break;
				}
			}

			if (GetMode() == MODE_VS)
			{
				if ((pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE) || (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE) || (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE))
				{
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false }, bSa2[MAX_BULLET] = { true };
						if (!bSa[i] && bSa2[i])
						{
							SCount[i]++;
							if (SCount[i] >= NOSEARCH_TIME)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else if (bSa[i] && bSa2[i])
						{
							SCount[i]++;
							if (SCount[i] >= SEARCH_TIME)
							{
								bSa[i] = false;
								bSa2[i] = false;
							}

							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							switch (rand() % 3)
							{
							case 0:
								Xlong[i] = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							case 1:
								Xlong[i] = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							case 2:
								Xlong[i] = pPlayer4->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer4->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							}
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_V;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = false;
								bSa2[i2] = true;
								SCount[i2] = 0;
							}
						}
					}

					if (pPlayer->pos[SENTER].x - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].x + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						}

						HitPlayer(nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (pPlayer2->pos[SENTER].x - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].x + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						}

						HitPlayer2(nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (pPlayer4->pos[SENTER].x - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].x + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer3->fLength * 0.01f);
							break;
						}

						HitPlayer4(nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
			}
			}
			else if (g_aBullet[i].Type == BULLETTYPE_PLAYER4)
			{//プレイヤーの弾
			Enemy* pEnemy;
			Boss* pBoss;
			Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;

			pEnemy = GetEnemy();
			pBoss = GetBoss();
			pPlayer = GetPlayer();
			pPlayer2 = GetPlayer2();
			pPlayer3 = GetPlayer3();
			pPlayer4 = GetPlayer4();
			float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
			for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{//敵が使用されている
					switch (pEnemy->nType)
					{
					case 0:
						EnemyWidth = ENEMY_WIDTH;
						EnemyHeight = ENEMY_HEIGHT;
						break;
					case 1:
						EnemyWidth = ENEMY_WIDTH2;
						EnemyHeight = ENEMY_HEIGHT2;
						break;
					case 2:
						EnemyWidth = ENEMY_WIDTH3;
						EnemyHeight = ENEMY_HEIGHT3;
						break;
					case 3:
						EnemyWidth = ENEMY_WIDTH4;
						EnemyHeight = ENEMY_HEIGHT4;
						break;
					}

					if (pEnemy->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pEnemy->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pEnemy->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pEnemy->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						}

						HitEnemy(i2, nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
			}

			pEnemy = GetEnemy();

			for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{//敵が使用されている
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false };
						if (!bSa[i])
						{
							SCount[i]++;
							if (SCount[i] >= NOSEARCH_TIME)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else
						{
							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							Xlong[i] = pEnemy->pos.x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pEnemy->pos.y - g_aBullet[i].pos[SENTER].y;
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = false;
							}
						}
					}
					break;
				}
			}

			for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
			{
				if (pBoss->bUse == true)
				{//敵が使用されている
					switch (pBoss->nType)
					{
					case 0:
						EnemyWidth = BOSS_WIDTH;
						EnemyHeight = BOSS_HEIGHT;
						break;
					case 1:
						EnemyWidth = BOSS_WIDTH2;
						EnemyHeight = BOSS_HEIGHT2;
						break;
					case 2:
						EnemyWidth = BOSS_WIDTH3;
						EnemyHeight = BOSS_HEIGHT3;
						break;
					case 3:
						EnemyWidth = BOSS_WIDTH4;
						EnemyHeight = BOSS_HEIGHT4;
						break;
					}

					if (pBoss->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pBoss->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pBoss->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pBoss->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						}

						HitBoss(i2, nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
			}

			pBoss = GetBoss();

			for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
			{
				if (pBoss->bUse == true)
				{//敵が使用されている
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false };
						if (!bSa[i])
						{
							SCount[i]++;
							if (SCount[i] >= NOSEARCH_TIME)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else
						{
							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							Xlong[i] = pBoss->pos.x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pBoss->pos.y - g_aBullet[i].pos[SENTER].y;
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = false;
							}
						}
					}
					break;
				}
			}

			if (GetMode() == MODE_VS)
			{
				if ((pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE) || (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE) || (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE))
				{
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false }, bSa2[MAX_BULLET] = { true };
						if (!bSa[i] && bSa2[i])
						{
							SCount[i]++;
							if (SCount[i] >= NOSEARCH_TIME)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else if (bSa[i] && bSa2[i])
						{
							SCount[i]++;
							if (SCount[i] >= SEARCH_TIME)
							{
								bSa[i] = false;
								bSa2[i] = false;
							}

							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							switch (rand() % 3)
							{
							case 0:
								Xlong[i] = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							case 1:
								Xlong[i] = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							case 2:
								Xlong[i] = pPlayer3->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer3->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							}
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_V;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = false;
								bSa2[i2] = true;
								SCount[i2] = 0;
							}
						}
					}

					if (pPlayer->pos[SENTER].x - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].x + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						}

						HitPlayer(nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (pPlayer2->pos[SENTER].x - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].x + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						}

						HitPlayer2(nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (pPlayer3->pos[SENTER].x - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].x + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = (int)((float)BULLET_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 1:
							nDamage = (int)((float)BULLET3_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 2:
							nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 3:
							nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 4:
							nDamage = (int)((float)BULLETBOM_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						case 5:
							nDamage = (int)((float)BULLETSP_DAMAGE * pPlayer4->fLength * 0.01f);
							break;
						}

						HitPlayer3(nDamage);
						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
			}
			}
			else if (g_aBullet[i].Type == BULLETTYPE_ENEMY)
			{//敵の弾
				Player* pPlayer, * pPlayer2;
				DEMOCPU* pDemoCpu;

				pPlayer = GetPlayer();
				pPlayer2 = GetPlayer2();
				pPlayer2 = GetPlayer3();
				pPlayer2 = GetPlayer4();
				pDemoCpu = GetDemoCpu();
				if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
				{
					if (pPlayer->pos[SENTER].x - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].x + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage = 0;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = BULLET_DAMAGE;
							break;
						case 1:
							nDamage = BULLET3_DAMAGE;
							break;
						case 2:
							nDamage = BULLETALL_DAMAGE;
							break;
						case 3:
							nDamage = BULLETSEARCH_DAMAGE;
							break;
						case 4:
							nDamage = BULLETBOM_DAMAGE;
							break;
						case 5:
							nDamage = BULLETSP_DAMAGE;
							break;
						}

						HitPlayer(nDamage);

						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
						}
					}
				}

				if (pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE)
				{
					if (pPlayer2->pos[SENTER].x - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].x + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage = 0;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = BULLET_DAMAGE;
							break;
						case 1:
							nDamage = BULLET3_DAMAGE;
							break;
						case 2:
							nDamage = BULLETALL_DAMAGE;
							break;
						case 3:
							nDamage = BULLETSEARCH_DAMAGE;
							break;
						case 4:
							nDamage = BULLETBOM_DAMAGE;
							break;
						case 5:
							nDamage = BULLETSP_DAMAGE;
							break;
						}

						HitPlayer2(nDamage);

						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}

				if (GetMode() == MODE_DEMO)
				{
					if (pDemoCpu->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
					{
						if (pDemoCpu->pos.x - pDemoCpu->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pDemoCpu->pos.x + pDemoCpu->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pDemoCpu->pos.y - pDemoCpu->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pDemoCpu->pos.y + pDemoCpu->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage = 0;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = BULLET_DAMAGE;
								break;
							case 1:
								nDamage = BULLET3_DAMAGE;
								break;
							case 2:
								nDamage = BULLETALL_DAMAGE;
								break;
							case 3:
								nDamage = BULLETSEARCH_DAMAGE;
								break;
							case 4:
								nDamage = BULLETBOM_DAMAGE;
								break;
							case 5:
								nDamage = BULLETSP_DAMAGE;
								break;
							}

							HitDemoCpu(nDamage);

							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
					}
				}

				if ((pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE) || (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE) || (pDemoCpu->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE))
				{
					if (g_aBullet[i].nType == 3)
					{
						static bool bSa[MAX_BULLET] = { true };
						static int nSaCnt[MAX_BULLET] = { 0 }, nPlayer[MAX_BULLET] = { 0 };
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };

						if (nSaCnt[i] == 0 && bSa[i] == true)
						{
							nPlayer[i] = rand() % 2;
						}

						nSaCnt[i]++;

						if (GetMode() == MODE_DEMO)
						{
							Xlong[i] = pDemoCpu->pos.x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pDemoCpu->pos.y - g_aBullet[i].pos[SENTER].y;
						}
						else if (!(pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE))
						{
							Xlong[i] = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
						}
						else if (!(pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE))
						{
							Xlong[i] = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
						}
						else
						{
							switch (nPlayer[i])
							{
							case 0:
								Xlong[i] = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							case 1:
								Xlong[i] = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							}
						}

						if (nSaCnt[i] >= SEARCH_TIME)
						{
							nSaCnt[i] = 0;
							bSa[i] = false;
						}

						if (bSa[i])
						{
							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_B;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = true;
							}
						}
					}
				}
			}
			else if (g_aBullet[i].Type == BULLETTYPE_BOSS)
			{//敵の弾
				Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;
				DEMOCPU* pDemoCpu;

				pPlayer = GetPlayer();
				pPlayer2 = GetPlayer2();
				pPlayer3 = GetPlayer3();
				pPlayer4 = GetPlayer4();
				pDemoCpu = GetDemoCpu();
				if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
				{
					if (pPlayer->pos[SENTER].x - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].x + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y - pPlayer->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer->pos[SENTER].y + pPlayer->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage = 0;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = BULLET_DAMAGE;
							break;
						case 1:
							nDamage = BULLET3_DAMAGE;
							break;
						case 2:
							nDamage = BULLETALL_DAMAGE;
							break;
						case 3:
							nDamage = BULLETSEARCH_DAMAGE;
							break;
						case 4:
							nDamage = BULLETBOM_DAMAGE;
							break;
						case 5:
							nDamage = BULLETSP_DAMAGE;
							break;
						}

						HitPlayer(nDamage);

						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}

				if (pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE)
				{
					if (pPlayer2->pos[SENTER].x - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].x + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y - pPlayer2->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer2->pos[SENTER].y + pPlayer2->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage = 0;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = BULLET_DAMAGE;
							break;
						case 1:
							nDamage = BULLET3_DAMAGE;
							break;
						case 2:
							nDamage = BULLETALL_DAMAGE;
							break;
						case 3:
							nDamage = BULLETSEARCH_DAMAGE;
							break;
						case 4:
							nDamage = BULLETBOM_DAMAGE;
							break;
						case 5:
							nDamage = BULLETSP_DAMAGE;
							break;
						}

						HitPlayer2(nDamage);

						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}

				if (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE)
				{
					if (pPlayer3->pos[SENTER].x - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].x + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y - pPlayer3->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer3->pos[SENTER].y + pPlayer3->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage = 0;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = BULLET_DAMAGE;
							break;
						case 1:
							nDamage = BULLET3_DAMAGE;
							break;
						case 2:
							nDamage = BULLETALL_DAMAGE;
							break;
						case 3:
							nDamage = BULLETSEARCH_DAMAGE;
							break;
						case 4:
							nDamage = BULLETBOM_DAMAGE;
							break;
						case 5:
							nDamage = BULLETSP_DAMAGE;
							break;
						}

						HitPlayer3(nDamage);

						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}

				if (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE)
				{
					if (pPlayer4->pos[SENTER].x - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].x + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y - pPlayer4->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pPlayer4->pos[SENTER].y + pPlayer4->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
					{
						int nDamage = 0;

						switch (g_aBullet[i].nType)
						{
						case 0:
							nDamage = BULLET_DAMAGE;
							break;
						case 1:
							nDamage = BULLET3_DAMAGE;
							break;
						case 2:
							nDamage = BULLETALL_DAMAGE;
							break;
						case 3:
							nDamage = BULLETSEARCH_DAMAGE;
							break;
						case 4:
							nDamage = BULLETBOM_DAMAGE;
							break;
						case 5:
							nDamage = BULLETSP_DAMAGE;
							break;
						}

						HitPlayer4(nDamage);

						for (int i2 = 0; i2 < POS_MAX; i2++)
						{
							g_aBullet[i].bUse[i2] = false;
							g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}

				if (GetMode() == MODE_DEMO)
				{
					if (pDemoCpu->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
					{
						if (pDemoCpu->pos.x - pDemoCpu->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].x + g_aBullet[i].fLength / 10.0f && pDemoCpu->pos.x + pDemoCpu->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].x - g_aBullet[i].fLength / 10.0f && pDemoCpu->pos.y - pDemoCpu->fLength * HIT_INA <= g_aBullet[i].pos[SENTER].y + g_aBullet[i].fLength / 10.0f && pDemoCpu->pos.y + pDemoCpu->fLength * HIT_INA >= g_aBullet[i].pos[SENTER].y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage = 0;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = BULLET_DAMAGE;
								break;
							case 1:
								nDamage = BULLET3_DAMAGE;
								break;
							case 2:
								nDamage = BULLETALL_DAMAGE;
								break;
							case 3:
								nDamage = BULLETSEARCH_DAMAGE;
								break;
							case 4:
								nDamage = BULLETBOM_DAMAGE;
								break;
							case 5:
								nDamage = BULLETSP_DAMAGE;
								break;
							}

							HitDemoCpu(nDamage);

							for (int i2 = 0; i2 < POS_MAX; i2++)
							{
								g_aBullet[i].bUse[i2] = false;
								g_aBullet[i].pos[i2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
					}
				}

				if ((pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE) || (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE) || (pDemoCpu->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE))
				{
					if (g_aBullet[i].nType == 3)
					{
						static bool bSa[MAX_BULLET] = { true };
						static int nSaCnt[MAX_BULLET] = { 0 }, nPlayer[MAX_BULLET] = { 0 };
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };

						if (nSaCnt[i] == 0 && bSa[i] == true)
						{
							nPlayer[i] = rand() % 2;
						}

						nSaCnt[i]++;

						if (GetMode() == MODE_DEMO)
						{
							Xlong[i] = pDemoCpu->pos.x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pDemoCpu->pos.y - g_aBullet[i].pos[SENTER].y;
						}
						else if (!(pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE))
						{
							Xlong[i] = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
						}
						else if (!(pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE))
						{
							Xlong[i] = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
							Ylong[i] = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
						}
						else
						{
							switch (nPlayer[i])
							{
							case 0:
								Xlong[i] = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							case 1:
								Xlong[i] = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
								break;
							}
						}

						if (nSaCnt[i] >= SEARCH_TIME)
						{
							nSaCnt[i] = 0;
							bSa[i] = false;
						}

						if (bSa[i])
						{
							fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);
							fRotDest[i] = atan2f(Xlong[i], Ylong[i]);

							fRotDiff[i] = fRotDest[i] - fRotMove[i];
							if (fRotDiff[i] > D3DX_PI)
							{
								fRotDiff[i] -= D3DX_PI * 2;
							}
							else if (fRotDiff[i] < -D3DX_PI)
							{
								fRotDiff[i] += D3DX_PI * 2;
							}

							fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_B;

							if (fRotMove[i] > D3DX_PI)
							{
								fRotMove[i] -= D3DX_PI * 2;
							}
							else if (fRotMove[i] < -D3DX_PI)
							{
								fRotMove[i] += D3DX_PI * 2;
							}

							g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
						}

						for (int i2 = 0; i2 < MAX_BULLET; i2++)
						{
							if (g_aBullet[i2].bUse[SENTER] == false)
							{
								bSa[i2] = true;
							}
						}
					}

				}
			}

			g_aBullet[i].nLife--;

			Boss* pBoss;
			pBoss = GetBoss();
			g_aBullet[i].pos[SENTER].x += g_aBullet[i].move.x;
			g_aBullet[i].pos[SENTER].y += g_aBullet[i].move.y;
			g_aBullet[i].pos[SENTER].x -= pBoss->move.x * PLAYER_FIELD_INA;
			g_aBullet[i].pos[SENTER].y -= pBoss->move.y * PLAYER_FIELD_INA;

			if (bOberP1())
			{
				Player* pPlayer;
				pPlayer = GetPlayer();
				float Xlong = 0.0f, Ylong = 0.0f;

				Xlong = pPlayer->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
				Ylong = pPlayer->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
				if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
				{
					g_aBullet[i].bUse[LEFTUP] = true;
					if (GetMode()==MODE_VS)
					{
						g_aBullet[i].pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA_VS, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
					}
					else
					{
						g_aBullet[i].pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA, 0.0f);
					}
				}
				else
				{
					g_aBullet[i].bUse[LEFTUP] = false;
				}
			}
			else
			{
				g_aBullet[i].bUse[LEFTUP] = false;
			}
			if (bOberP2())
			{
				Player* pPlayer2;
				pPlayer2 = GetPlayer2();
				float Xlong = 0.0f, Ylong = 0.0f;

				Xlong = pPlayer2->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
				Ylong = pPlayer2->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
				if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
				{
					g_aBullet[i].bUse[RIGHTUP] = true;
					if (GetMode() == MODE_VS)
					{
						g_aBullet[i].pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA_VS, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
					}
					else
					{
						g_aBullet[i].pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA, 0.0f);
					}
				}
				else
				{
					g_aBullet[i].bUse[RIGHTUP] = false;
				}
			}
			else
			{
				g_aBullet[i].bUse[RIGHTUP] = false;
			}
			if (bOberP3())
			{
				Player* pPlayer3;
				pPlayer3 = GetPlayer3();
				float Xlong = 0.0f, Ylong = 0.0f;

				Xlong = pPlayer3->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
				Ylong = pPlayer3->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
				if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
				{
					g_aBullet[i].bUse[LEFTDOWN] = true;
					if (GetMode() == MODE_VS)
					{
						g_aBullet[i].pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
					}
					else
					{
						g_aBullet[i].pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA, 0.0f);
					}
				}
				else
				{
					g_aBullet[i].bUse[LEFTDOWN] = false;
				}
			}
			else
			{
				g_aBullet[i].bUse[LEFTDOWN] = false;
			}
			if (bOberP4())
			{
				Player* pPlayer4;
				pPlayer4 = GetPlayer4();
				float Xlong = 0.0f, Ylong = 0.0f;

				Xlong = pPlayer4->pos[SENTER].x - g_aBullet[i].pos[SENTER].x;
				Ylong = pPlayer4->pos[SENTER].y - g_aBullet[i].pos[SENTER].y;
				if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
				{
					g_aBullet[i].bUse[RIGHTDOWN] = true;
					if (GetMode() == MODE_VS)
					{
						g_aBullet[i].pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
					}
					else
					{
						g_aBullet[i].pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA, 0.0f);
					}
				}
				else
				{
					g_aBullet[i].bUse[RIGHTDOWN] = false;
				}
			}
			else
			{
				g_aBullet[i].bUse[RIGHTDOWN] = false;
			}

			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			//座標設定
			pVtx += i * VT_MAX * POS_MAX;//ポリゴン1つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			for (int i2 = 1; i2 < POS_MAX; i2++)
			{
				pVtx += VT_MAX;
				if (GetMode()==MODE_VS)
				{
					pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, g_aBullet[i].pos[i2].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, g_aBullet[i].pos[i2].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, g_aBullet[i].pos[i2].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, g_aBullet[i].pos[i2].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA_VS, 0.0f);
				}
				else
				{
					pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, g_aBullet[i].pos[i2].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, g_aBullet[i].pos[i2].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, g_aBullet[i].pos[i2].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[i2].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, g_aBullet[i].pos[i2].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f) * SPLIT_INA, 0.0f);
				}
			}

			g_pVtxBuffBullet->Unlock();//バッファのアンロック

			if (GetMode()==MODE_VS)
			{
				if (g_aBullet[i].pos[SENTER].x > 0 && g_aBullet[i].pos[SENTER].x < SCREEN_WIDTH && g_aBullet[i].pos[SENTER].y>0 && g_aBullet[i].pos[SENTER].y < SCREEN_HEIGHT && (!bOberP1() || !(g_aBullet[i].pos[SENTER].x <= SCREEN_WIDTH * SPLIT_INA_VS && g_aBullet[i].pos[SENTER].y <= SCREEN_HEIGHT * SPLIT_INA_VS)) && (!bOberP2() || !(g_aBullet[i].pos[SENTER].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS && g_aBullet[i].pos[SENTER].y <= SCREEN_HEIGHT * SPLIT_INA_VS)) && !(g_aBullet[i].pos[SENTER].x <= SCREEN_WIDTH * SPLIT_INA_VS && g_aBullet[i].pos[SENTER].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS) && !(g_aBullet[i].pos[SENTER].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS && g_aBullet[i].pos[SENTER].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS))
				{
					g_aBullet[i].bDisp = true;
				}
				else
				{
					g_aBullet[i].bDisp = false;
				}
			}
			else
			{
				if (g_aBullet[i].pos[SENTER].x > 0 && g_aBullet[i].pos[SENTER].x < SCREEN_WIDTH && g_aBullet[i].pos[SENTER].y>0 && g_aBullet[i].pos[SENTER].y < SCREEN_HEIGHT && (!bOberP1() || !(g_aBullet[i].pos[SENTER].x <= SCREEN_WIDTH * SPLIT_INA && g_aBullet[i].pos[SENTER].y <= SCREEN_HEIGHT * SPLIT_INA)) && (!bOberP2() || !(g_aBullet[i].pos[SENTER].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && g_aBullet[i].pos[SENTER].y <= SCREEN_HEIGHT * SPLIT_INA)) && !(g_aBullet[i].pos[SENTER].x <= SCREEN_WIDTH * SPLIT_INA && g_aBullet[i].pos[SENTER].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA) && !(g_aBullet[i].pos[SENTER].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && g_aBullet[i].pos[SENTER].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA))
				{
					g_aBullet[i].bDisp = true;
				}
				else
				{
					g_aBullet[i].bDisp = false;
				}
			}

			if (g_aBullet[i].nLife <= 0)
			{
				if (g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 <= SCREEN_WIDTH && g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 >= 0.0f && g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 <= SCREEN_HEIGHT && g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 >= 0.0f)
				{
					SetExef(g_aBullet[i].pos[SENTER], g_aBullet[i].fLength, true);
				}
				if (bOberP1())
				{
					if (g_aBullet[i].bUse[LEFTUP])
					{
						if (GetMode()==MODE_VS)
						{
							SetExef(g_aBullet[i].pos[LEFTUP], g_aBullet[i].fLength* SPLIT_INA_VS, false);
						}
						else
						{
							SetExef(g_aBullet[i].pos[LEFTUP], g_aBullet[i].fLength* SPLIT_INA, false);
						}
					}
				}
				if (bOberP2())
				{
					if (g_aBullet[i].bUse[RIGHTUP])
					{
						if (GetMode() == MODE_VS)
						{
							SetExef(g_aBullet[i].pos[RIGHTUP], g_aBullet[i].fLength * SPLIT_INA_VS, false);
						}
						else
						{
							SetExef(g_aBullet[i].pos[RIGHTUP], g_aBullet[i].fLength * SPLIT_INA, false);
						}
					}
				}
				if (bOberP3())
				{
					if (g_aBullet[i].bUse[LEFTDOWN])
					{
						if (GetMode() == MODE_VS)
						{
							SetExef(g_aBullet[i].pos[LEFTDOWN], g_aBullet[i].fLength * SPLIT_INA_VS, false);
						}
						else
						{
							SetExef(g_aBullet[i].pos[LEFTDOWN], g_aBullet[i].fLength * SPLIT_INA, false);
						}
					}
				}
				if (bOberP4())
				{
					if (g_aBullet[i].bUse[RIGHTDOWN])
					{
						if (GetMode() == MODE_VS)
						{
							SetExef(g_aBullet[i].pos[RIGHTDOWN], g_aBullet[i].fLength * SPLIT_INA_VS, false);
						}
						else
						{
							SetExef(g_aBullet[i].pos[RIGHTDOWN], g_aBullet[i].fLength * SPLIT_INA, false);
						}
					}
				}
				for (int i2 = 0; i2 < POS_MAX; i2++)
				{
					g_aBullet[i].bUse[i2] = false;
				}
			}

			D3DXCOLOR col;
			switch (g_aBullet[i].nType)
			{
			case 0:
				col = D3DXCOLOR(0.8f, 0.0f, 0.0f, (float)(rand() % 51) / 100.0f);
				break;
			case 1:
				col = D3DXCOLOR(0.0f, 0.8f, 0.0f, (float)(rand() % 51) / 100.0f);
				break;
			case 2:
				col = D3DXCOLOR(0.0f, 0.0f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case 3:
				col = D3DXCOLOR(0.0f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case 4:
				col = D3DXCOLOR(0.8f, 0.0f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case 5:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			}

			if (g_aBullet[i].bUse[SENTER])
			{
				SetEffect(g_aBullet[i].pos[SENTER], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, g_aBullet[i].fLength / 5.0f, BULLET_EFFECT_LIFE, true);
			}
			if (bOberP1())
			{
				if (g_aBullet[i].bUse[LEFTUP])
				{
					if (GetMode()==MODE_VS)
					{
						SetEffect(g_aBullet[i].pos[LEFTUP], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f) * SPLIT_INA_VS, BULLET_EFFECT_LIFE, false);
					}
					else
					{
						SetEffect(g_aBullet[i].pos[LEFTUP], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f)* SPLIT_INA, BULLET_EFFECT_LIFE, false);
					}
				}
			}
			if (bOberP2())
			{
				if (g_aBullet[i].bUse[RIGHTUP])
				{
					if (GetMode() == MODE_VS)
					{
						SetEffect(g_aBullet[i].pos[RIGHTUP], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f) * SPLIT_INA_VS, BULLET_EFFECT_LIFE, false);
					}
					else
					{
						SetEffect(g_aBullet[i].pos[RIGHTUP], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f) * SPLIT_INA, BULLET_EFFECT_LIFE, false);
					}
				}
			}
			if (bOberP3())
			{
				if (g_aBullet[i].bUse[LEFTDOWN])
				{
					if (GetMode() == MODE_VS)
					{
						SetEffect(g_aBullet[i].pos[LEFTDOWN], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f) * SPLIT_INA_VS, BULLET_EFFECT_LIFE, false);
					}
					else
					{
						SetEffect(g_aBullet[i].pos[LEFTDOWN], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f) * SPLIT_INA, BULLET_EFFECT_LIFE, false);
					}
				}
			}
			if (bOberP4())
			{
				if (g_aBullet[i].bUse[RIGHTDOWN])
				{
					if (GetMode() == MODE_VS)
					{
						SetEffect(g_aBullet[i].pos[RIGHTDOWN], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f) * SPLIT_INA_VS, BULLET_EFFECT_LIFE, false);
					}
					else
					{
						SetEffect(g_aBullet[i].pos[RIGHTDOWN], D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, (g_aBullet[i].fLength / 5.0f) * SPLIT_INA, BULLET_EFFECT_LIFE, false);
					}
				}
			}
		}
	}
}

//----------------
//弾の描画処理
//----------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BULLET * POS_MAX; i++)
	{
		if (g_aBullet[i / POS_MAX].bUse[i % POS_MAX] == true)
		{//弾が使用されている
			if (g_aBullet[i / POS_MAX].bDisp || i % POS_MAX != SENTER)
			{
				if (GetMode() != MODE_VS || i % POS_MAX != SENTER)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_apTextureBullet);
					//弾の描画
					pDevice->DrawPrimitive
					(
						D3DPT_TRIANGLESTRIP,//タイプ
						i * VT_MAX,//始まりの番号
						2//ポリゴンの個数
					);
				}
			}
		}
	}
}

//-------------------
//発通常弾
//-------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	static int Inter = 0;

	Inter++;

	if (Inter % nInter == 0)
	{
		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (g_aBullet[i].bUse[SENTER] == false)
			{//弾が使用されていない
				g_aBullet[i].pos[SENTER] = pos;
				g_aBullet[i].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//頂点座標設定
				pVtx += i * VT_MAX * POS_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//バッファのアンロック

				g_aBullet[i].move = move;
				g_aBullet[i].nLife = nLife;
				g_aBullet[i].Type = type;
				g_aBullet[i].nType = 0;
				g_aBullet[i].bUse[SENTER] = true;

				break;
			}
		}
	}
}

//-----------------
//3wey弾
//-----------------
void Set3Bullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	static int Inter = 0;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;
		switch (type)
		{
		case BULLETTYPE_PLAYER:
			Player* pPlayer;
			pPlayer = GetPlayer();
			pPlayer->BulletCount--;
			break;
		case BULLETTYPE_PLAYER2:
			Player* pPlayer2;
			pPlayer2 = GetPlayer2();
			pPlayer2->BulletCount--;
			break;
		}

		for (int i = 0; i < MAX_BULLET - 2; i++)
		{
			if (g_aBullet[i].bUse[SENTER] == false && g_aBullet[i + 1].bUse[SENTER] == false && g_aBullet[i + 2].bUse[SENTER] == false)
			{//弾が使用されていない
				g_aBullet[i].pos[SENTER] = pos;
				g_aBullet[i + 1].pos[SENTER] = pos;
				g_aBullet[i + 2].pos[SENTER] = pos;
				g_aBullet[i].fLength = fLengthPlayer;
				g_aBullet[i + 1].fLength = fLengthPlayer;
				g_aBullet[i + 2].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//頂点座標設定
				pVtx += i * VT_MAX * POS_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX * POS_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX * POS_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//バッファのアンロック

				g_aBullet[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED, 0.0f);
				g_aBullet[i + 1].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.25f) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * 0.25f) * BULLET_SPEED, 0.0f);
				g_aBullet[i + 2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.75f) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * 0.75f) * BULLET_SPEED, 0.0f);

				g_aBullet[i].nLife = nLife;
				g_aBullet[i + 1].nLife = nLife;
				g_aBullet[i + 2].nLife = nLife;
				g_aBullet[i].Type = type;
				g_aBullet[i + 1].Type = type;
				g_aBullet[i + 2].Type = type;
				g_aBullet[i].nType = 1;
				g_aBullet[i + 1].nType = 1;
				g_aBullet[i + 2].nType = 1;
				g_aBullet[i].bUse[SENTER] = true;
				g_aBullet[i + 1].bUse[SENTER] = true;
				g_aBullet[i + 2].bUse[SENTER] = true;

				break;
			}
		}
	}
}

//----------------
//全方位弾
//----------------
void SetAllBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	float fAll = 0.0f;
	static int Inter = 0;
	bool OK = false;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;
		switch (type)
		{
		case BULLETTYPE_PLAYER:
			Player* pPlayer;
			pPlayer = GetPlayer();
			pPlayer->BulletCount--;
			break;
		case BULLETTYPE_PLAYER2:
			Player* pPlayer2;
			pPlayer2 = GetPlayer2();
			pPlayer2->BulletCount--;
			break;
		}

		for (int i = 0; i < MAX_BULLET - ALL_BULLET; i++)
		{
			for (int i2 = 0; i2 < ALL_BULLET; i2++)
			{
				if (g_aBullet[i + i2].bUse[SENTER] == true)
				{
					break;
				}
				if (i2 == ALL_BULLET - 1)
				{
					OK = true;
				}
			}
			if (OK == true)
			{
				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += i * VT_MAX * POS_MAX;//ポリゴン1つ分進める
				for (int i2 = 0; i2 < ALL_BULLET; i2++)
				{
					if (g_aBullet[i + i2].bUse[SENTER] == false)
					{//弾が使用されていない

						g_aBullet[i + i2].pos[SENTER] = pos;
						g_aBullet[i + i2].fLength = fLengthPlayer;


						//頂点座標設定
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX * POS_MAX;//ポリゴン1つ分進める

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 2;
						g_aBullet[i + i2].bUse[SENTER] = true;
						fAll += 2.0f / (float)ALL_BULLET;
					}
				}
				g_pVtxBuffBullet->Unlock();//バッファのアンロック
				break;
			}
		}
	}
}

//-----------------
//誘導弾
//-----------------
void SetSearchBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	float fAll = 0.0f;
	static int Inter = 0;
	bool OK = false;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;
		switch (type)
		{
		case BULLETTYPE_PLAYER:
			Player* pPlayer;
			pPlayer = GetPlayer();
			pPlayer->BulletCount--;
			break;
		case BULLETTYPE_PLAYER2:
			Player* pPlayer2;
			pPlayer2 = GetPlayer2();
			pPlayer2->BulletCount--;
			break;
		}

		for (int i = 0; i < MAX_BULLET - ALL_BULLETS; i++)
		{
			for (int i2 = 0; i2 < ALL_BULLETS; i2++)
			{
				if (g_aBullet[i + i2].bUse[SENTER] == true)
				{
					break;
				}
				if (i2 == ALL_BULLETS - 1)
				{
					OK = true;
				}
			}
			if (OK == true)
			{
				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += i * VT_MAX * POS_MAX;//ポリゴン1つ分進める
				for (int i2 = 0; i2 < ALL_BULLETS; i2++)
				{
					if (g_aBullet[i + i2].bUse[SENTER] == false)
					{//弾が使用されていない

						g_aBullet[i + i2].pos[SENTER] = pos;
						g_aBullet[i + i2].fLength = fLengthPlayer;


						//頂点座標設定
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX * POS_MAX;//ポリゴン1つ分進める

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 3;
						g_aBullet[i + i2].bUse[SENTER] = true;
						fAll += 2.0f / (float)ALL_BULLETS;
					}
				}
				g_pVtxBuffBullet->Unlock();//バッファのアンロック
				break;
			}
		}
	}
}

//------------
//巨大弾
//------------
void SetBomBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	static int Inter = 0;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;
		switch (type)
		{
		case BULLETTYPE_PLAYER:
			Player* pPlayer;
			pPlayer = GetPlayer();
			pPlayer->BulletCount--;
			break;
		case BULLETTYPE_PLAYER2:
			Player* pPlayer2;
			pPlayer2 = GetPlayer2();
			pPlayer2->BulletCount--;
			break;
		}

		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (g_aBullet[i].bUse[SENTER] == false)
			{//弾が使用されていない
				g_aBullet[i].pos[SENTER] = pos;
				g_aBullet[i].fLength = fLengthPlayer;
				g_aBullet[i].fLength *= BOM_SIZE_INA;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//頂点座標設定
				pVtx += i * VT_MAX * POS_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//バッファのアンロック

				g_aBullet[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED, 0.0f);

				g_aBullet[i].nLife = nLife;
				g_aBullet[i].Type = type;
				g_aBullet[i].nType = 4;
				g_aBullet[i].bUse[SENTER] = true;

				break;
			}
		}
	}
}

//------------
//巨大弾
//------------
void SetSPBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	static int Inter = 0;
	bool OK = false;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;
		switch (type)
		{
		case BULLETTYPE_PLAYER:
			Player* pPlayer;
			pPlayer = GetPlayer();
			pPlayer->BulletCount--;
			break;
		case BULLETTYPE_PLAYER2:
			Player* pPlayer2;
			pPlayer2 = GetPlayer2();
			pPlayer2->BulletCount--;
			break;
		}

		for (int i = 0; i < MAX_BULLET - ALL_BULLETSP; i++)
		{
			for (int i2 = 0; i2 < ALL_BULLETSP; i2++)
			{
				if (g_aBullet[i + i2].bUse[SENTER] == true)
				{
					break;
				}
				if (i2 == ALL_BULLETSP - 1)
				{
					OK = true;
				}
			}
			if (OK == true)
			{
				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += i * VT_MAX * POS_MAX;//ポリゴン1つ分進める
				for (int i2 = 0; i2 < ALL_BULLETSP; i2++)
				{
					if (g_aBullet[i + i2].bUse[SENTER] == false)
					{//弾が使用されていない

						g_aBullet[i + i2].pos[SENTER] = pos;
						g_aBullet[i + i2].pos[SENTER] += D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP * i2, cosf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP * i2, 0.0f);
						g_aBullet[i + i2].fLength = fLengthPlayer;
						g_aBullet[i + i2].fLength *= SP_SIZE_INA;

						//頂点座標設定
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX * POS_MAX;//ポリゴン1つ分進める

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP, cosf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 5;
						g_aBullet[i + i2].bUse[SENTER] = true;
					}
				}
				g_pVtxBuffBullet->Unlock();//バッファのアンロック
				break;
			}
		}
	}
}