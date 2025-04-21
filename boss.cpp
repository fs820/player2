//----------------------------------------
//
//ボス表示処理[boss.cpp]
//Author fuma sato
//
//----------------------------------------
#include"boss.h"
#include"bullet.h"
#include"explosion.h"
#include"score.h"
#include"exef.h"
#include"input.h"

//マクロ定義
#define BOSS_BULLET_INTER (10)//バレット間隔
#define BOSS_BULLET_SPEED (10.0f)//バレットスピード
#define BOSS_BULLET_SIZE (100.0f)//バレットサイズ
#define BOSS_TERN_COUNT (60)//方向転換間隔
#define BOSS_SPEED (0.3f)//ボスのスピード
#define BOSS_SPEED2 (0.6f)//ボスのスピード
#define BOSS_SPEED3 (0.1f)//ボスのスピード
#define BOSS_SPEED4 (0.0f)//ボスのスピード
#define BOSS_LIFE (2000)
#define BOSS_LIFE2 (3000)
#define BOSS_LIFE3 (2500)
#define BOSS_LIFE4 (5000)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBoss[NUM_BOSS] = {};//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;//バッファのポインタ
Boss g_aBoss[MAX_BOSS];//ボス情報
int g_nNumBoss = 0;//ボスの数

//-----------------------
//ボスの初期化処理
//-----------------------
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BOSS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS,
		&g_apTextureBoss[0]
	);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS2,
		&g_apTextureBoss[1]
	);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS3,
		&g_apTextureBoss[2]
	);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS4,
		&g_apTextureBoss[3]
	);

	for (int i = 0; i < MAX_BOSS; i++)
	{//情報初期化
		g_aBoss[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[i].fSpeed = 0.0f;
		g_aBoss[i].state = BOSSSTATE_NORMAL;
		g_aBoss[i].nLife = 100;
		g_aBoss[i].nCounterState = 0;
		g_aBoss[i].fLength = sqrtf(BOSS_WIDTH * BOSS_WIDTH + BOSS_HEIGHT * BOSS_HEIGHT) / 2.0f;
		g_aBoss[i].nType = 0;
		g_aBoss[i].bUse = false;
	}
	g_nNumBoss = 0;

	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < MAX_BOSS; i++)
	{
		//頂点座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH / 2, g_aBoss[i].pos.y - BOSS_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH / 2, g_aBoss[i].pos.y - BOSS_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH / 2, g_aBoss[i].pos.y + BOSS_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH / 2, g_aBoss[i].pos.y + BOSS_HEIGHT / 2, 0.0f);

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

		pVtx += VT_MAX;
	}

	g_pVtxBuffBoss->Unlock();//プレイヤーバッファのアンロック
}

