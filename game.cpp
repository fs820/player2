//------------------------------------------
//
//ゲームメインの処理[game.cpp]
//Author fuma sato
//
//------------------------------------------

#include"game.h"
#include"player.h"
#include"player2.h"
#include"player3.h"
#include"player4.h"
#include"boss.h"
#include"back.h"
#include"p1back.h"
#include"p2back.h"
#include"p3back.h"
#include"p4back.h"
#include"bullet.h"
#include"effect.h"
#include"exef.h"
#include"explosion.h"
#include"enemy.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"cursor.h"
#include"mark.h"
#include"item.h"

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
LPDIRECT3DTEXTURE9 g_apTextureGame[SELECT_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;//バッファのポインタ
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameState = 0;
int g_BossNumber = 0;//どのボスか
bool g_bClear = false;

//--------------------
//初期化処理
//--------------------
void InitGame(void)
{
	InitBack();//背景
	InitBullet();//弾
	InitEffect();
	InitExef();
	InitExplosion();//爆発
	InitP1Back();
	InitP2Back();
	InitP3Back();
	InitP4Back();
	InitPlayer();//プレイヤー
	if (ControllerNum(CONTYPE_D) >= 2)
	{
		InitPlayer2();
	}
	if (ControllerNum(CONTYPE_D) >= 3)
	{
		InitPlayer3();
	}
	if (ControllerNum(CONTYPE_D) >= 4)
	{
		InitPlayer4();
	}
	InitEnemy();//敵
	InitBoss();
	InitItem();
	InitScore();//スコア
	InitMark();
	SetScore(0,true);//スコア書き換え
	//SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 0);//敵発生
	SetBoss(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), g_BossNumber);//ボス発生

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
		&g_pVtxBuffGame,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRESTART,
		&g_apTextureGame[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRETRY,
		&g_apTextureGame[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTBACK,
		&g_apTextureGame[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTEXIT,
		&g_apTextureGame[3]
	);


	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	posSelect = D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2-SELECT_HEIGHT*1.5f,0.0f);
	g_bClear = false;

	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

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

	g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

	PlaySound(SOUND_LABEL_BGM4);
}

//------------------
//終了処理
//------------------
void UninitGame(void)
{
	g_gameState = GAMESTATE_NONE;

	//サウンド
	StopSound(SOUND_LABEL_BGM4);

	UninitMark();
	UninitScore();//スコア
	UninitItem();
	UninitBoss();
	UninitEnemy();//敵
	UninitPlayer4();
	UninitPlayer3();
	UninitPlayer2();
	UninitPlayer();//プレイヤー
	UninitP4Back();
	UninitP3Back();
	UninitP2Back();
	UninitP1Back();
	UninitExplosion();//爆発
	UninitExef();
	UninitEffect();
	UninitBullet();//弾
	UninitBack();//背景
}

//--------------
//更新処理
//--------------
void UpdateGame(void)
{
	FADE fade;
	fade = GetFade();
	if (fade == FADE_NONE)
	{
		static SELECT SelectNew = SELECT_RESTART;
		static int SpoItemCount = 0;

		if (g_gameState != GAMESTATE_PAUSE)
		{
			SpoItemCount++;
			if (SpoItemCount % SPOITEM_TIME == 0)
			{
				Boss *pBoss;
				pBoss = GetBoss();
				int nRand = rand() % (SCREEN_WIDTH * 2) - SCREEN_WIDTH, nRand2 = rand() % (SCREEN_HEIGHT * 2) - SCREEN_HEIGHT;
				float fRand = (float)((rand() % 11) / 10.0f), fRand2 = (float)((rand() % 11) / 10.0f), fRand3 = (float)((rand() % 11) / 10.0f);
				int nRandT = rand() % 100 + 1;
				TYPE Type = TYPE_NORMAL;
				D3DXCOLOR col;
				if (nRandT <= 40)
				{
					Type = TYPE_3WEY;
					col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				}
				else if (nRandT <= 70)
				{
					Type = TYPE_ALL;
					col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				}
				else if(nRandT <= 90)
				{
					Type = TYPE_SEARCH;
					col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				}
				else if (nRandT <= 98)
				{
					Type = TYPE_BOM;
					col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
				}
				else
				{
					Type = TYPE_SP;
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				SetItem(D3DXVECTOR3(pBoss->pos.x + nRand, pBoss->pos.y + nRand2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, ITEM_SIZE, ITEM_LIFE, true, Type, MAX_ITEM);
			}

			if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true || (GetMousePress(MOUSE_LEFT) == true && GetMousePress(MOUSE_RIGHT) == true && GetMousePress(MOUSE_SENTER) == true))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					g_gameState = GAMESTATE_PAUSE;
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

					StopSound();
				}
			}

			int NumEnemy, NumBoss;
			Player* pPlayer, * pPlayer2,* pPlayer3, * pPlayer4;

			pPlayer = GetPlayer();
			pPlayer2 = GetPlayer2();
			pPlayer3 = GetPlayer3();
			pPlayer4 = GetPlayer4();
			NumEnemy = GetNumEnemy();
			NumBoss = GetNumBoss();

			switch (g_gameState)
			{
			case GAMESTATE_NORMAL:
				switch (ControllerNum(CONTYPE_D))
				{
				case 0:
					if (pPlayer->state == PLAYERSTATE_DIE)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = false;
					}
					else if (NumEnemy <= 0 && NumBoss <= 0)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = true;
					}
					break;
				case 1:
					if (pPlayer->state == PLAYERSTATE_DIE)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = false;
					}
					else if (NumEnemy <= 0 && NumBoss <= 0)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = true;
					}
					break;
				case 2:
					if (pPlayer->state == PLAYERSTATE_DIE && pPlayer2->state == PLAYERSTATE_DIE)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = false;
					}
					else if (NumEnemy <= 0 && NumBoss <= 0)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = true;
					}
					break;
				case 3:
					if (pPlayer->state == PLAYERSTATE_DIE && pPlayer2->state == PLAYERSTATE_DIE && pPlayer3->state == PLAYERSTATE_DIE)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = false;
					}
					else if (NumEnemy <= 0 && NumBoss <= 0)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = true;
					}
					break;
				case 4:
					if (pPlayer->state == PLAYERSTATE_DIE && pPlayer2->state == PLAYERSTATE_DIE && pPlayer3->state == PLAYERSTATE_DIE && pPlayer4->state == PLAYERSTATE_DIE)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = false;
					}
					else if (NumEnemy <= 0 && NumBoss <= 0)
					{
						SetGameState(GAMESTATE_END);
						g_bClear = true;
					}
					break;
				}
				break;
			case GAMESTATE_END:
				g_nCounterGameState++;
				if (g_nCounterGameState >= INTER)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM4);
						//切替
						SetFade(MODE_RESULT);
					}
				}
				break;
			}
		}
		else
		{
			if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
			{
				g_gameState = GAMESTATE_NORMAL;
				PlaySound(SOUND_LABEL_BGM4);
			}
			else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
			{
				FADE fade;
				switch (SelectNew)
				{
				case SELECT_RESTART:
					g_gameState = GAMESTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM4);
					break;
				case SELECT_RETRY:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM4);
						//切替
						SetFade(MODE_GAME);
					}
					break;
				case SELECT_BACK:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM4);
						//切替
						SetFade(MODE_TITLE);
					}
					break;
				case SELECT_EXIT:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//サウンド
						StopSound(SOUND_LABEL_BGM4);
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

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				for (int i = 0; i < SELECT_MAX; i++)
				{
					if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
					{
						FADE fade;
						switch (i)
						{
						case SELECT_RESTART:
							g_gameState = GAMESTATE_NORMAL;
							PlaySound(SOUND_LABEL_BGM4);
							break;
						case SELECT_RETRY:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
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

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
			}
			else if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
			{
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
			}
			else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
			{
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
			}
		}

		for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
		{
			if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
			{
				if (g_gameState != GAMESTATE_PAUSE)
				{
					if (GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i) == true)
					{
						FADE fade;
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							g_gameState = GAMESTATE_PAUSE;
							VERTEX_2D* pVtx;//頂点情報ポインタ

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

							pVtx += VT_MAX * SelectNew;

							//カラー
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

							StopSound();
						}
					}
				}
				else
				{
					if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i) == true)
					{
						g_gameState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
					}
					else if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) == true)
					{
						FADE fade;
						switch (SelectNew)
						{
						case SELECT_RESTART:
							g_gameState = GAMESTATE_NORMAL;
							PlaySound(SOUND_LABEL_BGM4);
							break;
						case SELECT_RETRY:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
				}
			}
			else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
			{
				if (g_gameState != GAMESTATE_PAUSE)
				{
					if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i) == true)
					{
						FADE fade;
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							g_gameState = GAMESTATE_PAUSE;
							VERTEX_2D* pVtx;//頂点情報ポインタ

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

							pVtx += VT_MAX * SelectNew;

							//カラー
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

							StopSound();
						}
					}
				}
				else
				{
					if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i) == true)
					{
						g_gameState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
					}
					else if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) == true)
					{
						FADE fade;
						switch (SelectNew)
						{
						case SELECT_RESTART:
							g_gameState = GAMESTATE_NORMAL;
							PlaySound(SOUND_LABEL_BGM4);
							break;
						case SELECT_RETRY:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
				}
			}
			else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
			{
				if (g_gameState != GAMESTATE_PAUSE)
				{
					if (GetdJoykeyTrigger(NINKEY_＋, (CONTROLLER)i) == true)
					{
						FADE fade;
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							g_gameState = GAMESTATE_PAUSE;
							VERTEX_2D* pVtx;//頂点情報ポインタ

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

							pVtx += VT_MAX * SelectNew;

							//カラー
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

							StopSound();
						}
					}
				}
				else
				{
					if (GetdJoykeyTrigger(NINKEY_−, (CONTROLLER)i) == true)
					{
						g_gameState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
					}
					else if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) == true)
					{
						FADE fade;
						switch (SelectNew)
						{
						case SELECT_RESTART:
							g_gameState = GAMESTATE_NORMAL;
							PlaySound(SOUND_LABEL_BGM4);
							break;
						case SELECT_RETRY:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
				}
			}
			else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
			{
				if (g_gameState != GAMESTATE_PAUSE)
				{
					if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i) == true)
					{
						FADE fade;
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							g_gameState = GAMESTATE_PAUSE;
							VERTEX_2D* pVtx;//頂点情報ポインタ

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

							pVtx += VT_MAX * SelectNew;

							//カラー
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

							StopSound();
						}
					}
				}
				else
				{
					if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i) == true)
					{
						g_gameState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
					}
					else if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) == true)
					{
						FADE fade;
						switch (SelectNew)
						{
						case SELECT_RESTART:
							g_gameState = GAMESTATE_NORMAL;
							PlaySound(SOUND_LABEL_BGM4);
							break;
						case SELECT_RETRY:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
								//切替
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//サウンド
								StopSound(SOUND_LABEL_BGM4);
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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
					}
				}
			}
		}

		if (g_gameState != GAMESTATE_PAUSE)
		{
			UpdateBack();//背景
			UpdateBullet();//弾
			UpdatePlayer();//プレイヤー
			if (ControllerNum(CONTYPE_D) >= 2)
			{
				UpdatePlayer2();
			}
			if (ControllerNum(CONTYPE_D) >= 3)
			{
				UpdatePlayer3();
			}
			if (ControllerNum(CONTYPE_D) >= 4)
			{
				UpdatePlayer4();
			}
			UpdateEnemy();//敵
			UpdateBoss();
			UpdateEffect();
			UpdateExef();
			UpdateExplosion();//爆発
			if (bOberP1())
			{
				UpdateP1Back();
			}
			if (bOberP2())
			{
				UpdateP2Back();
			}
			if (bOberP3())
			{
				UpdateP3Back();
			}
			if (bOberP4())
			{
				UpdateP4Back();
			}
			UpdateItem();
			UpdateScore();//スコア
			UpdateMark();
		}
	}
}

