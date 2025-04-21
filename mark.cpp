//------------------------------------
//
//プレイヤー方向マーク処理[mark.cpp]
//Author fuma sato
//
//------------------------------------
#include"mark.h"
#include"player.h"
#include"player2.h"
#include"player3.h"
#include"player4.h"

#define MARK_MAX (4)
#define MARK_SIZE (64.0f)
#define MARK_SPEED (20.0f)

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;//プレイヤー座標管理
	D3DXVECTOR3 move;//プレイヤースピード管理
	D3DXVECTOR3 rot;//プレイヤー向き管理
	D3DCOLOR col;//色
	float fLength;//対角線の長さ(大きさ)
	float fAngle;//対角線の角度
}MARK;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMark = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMark = NULL;//バッファのポインタ
MARK g_Mark[MARK_MAX];
//-------------------
//マークの初期化処理
//-------------------
void InitMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MARK_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMark,
		NULL
	);

	if (GetMode()==MODE_VS)
	{
		//テクスチャの読み込み
		HRESULT hr = D3DXCreateTextureFromFile
		(
			pDevice,
			TEXTURE_EFFECT,
			&g_pTextureMark
		);

		for (int i = 0; i < MARK_MAX; i++)
		{
			g_Mark[i].pos = D3DXVECTOR3(MARK_SIZE * VS_MARK_SIZE_INA, MARK_SIZE * VS_MARK_SIZE_INA, 0.0f);
			g_Mark[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_Mark[i].fLength = sqrtf(MARK_SIZE * VS_MARK_SIZE_INA * MARK_SIZE * VS_MARK_SIZE_INA + MARK_SIZE * VS_MARK_SIZE_INA * MARK_SIZE * VS_MARK_SIZE_INA) / 2.0f;//対角線の長さ
			g_Mark[i].fAngle = atan2f(MARK_SIZE * VS_MARK_SIZE_INA, MARK_SIZE * VS_MARK_SIZE_INA);//角度
		}
	}
	else
	{
		//テクスチャの読み込み
		HRESULT hr = D3DXCreateTextureFromFile
		(
			pDevice,
			TEXTURE_NAME_MARK,
			&g_pTextureMark
		);

		for (int i = 0; i < MARK_MAX; i++)
		{
			g_Mark[i].pos = D3DXVECTOR3(MARK_SIZE, MARK_SIZE, 0.0f);
			g_Mark[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_Mark[i].fLength = sqrtf(MARK_SIZE * MARK_SIZE + MARK_SIZE * MARK_SIZE) / 2.0f;//対角線の長さ
			g_Mark[i].fAngle = atan2f(MARK_SIZE, MARK_SIZE);//角度
		}
	}

	g_Mark[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.8f);
	g_Mark[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
	g_Mark[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
	g_Mark[3].col = D3DXCOLOR(0.5f, 0.0f, 0.5f, 0.8f);

	g_pVtxBuffMark->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	for (int i = 0; i < MARK_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = g_Mark[i].col;
		pVtx[1].col = g_Mark[i].col;
		pVtx[2].col = g_Mark[i].col;
		pVtx[3].col = g_Mark[i].col;
		
		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffMark->Unlock();//バッファのアンロック
}

//--------------------
//マークの終了処理
//--------------------
void UninitMark(void)
{
	//テクスチャの破棄
	if (g_pTextureMark != NULL)
	{
		g_pTextureMark->Release();
		g_pTextureMark = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMark != NULL)
	{
		g_pVtxBuffMark->Release();
		g_pVtxBuffMark = NULL;
	}
}

//---------------------
//マークの更新処理
//---------------------
void UpdateMark(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ

	Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;
	pPlayer = GetPlayer();
	pPlayer2 = GetPlayer2();
	pPlayer3 = GetPlayer3();
	pPlayer4 = GetPlayer4();
	float Xlong[MARK_MAX] = { 0.0f }, Ylong[MARK_MAX] = { 0.0f };

	if (GetMode()==MODE_VS)
	{
		Xlong[0] = pPlayer->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[0] = pPlayer->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[0] *= VS_MARK_INA;
		Ylong[0] *= VS_MARK_INA;
		g_Mark[0].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[0], -Ylong[0]));
		g_Mark[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[0], SCREEN_HEIGHT / 2 + Ylong[0], 0.0f);

		Xlong[1] = pPlayer2->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[1] = pPlayer2->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[1] *= VS_MARK_INA;
		Ylong[1] *= VS_MARK_INA;
		g_Mark[1].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[1], -Ylong[1]));
		g_Mark[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[1], SCREEN_HEIGHT / 2 + Ylong[1], 0.0f);

		Xlong[2] = pPlayer3->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[2] = pPlayer3->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[2] *= VS_MARK_INA;
		Ylong[2] *= VS_MARK_INA;
		g_Mark[2].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[2], -Ylong[2]));
		g_Mark[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[2], SCREEN_HEIGHT / 2 + Ylong[2], 0.0f);

		Xlong[3] = pPlayer4->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[3] = pPlayer4->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[3] *= VS_MARK_INA;
		Ylong[3] *= VS_MARK_INA;
		g_Mark[3].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[3], -Ylong[3]));
		g_Mark[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[3], SCREEN_HEIGHT / 2 + Ylong[3], 0.0f);
	}
	else
	{
		Xlong[0] = pPlayer->pos[SENTER].x - g_Mark[0].pos.x;
		Ylong[0] = pPlayer->pos[SENTER].y - g_Mark[0].pos.y;
		g_Mark[0].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[0], -Ylong[0]));
		g_Mark[0].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[0], -Ylong[0])) * MARK_SPEED, -cosf(atan2f(-Xlong[0], -Ylong[0])) * MARK_SPEED, 0.0f);

		Xlong[1] = pPlayer2->pos[SENTER].x - g_Mark[1].pos.x;
		Ylong[1] = pPlayer2->pos[SENTER].y - g_Mark[1].pos.y;
		g_Mark[1].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[1], -Ylong[1]));
		g_Mark[1].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, -cosf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, 0.0f);

		Xlong[2] = pPlayer3->pos[SENTER].x - g_Mark[2].pos.x;
		Ylong[2] = pPlayer3->pos[SENTER].y - g_Mark[2].pos.y;
		g_Mark[2].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[2], -Ylong[2]));
		g_Mark[2].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[2], -Ylong[2])) * MARK_SPEED, -cosf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, 0.0f);

		Xlong[3] = pPlayer4->pos[SENTER].x - g_Mark[3].pos.x;
		Ylong[3] = pPlayer4->pos[SENTER].y - g_Mark[3].pos.y;
		g_Mark[3].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[3], -Ylong[3]));
		g_Mark[3].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[3], -Ylong[3])) * MARK_SPEED, -cosf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, 0.0f);

		for (int i = 0; i < MARK_MAX; i++)
		{
			g_Mark[i].pos.x += g_Mark[i].move.x;
			g_Mark[i].pos.y += g_Mark[i].move.y;

			if (g_Mark[i].pos.x - g_Mark[i].fLength < 0)
			{
				g_Mark[i].pos.x = g_Mark[i].fLength;
			}
			if (g_Mark[i].pos.x + g_Mark[i].fLength > SCREEN_WIDTH)
			{
				g_Mark[i].pos.x = SCREEN_WIDTH - g_Mark[i].fLength;
			}
			if (g_Mark[i].pos.y - g_Mark[i].fLength < 0)
			{
				g_Mark[i].pos.y = g_Mark[i].fLength;
			}
			if (g_Mark[i].pos.y + g_Mark[i].fLength > SCREEN_HEIGHT)
			{
				g_Mark[i].pos.y = SCREEN_HEIGHT - g_Mark[i].fLength;
			}
		}
	}

	g_pVtxBuffMark->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	for (int i = 0; i < MARK_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffMark->Unlock();//バッファのアンロック
}

//----------------------
//マークの描画処理
//----------------------
void DrawMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

    //デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffMark, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMark);

	if (bOberP1()|| GetMode() == MODE_VS)
	{
		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			0,//始まりの番号
			2//ポリゴンの個数
		);
	}

	if (bOberP2() || GetMode() == MODE_VS)
	{
		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			VT_MAX,//始まりの番号
			2//ポリゴンの個数
		);
	}

	if (bOberP3() || GetMode() == MODE_VS)
	{
		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			VT_MAX * 2,//始まりの番号
			2//ポリゴンの個数
		);
	}

	if (bOberP4() || GetMode() == MODE_VS)
	{
		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			VT_MAX * 3,//始まりの番号
			2//ポリゴンの個数
		);
	}
}