//---------------------
//ボスの終了処理
//---------------------
void UninitBoss(void)
{
	for (int i = 0; i < NUM_BOSS; i++)
	{
		//テクスチャの破棄
		if (g_apTextureBoss[i] != NULL)
		{
			g_apTextureBoss[i]->Release();
			g_apTextureBoss[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//-----------------------
//ボスの更新処理
//-----------------------
void UpdateBoss(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	static int aBossBulletCnt[MAX_BOSS] = { 0 };
	static int nCount[MAX_BOSS] = { 0 };

	for (int i = 0; i < MAX_BOSS; i++)
	{//ボスの数だけ繰り返す
		if (g_aBoss[i].bUse == true)
		{//ボスが使用されている
			switch (g_aBoss[i].state)
			{//状態
			case BOSSSTATE_NORMAL://通常
				break;
			case BOSSSTATE_DAMAGE://ダメージ
				g_aBoss[i].nCounterState--;
				if (g_aBoss[i].nCounterState <= 0)
				{
					g_aBoss[i].state = BOSSSTATE_NORMAL;

					g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);
					pVtx += i * VT_MAX;
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					g_pVtxBuffBoss->Unlock();//バッファのアンロック
				}
			}

			aBossBulletCnt[i]+=ControllerNum(CONTYPE_D);
			if (ControllerNum(CONTYPE_D)==0)
			{
				aBossBulletCnt[i] ++;
			}

			if (aBossBulletCnt[i] >= BOSS_BULLET_INTER)
			{//バレット間隔
				aBossBulletCnt[i] = 0;
				//弾の設定
				int nRand = 0;
				nRand = rand() % 4;
				switch (nRand)
				{//ランダム
				case 0:
					nRand = rand() % 201;
					SetBullet(g_aBoss[i].pos, D3DXVECTOR3(sinf(D3DX_PI * (((float)nRand / 10.0f) - 1.0f)) * BOSS_BULLET_SPEED, cosf(D3DX_PI * (((float)nRand / 10.0f) - 1.0f)) * BOSS_BULLET_SPEED, 0.0f), BOSS_BULLET_SIZE, BULLET_LIFE, BULLETTYPE_BOSS, 1);
					break;
				case 1:
					Set3Bullet(g_aBoss[i].pos, atan2f(((float)(rand() % 201) / 100.0f) - 1.0f, ((float)(rand() % 201) / 100.0f) - 1.0f), BOSS_BULLET_SIZE, BULLET_LIFE, BULLETTYPE_BOSS, 1);
					break;
				case 2:
					SetAllBullet(g_aBoss[i].pos, atan2f(((float)(rand() % 201) / 100.0f) - 1.0f, ((float)(rand() % 201) / 100.0f) - 1.0f), BOSS_BULLET_SIZE, BULLET_LIFE, BULLETTYPE_BOSS, 1);
					break;
				case 3:
					SetSearchBullet(g_aBoss[i].pos, atan2f(((float)(rand() % 201) / 100.0f) - 1.0f, ((float)(rand() % 201) / 100.0f) - 1.0f), BOSS_BULLET_SIZE, BULLET_LIFE, BULLETTYPE_BOSS, 11);
					break;
				}
			}

			float BossWidth = 0.0f, BossHeight = 0.0f;
			switch (g_aBoss[i].nType)
			{
			case 0:
				BossWidth = BOSS_WIDTH;
				BossHeight = BOSS_HEIGHT;
				break;
			case 1:
				BossWidth = BOSS_WIDTH2;
				BossHeight = BOSS_HEIGHT2;
				break;
			case 2:
				BossWidth = BOSS_WIDTH3;
				BossHeight = BOSS_HEIGHT3;
				break;
			case 3:
				BossWidth = BOSS_WIDTH4;
				BossHeight = BOSS_HEIGHT4;
				break;
			}
			for (int i2 = 0; i2 < MAX_BOSS; i2++)
			{
				if (g_aBoss[i2].bUse == true)
				{
					float BossWidth2 = 0.0f, BossHeight2 = 0.0f;
					switch (g_aBoss[i2].nType)
					{
					case 0:
						BossWidth2 = BOSS_WIDTH;
						BossHeight2 = BOSS_HEIGHT;
						break;
					case 1:
						BossWidth2 = BOSS_WIDTH2;
						BossHeight2 = BOSS_HEIGHT2;
						break;
					case 2:
						BossWidth2 = BOSS_WIDTH3;
						BossHeight2 = BOSS_HEIGHT3;
						break;
					case 3:
						BossWidth2 = BOSS_WIDTH4;
						BossHeight2 = BOSS_HEIGHT4;
						break;
					}
					if (i != i2 && g_aBoss[i].pos.x - BossWidth <= g_aBoss[i2].pos.x + BossWidth2 && g_aBoss[i].pos.x + BossWidth >= g_aBoss[i2].pos.x - BossWidth2 && g_aBoss[i].pos.y - BossHeight <= g_aBoss[i2].pos.y + BossHeight2 && g_aBoss[i].pos.y + BossHeight >= g_aBoss[i2].pos.y - BossHeight2)
					{
						if (g_aBoss[i].pos.x <= g_aBoss[i2].pos.x - BossWidth2)
						{
							g_aBoss[i].pos.x = (g_aBoss[i2].pos.x - BossWidth2) - BossWidth;
						}
						else if (g_aBoss[i].pos.x >= g_aBoss[i2].pos.x + BossWidth2)
						{
							g_aBoss[i].pos.x = (g_aBoss[i2].pos.x + BossWidth2) + BossWidth;
						}
						else if (g_aBoss[i].pos.y <= g_aBoss[i2].pos.y - BossHeight2)
						{
							g_aBoss[i].pos.y = (g_aBoss[i2].pos.y - BossHeight2) - BossHeight;
						}
						else if (g_aBoss[i].pos.y >= g_aBoss[i2].pos.y + BossHeight2)
						{
							g_aBoss[i].pos.y = (g_aBoss[i2].pos.y + BossHeight2) + BossHeight;
						}

						g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック
						pVtx += i * VT_MAX;

						//頂点座標設定
						pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BossWidth / 2, g_aBoss[i].pos.y - BossHeight / 2, 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BossWidth / 2, g_aBoss[i].pos.y - BossHeight / 2, 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BossWidth / 2, g_aBoss[i].pos.y + BossHeight / 2, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BossWidth / 2, g_aBoss[i].pos.y + BossHeight / 2, 0.0f);

						g_pVtxBuffBoss->Unlock();//プレイヤーバッファのアンロック

						g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック
						pVtx += i2 * VT_MAX;

						//頂点座標設定
						pVtx[0].pos = D3DXVECTOR3(g_aBoss[i2].pos.x - BossWidth2 / 2, g_aBoss[i2].pos.y - BossHeight2 / 2, 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBoss[i2].pos.x + BossWidth2 / 2, g_aBoss[i2].pos.y - BossHeight2 / 2, 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBoss[i2].pos.x - BossWidth2 / 2, g_aBoss[i2].pos.y + BossHeight2 / 2, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBoss[i2].pos.x + BossWidth2 / 2, g_aBoss[i2].pos.y + BossHeight2 / 2, 0.0f);

						g_pVtxBuffBoss->Unlock();//プレイヤーバッファのアンロック
					}
				}
			}

			nCount[i]++;
			if (nCount[i]% BOSS_TERN_COUNT ==0)
			{//方向転換間隔
				//ランダムに動く
				int nRand = rand() % 201;
				g_aBoss[i].move.x = sinf(D3DX_PI * (((float)nRand / 100.0f) - 1.0f)) * g_aBoss[i].fSpeed;
				g_aBoss[i].move.y = cosf(D3DX_PI * (((float)nRand / 100.0f) - 1.0f)) * g_aBoss[i].fSpeed;
			}
		}
	}
}

//--------------------
//ボスの描画処理
//--------------------
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BOSS; i++)
	{//ボスの数だけ繰り返す
		if (g_aBoss[i].bUse == true)
		{//ボスが使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBoss[g_aBoss[i].nType]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX * i,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//-------------------
//ボス設定
//-------------------
void SetBoss(D3DXVECTOR3 pos, int nType)
{
	for (int i = 0; i < MAX_BOSS; i++)
	{//ボスの数だけ繰り返す
		if (g_aBoss[i].bUse == false)
		{//ボスが使用されていない
			//初期化
			g_aBoss[i].pos = pos;
			g_aBoss[i].nType = nType;
			g_aBoss[i].state = BOSSSTATE_NORMAL;
			g_aBoss[i].nCounterState = 0;
			g_aBoss[i].bUse = true;

			//頂点座標設定

			VERTEX_2D* pVtx;

			g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += i * VT_MAX;//ポリゴン1つ分進める

			switch (g_aBoss[i].nType)
			{//タイプによって座標を設定
			case 0:
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH / 2, g_aBoss[i].pos.y - BOSS_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH / 2, g_aBoss[i].pos.y - BOSS_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH / 2, g_aBoss[i].pos.y + BOSS_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH / 2, g_aBoss[i].pos.y + BOSS_HEIGHT / 2, 0.0f);

				g_aBoss[i].fSpeed = BOSS_SPEED;
				g_aBoss[i].fLength = sqrtf(BOSS_WIDTH * BOSS_WIDTH + BOSS_HEIGHT * BOSS_HEIGHT) / 2.0f;
				g_aBoss[i].nLife = BOSS_LIFE * ControllerNum(CONTYPE_D);
				if (g_aBoss[i].nLife == 0)
				{
					g_aBoss[i].nLife = BOSS_LIFE;
				}
				break;
			case 1:
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH2 / 2, g_aBoss[i].pos.y - BOSS_HEIGHT2 / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH2 / 2, g_aBoss[i].pos.y - BOSS_HEIGHT2 / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH2 / 2, g_aBoss[i].pos.y + BOSS_HEIGHT2 / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH2 / 2, g_aBoss[i].pos.y + BOSS_HEIGHT2 / 2, 0.0f);

				g_aBoss[i].fSpeed = BOSS_SPEED2;
				g_aBoss[i].fLength = sqrtf(BOSS_WIDTH2 * BOSS_WIDTH2 + BOSS_HEIGHT2 * BOSS_HEIGHT2) / 2.0f;
				g_aBoss[i].nLife = BOSS_LIFE2 * ControllerNum(CONTYPE_D);
				if (g_aBoss[i].nLife == 0)
				{
					g_aBoss[i].nLife = BOSS_LIFE2;
				}
				break;
			case 2:
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH3 / 2, g_aBoss[i].pos.y - BOSS_HEIGHT3 / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH3 / 2, g_aBoss[i].pos.y - BOSS_HEIGHT3 / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH3 / 2, g_aBoss[i].pos.y + BOSS_HEIGHT3 / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH3 / 2, g_aBoss[i].pos.y + BOSS_HEIGHT3 / 2, 0.0f);

				g_aBoss[i].fSpeed = BOSS_SPEED3;
				g_aBoss[i].fLength = sqrtf(BOSS_WIDTH3 * BOSS_WIDTH3 + BOSS_HEIGHT3 * BOSS_HEIGHT3) / 2.0f;
				g_aBoss[i].nLife = BOSS_LIFE3 * ControllerNum(CONTYPE_D);
				if (g_aBoss[i].nLife == 0)
				{
					g_aBoss[i].nLife = BOSS_LIFE3;
				}
				break;
			case 3:
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH4 / 2, g_aBoss[i].pos.y - BOSS_HEIGHT4 / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH4 / 2, g_aBoss[i].pos.y - BOSS_HEIGHT4 / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH4 / 2, g_aBoss[i].pos.y + BOSS_HEIGHT4 / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH4 / 2, g_aBoss[i].pos.y + BOSS_HEIGHT4 / 2, 0.0f);

				g_aBoss[i].fSpeed = BOSS_SPEED4;
				g_aBoss[i].fLength = sqrtf(BOSS_WIDTH4 * BOSS_WIDTH4 + BOSS_HEIGHT4 * BOSS_HEIGHT4) / 2.0f;
				g_aBoss[i].nLife = BOSS_LIFE4 * ControllerNum(CONTYPE_D);
				if (g_aBoss[i].nLife == 0)
				{
					g_aBoss[i].nLife = BOSS_LIFE4;
				}
				break;
			}

			g_pVtxBuffBoss->Unlock();//バッファのアンロック
			g_nNumBoss++;//ボスの数を記録
			break;
		}
	}
}

