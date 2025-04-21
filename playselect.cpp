//------------------------------------------
//
//リザルトの処理[playselect.cpp]
//Author fuma sato
//
//------------------------------------------

#include"playselect.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"data.h"
#include"game.h"
#include"boss.h"
#include"cursor.h"
#include"movie.h"

#define PLAYSELECT_WIDTH (200)
#define PLAYSELECT_HEIGHT (200)

LPDIRECT3DTEXTURE9 g_apTexturePlaySelect[NUM_BOSS + 1] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlaySelect = NULL;//バッファのポインタ

//--------------------
//初期化処理
//--------------------
void InitPlaySelect(void)
{
	HRESULT hr = PlayVideo(GethWnd(), MOVIE_OP);
	if (FAILED(hr))
	{
		MessageBox(NULL, "動画の再生に失敗しました。", "エラー", MB_OK);
	}

	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 Bosspos;

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * (NUM_BOSS + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlaySelect,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_TITLE,
		&g_apTexturePlaySelect[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS,
		&g_apTexturePlaySelect[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS2,
		&g_apTexturePlaySelect[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS3,
		&g_apTexturePlaySelect[3]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS4,
		&g_apTexturePlaySelect[4]
	);

	Bosspos = D3DXVECTOR3(SCREEN_WIDTH/6, SCREEN_HEIGHT / 2, 0.0f);

	g_pVtxBuffPlaySelect->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

		//座標設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

	for (int i = 0; i < NUM_BOSS; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(Bosspos.x - BOSS_WIDTH / 2, Bosspos.y - BOSS_HEIGHT / 2, Bosspos.z);
		pVtx[1].pos = D3DXVECTOR3(Bosspos.x + BOSS_WIDTH / 2, Bosspos.y - BOSS_HEIGHT / 2, Bosspos.z);
		pVtx[2].pos = D3DXVECTOR3(Bosspos.x - BOSS_WIDTH / 2, Bosspos.y + BOSS_HEIGHT / 2, Bosspos.z);
		pVtx[3].pos = D3DXVECTOR3(Bosspos.x + BOSS_WIDTH / 2, Bosspos.y + BOSS_HEIGHT / 2, Bosspos.z);

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

		Bosspos.x += BOSS_WIDTH / 1.4f;
		pVtx += VT_MAX;
	}

	g_pVtxBuffPlaySelect->Unlock();//プレイヤーバッファのアンロック

	PlaySound(SOUND_LABEL_BGM6);
}

//------------------
//終了処理
//------------------
void UninitPlaySelect(void)
{
	//サウンド
	StopSound();
	//テクスチャの破棄
	for (int i = 0; i < NUM_BOSS + 1; i++)
	{
		if (g_apTexturePlaySelect[i] != NULL)
		{
			g_apTexturePlaySelect[i]->Release();
			g_apTexturePlaySelect[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlaySelect != NULL)
	{
		g_pVtxBuffPlaySelect->Release();
		g_pVtxBuffPlaySelect = NULL;
	}
}

//--------------
//更新処理
//--------------
void UpdatePlaySelect(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	static int SelectBoss = 0;

	if (GetKeyboradTrigger(DIK_LEFT) == true || GetKeyboradTrigger(DIK_A) == true || GetJoykeyTrigger(JOYKEY_LEFT, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
	{
		SelectBoss--;
		if (SelectBoss<0)
		{
			SelectBoss = 3;
		}
	}
	else if (GetKeyboradTrigger(DIK_RIGHT) == true || GetKeyboradTrigger(DIK_D) == true || GetJoykeyTrigger(JOYKEY_RIGHT, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
	{
		SelectBoss++;
		if (SelectBoss > 3)
		{
			SelectBoss = 0;
		}
	}
	else if (GetMouseTrigger(MOUSE_LEFT) == true)
	{
		VERTEX_2D* pVtx;//頂点情報ポインタ
		CURSOR* pCursor;
		pCursor = GetCursorIn();

		g_pVtxBuffPlaySelect->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

		pVtx += VT_MAX;

		for (int i = 0; i < NUM_BOSS; i++)
		{
			if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
			{
				FADE fade;
				switch (i)
				{
				case 0:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						SelectBoss = 0;
						//ボス決定
						SetBossNumber(SelectBoss);
						//サウンド
						StopSound();
						//切替
						SetFade(MODE_GAME);
					}
					break;
				case 1:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						SelectBoss = 1;
						//ボス決定
						SetBossNumber(SelectBoss);
						//サウンド
						StopSound();
						//切替
						SetFade(MODE_GAME);
					}
					break;
				case 2:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						SelectBoss = 2;
						//ボス決定
						SetBossNumber(SelectBoss);
						//サウンド
						StopSound();
						//切替
						SetFade(MODE_GAME);
					}
					break;
				case 3:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						SelectBoss = 3;
						//ボス決定
						SetBossNumber(SelectBoss);
						//サウンド
						StopSound();
						//切替
						SetFade(MODE_GAME);
					}
					break;
				}
				break;
			}
			pVtx += VT_MAX;
		}

		g_pVtxBuffPlaySelect->Unlock();//プレイヤーバッファのアンロック
	}


	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoyPovTrigger(POV_LEFT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_LEFT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss--;
				if (SelectBoss < 0)
				{
					SelectBoss = 3;
				}
			}
			else if (GetdJoyPovTrigger(POV_RIGHT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_RIGHT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss++;
				if (SelectBoss > 3)
				{
					SelectBoss = 0;
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoyPovTrigger(POV_LEFT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_LEFT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss--;
				if (SelectBoss < 0)
				{
					SelectBoss = 3;
				}
			}
			else if (GetdJoyPovTrigger(POV_RIGHT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_RIGHT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss++;
				if (SelectBoss > 3)
				{
					SelectBoss = 0;
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoyPovTrigger(POV_LEFT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_LEFT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss--;
				if (SelectBoss < 0)
				{
					SelectBoss = 3;
				}
			}
			else if (GetdJoyPovTrigger(POV_RIGHT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_RIGHT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss++;
				if (SelectBoss > 3)
				{
					SelectBoss = 0;
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoyPovTrigger(POV_LEFT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_LEFT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss--;
				if (SelectBoss < 0)
				{
					SelectBoss = 3;
				}
			}
			else if (GetdJoyPovTrigger(POV_RIGHT, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_RIGHT, STICK_LEFT, (CONTROLLER)i))
			{
				SelectBoss++;
				if (SelectBoss > 3)
				{
					SelectBoss = 0;
				}
			}
		}
	}


	g_pVtxBuffPlaySelect->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < NUM_BOSS; i++)
	{
		pVtx += VT_MAX;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	g_pVtxBuffPlaySelect->Unlock();//プレイヤーバッファのアンロック

	g_pVtxBuffPlaySelect->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	pVtx += VT_MAX * (SelectBoss + 1);

	//カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	g_pVtxBuffPlaySelect->Unlock();//プレイヤーバッファのアンロック

	if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_LEFT) == true)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//ボス決定
			SetBossNumber(SelectBoss);
			//サウンド
			StopSound();
			//切替
			SetFade(MODE_GAME);
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//ボス決定
					SetBossNumber(SelectBoss);
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_GAME);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//ボス決定
					SetBossNumber(SelectBoss);
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_GAME);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_＋, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//ボス決定
					SetBossNumber(SelectBoss);
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_GAME);
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//ボス決定
					SetBossNumber(SelectBoss);
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_GAME);
				}
			}
		}
	}

	if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_BACK, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound();
			//切替
			SetFade(MODE_TITLE);
		}
	}
	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_－, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
	}

}

//-------------------
//描画処理
//-------------------
void DrawPlaySelect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffPlaySelect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < NUM_BOSS + 1; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePlaySelect[i]);

		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			i * VT_MAX,//始まりの番号
			2//ポリゴンの個数
		);
	}
}