//------------------------------------------
//
//チュートリアルの処理[tuto.cpp]
//Author fuma sato
//
//------------------------------------------

#include"tuto.h"
#include"player.h"
#include"back.h"
#include"bullet.h"
#include"explosion.h"
#include"enemy.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"cursor.h"
#include"effect.h"
#include"exef.h"

#define SELECT_WIDTH (512)
#define SELECT_HEIGHT (128)

typedef enum
{
	SELECT_RESTART = 0,
	SELECT_RETRY,
	SELECT_BACK,
	SELECT_EXIT,
	SELECT_MAX,
}SELECT;

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTuto[SELECT_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTuto = NULL;//バッファのポインタ
GAMESTATE g_TutoState = GAMESTATE_NONE;
int g_nCounterTutoState = 0;

//--------------------
//初期化処理
//--------------------
void InitTuto(void)
{
	InitBack();//背景
	InitBullet();//弾
	InitPlayer();//プレイヤー
	InitEnemy();//敵
	InitEffect();
	InitExef();
	InitExplosion();//爆発
	InitScore();//スコア
	SetScore(0,true);//スコア書き換え
	SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 0);//敵発生
	SetEnemy(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), 1);//敵発生
	SetEnemy(D3DXVECTOR3(1000.0f, 350.0f, 0.0f), 2);//敵発生
	SetEnemy(D3DXVECTOR3(700.0f, 350.0f, 0.0f), 3);//敵発生

	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 posSelect;//スコアの位置

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SELECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTuto,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRESTART,
		&g_apTextureTuto[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRETRY,
		&g_apTextureTuto[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTBACK,
		&g_apTextureTuto[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTEXIT,
		&g_apTextureTuto[3]
	);

	g_TutoState = GAMESTATE_NORMAL;
	g_nCounterTutoState = 0;
	posSelect = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SELECT_HEIGHT * 1.5f, 0.0f);

	g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < SELECT_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

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

	g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

	PlaySound(SOUND_LABEL_BGM3);

}

//--------------------
//終了処理
//--------------------
void UninitTuto(void)
{
	g_TutoState = GAMESTATE_NONE;

	//サウンド
	StopSound(SOUND_LABEL_BGM3);
	SetScore(0,false);

	UninitScore();//スコア
	UninitExplosion();//爆発
	UninitExef();
	UninitEffect();
	UninitEnemy();//敵
	UninitPlayer();//プレイヤー
	UninitBullet();//弾
	UninitBack();//背景
}