//-------------------
//ヒット処理
//-------------------
void HitBoss(int nCntBoss, int nDamage)
{
	VERTEX_2D* pVtx;

	g_aBoss[nCntBoss].nLife -= nDamage;//ダメージ

	if (g_aBoss[nCntBoss].nLife <= 0)
	{//体力が尽きた
		//SetExplosion(g_aBoss[nCntBoss].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_aBoss->fLength);
		SetExef(g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].fLength,true);//パーティクル
		g_aBoss[nCntBoss].bUse = false;//ボスを未使用
		g_nNumBoss--;//ボスの数を減らす
		AddScore(nDamage * SCORE_DES_BOSS);//スコア
	}
	else
	{
		g_aBoss[nCntBoss].state = BOSSSTATE_DAMAGE;//ダメージ
		g_aBoss[nCntBoss].nCounterState = 5;//状態カウンター
		AddScore(nDamage * SCORE_HIT_BOSS);//スコア

		g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nCntBoss * VT_MAX;
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_pVtxBuffBoss->Unlock();//バッファのアンロック
	}
}

//---------------------
//ボス情報取得
//---------------------
Boss* GetBoss(void)
{
	return &g_aBoss[0];
}

//----------------------
//ボスの数の取得
//----------------------
int GetNumBoss(void)
{
	return g_nNumBoss;
}