//-------------------
//描画処理
//-------------------
void DrawGame(void)
{

	DrawBack();//背景
	if (bOberP1())
	{
		DrawP1Back();
	}
	if (bOberP2())
	{
		DrawP2Back();
	}
	if (bOberP3())
	{
		DrawP3Back();
	}
	if (bOberP4())
	{
		DrawP4Back();
	}
	DrawBullet();//弾
	DrawEffect();
	DrawExef();
	DrawExplosion();//爆発
	DrawPlayer();//プレイヤー
	if (ControllerNum(CONTYPE_D) >= 2)
	{
		DrawPlayer2();
	}
	if (ControllerNum(CONTYPE_D) >= 3)
	{
		DrawPlayer3();
	}
	if (ControllerNum(CONTYPE_D) >= 4)
	{
		DrawPlayer4();
	}
	DrawEnemy();//敵
	DrawBoss();
	DrawItem();
	DrawScore();//スコア
	DrawMark();

	if(g_gameState==GAMESTATE_PAUSE)
	{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

        //デバイスの取得
		pDevice = GetDevice();

		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffGame, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SELECT_MAX; i++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureGame[i]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i*VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//----------------------
//ゲーム状態切替
//----------------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//-----------------------
//ゲーム状態取得
//-----------------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//---------------------------
//どのボスか
//---------------------------
void SetBossNumber(int Boss)
{
	g_BossNumber = Boss;
}

//---------------------------
//ボス取得	
//---------------------------
int GetBossNumber(void)
{
	return g_BossNumber;
}

//---------------------
//クリア取得
//---------------------
bool GetClear(void)
{
	return g_bClear;
}