//--------------------
//更新処理
//--------------------
void UpdateTuto(void)
{
	static SELECT SelectNew = SELECT_RESTART;

	if (g_TutoState != GAMESTATE_PAUSE)
	{
		if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START,CONTROLLER_MAX) == true || (GetMousePress(MOUSE_LEFT) == true && GetMousePress(MOUSE_RIGHT) == true && GetMousePress(MOUSE_SENTER) == true))
		{
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_TutoState = GAMESTATE_PAUSE;
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

				StopSound();
			}
		}

		int NumEnemy;
		Player* pPlayer;

		pPlayer = GetPlayer();
		NumEnemy = GetNumEnemy();

		switch (g_TutoState)
		{
		case GAMESTATE_NORMAL:
			if (pPlayer->state == PLAYERSTATE_DIE)
			{
				SetTutoState(GAMESTATE_END);
			}
			else if (NumEnemy <= 0)
			{
				SetTutoState(GAMESTATE_END);
			}
			break;
		case GAMESTATE_END:
			g_nCounterTutoState++;
			if (g_nCounterTutoState >= INTER)
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
					//切替
					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}
	else
	{
		if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			g_TutoState = GAMESTATE_NORMAL;
			PlaySound(SOUND_LABEL_BGM3);
		}
		else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (SelectNew)
			{
			case SELECT_RESTART:
				g_TutoState = GAMESTATE_NORMAL;
				PlaySound(SOUND_LABEL_BGM3);
				break;
			case SELECT_RETRY:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
					//切替
					SetFade(MODE_TUTO);
				}
				break;
			case SELECT_BACK:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
					//切替
					SetFade(MODE_TITLE);
				}
				break;
			case SELECT_EXIT:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
					//切替
					HWND hWnd;
					hWnd = GethWnd();
					PostMessage(hWnd, WM_QUIT, 0, 0);
				}
				break;
			}
		}
		else if (GetMouseTrigger(MOUSE_LEFT) == true)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ
			CURSOR* pCursor;
			pCursor = GetCursorIn();

			g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			for (int i = 0; i < SELECT_MAX; i++)
			{
				if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
				{
					FADE fade;
					switch (i)
					{
					case SELECT_RESTART:
						g_TutoState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM3);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TUTO);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
		}
		else if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ

			g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

			switch (SelectNew)
			{
			case SELECT_RESTART:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_RETRY:
				SelectNew = SELECT_RESTART;
				break;
			case SELECT_BACK:
				SelectNew = SELECT_RETRY;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_BACK;
				break;
			}

			g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
		}
		else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN,STICK_LEFT,CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ

			g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

			switch (SelectNew)
			{
			case SELECT_RESTART:
				SelectNew = SELECT_RETRY;
				break;
			case SELECT_RETRY:
				SelectNew = SELECT_BACK;
				break;
			case SELECT_BACK:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_RESTART;
				break;
			}
			g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
		}
	}

	if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_BACK,CONTROLLER_MAX) == true)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound(SOUND_LABEL_BGM3);
			//切替
			SetFade(MODE_TITLE);
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (g_TutoState != GAMESTATE_PAUSE)
			{
				if (GetdJoykeyTrigger(ELEKEY_START,(CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_TutoState = GAMESTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i) == true)
				{
					g_TutoState = GAMESTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM3);
				}
				else if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_RESTART:
						g_TutoState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM3);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TUTO);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
				}
			}

			if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i) == true)
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		    {
		    if (g_TutoState != GAMESTATE_PAUSE)
		    {
				if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_TutoState = GAMESTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
		    else
		    {
				if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i) == true)
				{
					g_TutoState = GAMESTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM3);
				}
				else if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_RESTART:
						g_TutoState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM3);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TUTO);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
				}
			}

		    if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i) == true)
		    {
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
		    if (g_TutoState != GAMESTATE_PAUSE)
		    {
				if (GetdJoykeyTrigger(NINKEY_＋, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_TutoState = GAMESTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
		    else
		    {
				if (GetdJoykeyTrigger(NINKEY_−, (CONTROLLER)i) == true)
				{
					g_TutoState = GAMESTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM3);
				}
				else if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_RESTART:
						g_TutoState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM3);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TUTO);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
				}
			}

		    if (GetdJoykeyTrigger(NINKEY_−, (CONTROLLER)i) == true)
		    {
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
		    if (g_TutoState != GAMESTATE_PAUSE)
		    {
				if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_TutoState = GAMESTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
		    else
		    {
			if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i) == true)
			{
				g_TutoState = GAMESTATE_NORMAL;
				PlaySound(SOUND_LABEL_BGM3);
			}
			else if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) == true)
			{
				FADE fade;
				switch (SelectNew)
				{
				case SELECT_RESTART:
					g_TutoState = GAMESTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM3);
					break;
				case SELECT_RETRY:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM3);
						//切替
						SetFade(MODE_TUTO);
					}
					break;
				case SELECT_BACK:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM3);
						//切替
						SetFade(MODE_TITLE);
					}
					break;
				case SELECT_EXIT:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM3);
						//切替
						HWND hWnd;
						hWnd = GethWnd();
						PostMessage(hWnd, WM_QUIT, 0, 0);
					}
					break;
				}
			}
			else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
			{
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

				switch (SelectNew)
				{
				case SELECT_RESTART:
					SelectNew = SELECT_EXIT;
					break;
				case SELECT_RETRY:
					SelectNew = SELECT_RESTART;
					break;
				case SELECT_BACK:
					SelectNew = SELECT_RETRY;
					break;
				case SELECT_EXIT:
					SelectNew = SELECT_BACK;
					break;
				}

				g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
			}
			else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
			{
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック

				switch (SelectNew)
				{
				case SELECT_RESTART:
					SelectNew = SELECT_RETRY;
					break;
				case SELECT_RETRY:
					SelectNew = SELECT_BACK;
					break;
				case SELECT_BACK:
					SelectNew = SELECT_EXIT;
					break;
				case SELECT_EXIT:
					SelectNew = SELECT_RESTART;
					break;
				}
				g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffTuto->Unlock();//プレイヤーバッファのアンロック
			}
		}

		    if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i) == true)
		    {
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound(SOUND_LABEL_BGM3);
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
							StopSound(SOUND_LABEL_BGM3);
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
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TITLE);
						}
					}
				}
				else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
				{
					if (GetdJoykeyTrigger(NINKEY_−, (CONTROLLER)i))
					{
						FADE fade;
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound(SOUND_LABEL_BGM3);
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
							StopSound(SOUND_LABEL_BGM3);
							//切替
							SetFade(MODE_TITLE);
						}
					}
				}
			}

		}
	}

	if (g_TutoState != GAMESTATE_PAUSE)
	{
		UpdateBack();//背景
		UpdateBullet();//弾
		UpdatePlayer();//プレイヤー
		UpdateEnemy();//敵
		UpdateEffect();
		UpdateExef();
		UpdateExplosion();//爆発
		UpdateScore();//スコア
	}
}

//--------------------
//描画処理
//--------------------
void DrawTuto(void)
{
	DrawBack();//背景
	DrawBullet();//弾
	DrawPlayer();//プレイヤー
	DrawEnemy();//敵
	DrawEffect();
	DrawExef();
	DrawExplosion();//爆発
	DrawScore();//スコア

	if (g_TutoState == GAMESTATE_PAUSE)
	{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffTuto, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SELECT_MAX; i++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTuto[i]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//----------------------
//ゲーム状態切替
//----------------------
void SetTutoState(GAMESTATE state)
{
	g_TutoState = state;
	g_nCounterTutoState = 0;
}

//-----------------------
//ゲーム状態取得
//-----------------------
GAMESTATE GetTutoState(void)
{
	return g_TutoState;
}