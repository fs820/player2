//------------------------------------------
//
//�Q�[�����C���̏���[game.cpp]
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

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureGame[SELECT_MAX] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;//�o�b�t�@�̃|�C���^
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameState = 0;
int g_BossNumber = 0;//�ǂ̃{�X��
bool g_bClear = false;

//--------------------
//����������
//--------------------
void InitGame(void)
{
	InitBack();//�w�i
	InitBullet();//�e
	InitEffect();
	InitExef();
	InitExplosion();//����
	InitP1Back();
	InitP2Back();
	InitP3Back();
	InitP4Back();
	InitPlayer();//�v���C���[
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
	InitEnemy();//�G
	InitBoss();
	InitItem();
	InitScore();//�X�R�A
	InitMark();
	SetScore(0,true);//�X�R�A��������
	//SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 0);//�G����
	SetBoss(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), g_BossNumber);//�{�X����

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^
	D3DXVECTOR3 posSelect;//�X�R�A�̈ʒu

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SELECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGame,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRESTART,
		&g_apTextureGame[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRETRY,
		&g_apTextureGame[1]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTBACK,
		&g_apTextureGame[2]
	);

	//�e�N�X�`���̓ǂݍ���
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

	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < SELECT_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;

	}

	g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	PlaySound(SOUND_LABEL_BGM4);
}

//------------------
//�I������
//------------------
void UninitGame(void)
{
	g_gameState = GAMESTATE_NONE;

	//�T�E���h
	StopSound(SOUND_LABEL_BGM4);

	UninitMark();
	UninitScore();//�X�R�A
	UninitItem();
	UninitBoss();
	UninitEnemy();//�G
	UninitPlayer4();
	UninitPlayer3();
	UninitPlayer2();
	UninitPlayer();//�v���C���[
	UninitP4Back();
	UninitP3Back();
	UninitP2Back();
	UninitP1Back();
	UninitExplosion();//����
	UninitExef();
	UninitEffect();
	UninitBullet();//�e
	UninitBack();//�w�i
}

//--------------
//�X�V����
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
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
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
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_GAME);
					}
					break;
				case SELECT_BACK:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_TITLE);
					}
					break;
				case SELECT_EXIT:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						HWND hWnd;
						hWnd = GethWnd();
						PostMessage(hWnd, WM_QUIT, 0, 0);
					}
					break;
				}
			}
			else if (GetMouseTrigger(MOUSE_LEFT) == true)
			{
				VERTEX_2D* pVtx;//���_���|�C���^
				CURSOR* pCursor;
				pCursor = GetCursorIn();

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

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
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								HWND hWnd;
								hWnd = GethWnd();
								PostMessage(hWnd, WM_QUIT, 0, 0);
							}
							break;
						}
					}
					pVtx += VT_MAX;
				}

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
			else if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
			{
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
			else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
			{
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
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
							VERTEX_2D* pVtx;//���_���|�C���^

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

							pVtx += VT_MAX * SelectNew;

							//�J���[
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								HWND hWnd;
								hWnd = GethWnd();
								PostMessage(hWnd, WM_QUIT, 0, 0);
							}
							break;
						}
					}
					else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
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
							VERTEX_2D* pVtx;//���_���|�C���^

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

							pVtx += VT_MAX * SelectNew;

							//�J���[
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								HWND hWnd;
								hWnd = GethWnd();
								PostMessage(hWnd, WM_QUIT, 0, 0);
							}
							break;
						}
					}
					else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
					}
				}
			}
			else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
			{
				if (g_gameState != GAMESTATE_PAUSE)
				{
					if (GetdJoykeyTrigger(NINKEY_�{, (CONTROLLER)i) == true)
					{
						FADE fade;
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							g_gameState = GAMESTATE_PAUSE;
							VERTEX_2D* pVtx;//���_���|�C���^

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

							pVtx += VT_MAX * SelectNew;

							//�J���[
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

							StopSound();
						}
					}
				}
				else
				{
					if (GetdJoykeyTrigger(NINKEY_�|, (CONTROLLER)i) == true)
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
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								HWND hWnd;
								hWnd = GethWnd();
								PostMessage(hWnd, WM_QUIT, 0, 0);
							}
							break;
						}
					}
					else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
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
							VERTEX_2D* pVtx;//���_���|�C���^

							g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

							pVtx += VT_MAX * SelectNew;

							//�J���[
							pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

							g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_GAME);
							}
							break;
						case SELECT_BACK:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								SetFade(MODE_TITLE);
							}
							break;
						case SELECT_EXIT:
							fade = GetFade();
							if (fade == FADE_NONE)
							{
								//�T�E���h
								StopSound(SOUND_LABEL_BGM4);
								//�ؑ�
								HWND hWnd;
								hWnd = GethWnd();
								PostMessage(hWnd, WM_QUIT, 0, 0);
							}
							break;
						}
					}
					else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
					}
					else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
					{
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

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
						g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * SelectNew;

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
					}
				}
			}
		}

		if (g_gameState != GAMESTATE_PAUSE)
		{
			UpdateBack();//�w�i
			UpdateBullet();//�e
			UpdatePlayer();//�v���C���[
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
			UpdateEnemy();//�G
			UpdateBoss();
			UpdateEffect();
			UpdateExef();
			UpdateExplosion();//����
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
			UpdateScore();//�X�R�A
			UpdateMark();
		}
	}
}

//-------------------
//�`�揈��
//-------------------
void DrawGame(void)
{

	DrawBack();//�w�i
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
	DrawBullet();//�e
	DrawEffect();
	DrawExef();
	DrawExplosion();//����
	DrawPlayer();//�v���C���[
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
	DrawEnemy();//�G
	DrawBoss();
	DrawItem();
	DrawScore();//�X�R�A
	DrawMark();

	if(g_gameState==GAMESTATE_PAUSE)
	{
		LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

        //�f�o�C�X�̎擾
		pDevice = GetDevice();

		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffGame, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SELECT_MAX; i++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureGame[i]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i*VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//----------------------
//�Q�[����Ԑؑ�
//----------------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//-----------------------
//�Q�[����Ԏ擾
//-----------------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//---------------------------
//�ǂ̃{�X��
//---------------------------
void SetBossNumber(int Boss)
{
	g_BossNumber = Boss;
}

//---------------------------
//�{�X�擾	
//---------------------------
int GetBossNumber(void)
{
	return g_BossNumber;
}

//---------------------
//�N���A�擾
//---------------------
bool GetClear(void)
{
	return g_bClear;
}