//---------------------------------------
//
//�A�C�e���\������[item.cpp]
//Author fuma sato
//
//---------------------------------------

#include"item.h"
#include"player.h"
#include"player2.h"
#include"player3.h"
#include"player4.h"
#include"boss.h"
#include"vs.h"

//�A�C�e���\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	int nLife;//����
	bool bSenter;
	TYPE Type;
	bool bDisp;
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nNumber;
}Item;

//�O���[�o��
LPDIRECT3DTEXTURE9 g_apTextureItem = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;//�o�b�t�@�̃|�C���^
Item g_aItem[MAX_ITEM];//�A�C�e���̏��
int g_nItemLifeDef[MAX_ITEM] = { 0 };

//-----------------
//�A�C�e���̏���������
//-----------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_ITEM; i++)
	{
		g_aItem[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aItem[i].fRadius = 0.0f;
		g_aItem[i].nLife = 0;
		g_aItem[i].Type = TYPE_MAX;
		g_aItem[i].bDisp = true;
		g_aItem[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		g_aItem[i].nNumber = MAX_ITEM;
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_ITEM,
		&g_apTextureItem
	);

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

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

		pVtx += VT_MAX;//�|���S��1���i�߂�
	}
	g_pVtxBuffItem->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�A�C�e���̏I������
//----------------
void UninitItem(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureItem != NULL)
	{
		g_apTextureItem->Release();
		g_apTextureItem = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//----------------
//�A�C�e���̍X�V����
//----------------
void UpdateItem(void)
{
	VERTEX_2D* pVtx;
	static D3DXVECTOR3 Oldpos[MAX_ITEM];

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == true)
		{//�A�C�e�����g�p����Ă���
			Oldpos[i] = g_aItem[i].pos;
			g_aItem[i].nLife--;

			for (int i2 = 0; i2 < MAX_ITEM; i2++)
			{
				if (g_aItem[i2].nNumber == i)
				{
					g_aItem[i2].bUse = false;
				}
			}

			if (g_aItem[i].bSenter)
			{
				if (bOberP1())
				{
					Player* pPlayer;
					pPlayer = GetPlayer();
					float Xlong = 0.0f, Ylong = 0.0f;

					Xlong = pPlayer->pos[SENTER].x - g_aItem[i].pos.x;
					Ylong = pPlayer->pos[SENTER].y - g_aItem[i].pos.y;
					if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
					{
						if (GetMode()==MODE_VS)
						{
							SetItem(D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA_VS, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA_VS, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA_VS, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
						else
						{
							SetItem(D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
					}
				}
				if (bOberP2())
				{
					Player* pPlayer2;
					pPlayer2 = GetPlayer2();
					float Xlong = 0.0f, Ylong = 0.0f;

					Xlong = pPlayer2->pos[SENTER].x - g_aItem[i].pos.x;
					Ylong = pPlayer2->pos[SENTER].y - g_aItem[i].pos.y;
					if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
					{
						float fRand = (float)((rand() % 11) / 10.0f), fRand2 = (float)((rand() % 11) / 10.0f), fRand3 = (float)((rand() % 11) / 10.0f);
						TYPE Rand = (TYPE)(rand() % TYPE_MAX);
						if (GetMode() == MODE_VS)
						{
							SetItem(D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA_VS, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA_VS, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA_VS, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
						else
						{
							SetItem(D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
					}
				}
				if (bOberP3())
				{
					Player* pPlayer3;
					pPlayer3 = GetPlayer3();
					float Xlong = 0.0f, Ylong = 0.0f;

					Xlong = pPlayer3->pos[SENTER].x - g_aItem[i].pos.x;
					Ylong = pPlayer3->pos[SENTER].y - g_aItem[i].pos.y;
					if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
					{
						float fRand = (float)((rand() % 11) / 10.0f), fRand2 = (float)((rand() % 11) / 10.0f), fRand3 = (float)((rand() % 11) / 10.0f);
						TYPE Rand = (TYPE)(rand() % TYPE_MAX);
						if (GetMode() == MODE_VS)
						{
							SetItem(D3DXVECTOR3(pPlayer3->pos[RIGHTUP].x - Xlong * SPLIT_INA_VS, pPlayer3->pos[RIGHTUP].y - Ylong * SPLIT_INA_VS, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA_VS, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
						else
						{
							SetItem(D3DXVECTOR3(pPlayer3->pos[RIGHTUP].x - Xlong * SPLIT_INA, pPlayer3->pos[RIGHTUP].y - Ylong * SPLIT_INA, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
					}
				}
				if (bOberP4())
				{
					Player* pPlayer4;
					pPlayer4 = GetPlayer4();
					float Xlong = 0.0f, Ylong = 0.0f;

					Xlong = pPlayer4->pos[SENTER].x - g_aItem[i].pos.x;
					Ylong = pPlayer4->pos[SENTER].y - g_aItem[i].pos.y;
					if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
					{
						float fRand = (float)((rand() % 11) / 10.0f), fRand2 = (float)((rand() % 11) / 10.0f), fRand3 = (float)((rand() % 11) / 10.0f);
						TYPE Rand = (TYPE)(rand() % TYPE_MAX);
						if (GetMode() == MODE_VS)
						{
							SetItem(D3DXVECTOR3(pPlayer4->pos[RIGHTUP].x - Xlong * SPLIT_INA_VS, pPlayer4->pos[RIGHTUP].y - Ylong * SPLIT_INA_VS, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA_VS, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
						else
						{
							SetItem(D3DXVECTOR3(pPlayer4->pos[RIGHTUP].x - Xlong * SPLIT_INA, pPlayer4->pos[RIGHTUP].y - Ylong * SPLIT_INA, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aItem[i].col, ITEM_SIZE * SPLIT_INA, ITEM_LIFE, false, g_aItem[i].Type, i);
						}
					}
				}

				Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;
				pPlayer = GetPlayer();
				pPlayer2 = GetPlayer2();
				pPlayer3 = GetPlayer3();
				pPlayer4 = GetPlayer4();
				if (pPlayer->pos[SENTER].x > g_aItem[i].pos.x - g_aItem[i].fRadius && pPlayer->pos[SENTER].x< g_aItem[i].pos.x + g_aItem[i].fRadius && pPlayer->pos[SENTER].y> g_aItem[i].pos.y - g_aItem[i].fRadius && pPlayer->pos[SENTER].y < g_aItem[i].pos.y + g_aItem[i].fRadius)
				{
					pPlayer->BulletType = g_aItem[i].Type;
					switch (g_aItem[i].Type)
					{
					case TYPE_3WEY:
						pPlayer->BulletCount = WEY_NUM;
						break;
					case TYPE_ALL:
						pPlayer->BulletCount = ALL_NUM;
						break;
					case TYPE_SEARCH:
						pPlayer->BulletCount = SEARCH_NUM;
						break;
					case TYPE_BOM:
						pPlayer->BulletCount = BOM_NUM;
						break;
					case TYPE_SP:
						pPlayer->BulletCount = SP_NUM;
						break;
					}

					g_aItem[i].bUse = false;
					for (int i2 = 0; i2 < MAX_ITEM; i2++)
					{
						if (g_aItem[i2].nNumber == i)
						{
							g_aItem[i2].bUse = false;
						}
					}
				}
				if (pPlayer2->pos[SENTER].x > g_aItem[i].pos.x - g_aItem[i].fRadius && pPlayer2->pos[SENTER].x< g_aItem[i].pos.x + g_aItem[i].fRadius && pPlayer2->pos[SENTER].y> g_aItem[i].pos.y - g_aItem[i].fRadius && pPlayer2->pos[SENTER].y < g_aItem[i].pos.y + g_aItem[i].fRadius)
				{
					pPlayer2->BulletType = g_aItem[i].Type;
					switch (g_aItem[i].Type)
					{
					case TYPE_3WEY:
						pPlayer2->BulletCount = WEY_NUM;
						break;
					case TYPE_ALL:
						pPlayer2->BulletCount = ALL_NUM;
						break;
					case TYPE_SEARCH:
						pPlayer2->BulletCount = SEARCH_NUM;
						break;
					case TYPE_BOM:
						pPlayer2->BulletCount = BOM_NUM;
						break;
					case TYPE_SP:
						pPlayer2->BulletCount = SP_NUM;
						break;
					}

					g_aItem[i].bUse = false;
					for (int i2 = 0; i2 < MAX_ITEM; i2++)
					{
						if (g_aItem[i2].nNumber == i)
						{
							g_aItem[i2].bUse = false;
						}
					}
				}
				if (pPlayer3->pos[SENTER].x > g_aItem[i].pos.x - g_aItem[i].fRadius && pPlayer3->pos[SENTER].x< g_aItem[i].pos.x + g_aItem[i].fRadius && pPlayer3->pos[SENTER].y> g_aItem[i].pos.y - g_aItem[i].fRadius && pPlayer3->pos[SENTER].y < g_aItem[i].pos.y + g_aItem[i].fRadius)
				{
					pPlayer3->BulletType = g_aItem[i].Type;
					switch (g_aItem[i].Type)
					{
					case TYPE_3WEY:
						pPlayer3->BulletCount = WEY_NUM;
						break;
					case TYPE_ALL:
						pPlayer3->BulletCount = ALL_NUM;
						break;
					case TYPE_SEARCH:
						pPlayer3->BulletCount = SEARCH_NUM;
						break;
					case TYPE_BOM:
						pPlayer3->BulletCount = BOM_NUM;
						break;
					case TYPE_SP:
						pPlayer3->BulletCount = SP_NUM;
						break;
					}

					g_aItem[i].bUse = false;
					for (int i2 = 0; i2 < MAX_ITEM; i2++)
					{
						if (g_aItem[i2].nNumber == i)
						{
							g_aItem[i2].bUse = false;
						}
					}
				}
				if (pPlayer4->pos[SENTER].x > g_aItem[i].pos.x - g_aItem[i].fRadius && pPlayer4->pos[SENTER].x< g_aItem[i].pos.x + g_aItem[i].fRadius && pPlayer4->pos[SENTER].y> g_aItem[i].pos.y - g_aItem[i].fRadius && pPlayer4->pos[SENTER].y < g_aItem[i].pos.y + g_aItem[i].fRadius)
				{
					pPlayer4->BulletType = g_aItem[i].Type;
					switch (g_aItem[i].Type)
					{
					case TYPE_3WEY:
						pPlayer4->BulletCount = WEY_NUM;
						break;
					case TYPE_ALL:
						pPlayer4->BulletCount = ALL_NUM;
						break;
					case TYPE_SEARCH:
						pPlayer4->BulletCount = SEARCH_NUM;
						break;
					case TYPE_BOM:
						pPlayer4->BulletCount = BOM_NUM;
						break;
					case TYPE_SP:
						pPlayer4->BulletCount = SP_NUM;
						break;
					}

					g_aItem[i].bUse = false;
					for (int i2 = 0; i2 < MAX_ITEM; i2++)
					{
						if (g_aItem[i2].nNumber == i)
						{
							g_aItem[i2].bUse = false;
						}
					}
				}
				Boss *pBoss;
				pBoss = GetBoss();
				g_aItem[i].pos.x += g_aItem[i].move.x;
				g_aItem[i].pos.y += g_aItem[i].move.y;
				g_aItem[i].pos.x -= pBoss->move.x * PLAYER_FIELD_INA;
				g_aItem[i].pos.y -= pBoss->move.y * PLAYER_FIELD_INA;
			}

			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			//���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

			g_pVtxBuffItem->Unlock();//�o�b�t�@�̃A�����b�N

			if (g_aItem[i].nLife <= 0)
			{
				g_aItem[i].bUse = false;
				for (int i2 = 0; i2 < MAX_ITEM; i2++)
				{
					if (g_aItem[i2].nNumber == i)
					{
						g_aItem[i2].bUse = false;
					}
				}
			}

			if (!g_aItem[i].bSenter)
			{
				if (GetMode() == MODE_VS)
				{
					if (Oldpos[i].x < SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y < SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aItem[i].pos.x >= SCREEN_WIDTH * SPLIT_INA_VS - SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA_VS - SPLIT_ITEMSP || g_aItem[i].pos.x <= 0 + SPLIT_ITEMSP || g_aItem[i].pos.y <= 0 + SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP1())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer = GetPlayer();

						Xmove = pPlayer->pos[SENTER].x - pPlayer->Oldpos.x;
						Ymove = pPlayer->pos[SENTER].y - pPlayer->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
					else if (Oldpos[i].x > SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y < SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aItem[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS + SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA_VS - SPLIT_ITEMSP || g_aItem[i].pos.x >= SCREEN_WIDTH - SPLIT_ITEMSP || g_aItem[i].pos.y <= 0 + SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP2())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer2;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer2 = GetPlayer2();

						Xmove = pPlayer2->pos[SENTER].x - pPlayer2->Oldpos.x;
						Ymove = pPlayer2->pos[SENTER].y - pPlayer2->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
					else if (Oldpos[i].x < SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y > SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aItem[i].pos.x >= SCREEN_WIDTH * SPLIT_INA_VS - SPLIT_ITEMSP || g_aItem[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS + SPLIT_ITEMSP || g_aItem[i].pos.x <= 0 + SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT - SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP3())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer3;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer3 = GetPlayer3();

						Xmove = pPlayer3->pos[SENTER].x - pPlayer3->Oldpos.x;
						Ymove = pPlayer3->pos[SENTER].y - pPlayer3->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
					else if (Oldpos[i].x > SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y > SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aItem[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS + SPLIT_ITEMSP || g_aItem[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS + SPLIT_ITEMSP || g_aItem[i].pos.x >= SCREEN_WIDTH - SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT - SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP4())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer4;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer4 = GetPlayer4();

						Xmove = pPlayer4->pos[SENTER].x - pPlayer4->Oldpos.x;
						Ymove = pPlayer4->pos[SENTER].y - pPlayer4->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
				}
				else
				{
					if (Oldpos[i].x < SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y < SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aItem[i].pos.x >= SCREEN_WIDTH * SPLIT_INA - SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA - SPLIT_ITEMSP || g_aItem[i].pos.x <= 0 + SPLIT_ITEMSP || g_aItem[i].pos.y <= 0 + SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP1())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer = GetPlayer();

						Xmove = pPlayer->pos[SENTER].x - pPlayer->Oldpos.x;
						Ymove = pPlayer->pos[SENTER].y - pPlayer->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA;
					}
					else if (Oldpos[i].x > SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y < SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aItem[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA + SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA - SPLIT_ITEMSP || g_aItem[i].pos.x >= SCREEN_WIDTH - SPLIT_ITEMSP || g_aItem[i].pos.y <= 0 + SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP2())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer2;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer2 = GetPlayer2();

						Xmove = pPlayer2->pos[SENTER].x - pPlayer2->Oldpos.x;
						Ymove = pPlayer2->pos[SENTER].y - pPlayer2->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA;
					}
					else if (Oldpos[i].x < SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y > SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aItem[i].pos.x >= SCREEN_WIDTH * SPLIT_INA - SPLIT_ITEMSP || g_aItem[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA + SPLIT_ITEMSP || g_aItem[i].pos.x <= 0 + SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT - SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP3())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer3;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer3 = GetPlayer3();

						Xmove = pPlayer3->pos[SENTER].x - pPlayer3->Oldpos.x;
						Ymove = pPlayer3->pos[SENTER].y - pPlayer3->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA;
					}
					else if (Oldpos[i].x > SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y > SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aItem[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA + SPLIT_ITEMSP || g_aItem[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA + SPLIT_ITEMSP || g_aItem[i].pos.x >= SCREEN_WIDTH - SPLIT_ITEMSP || g_aItem[i].pos.y >= SCREEN_HEIGHT - SPLIT_ITEMSP)
						{
							g_aItem[i].bUse = false;
						}

						if (!bOberP4())
						{
							g_aItem[i].bUse = false;
						}

						Player* pPlayer4;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer4 = GetPlayer4();

						Xmove = pPlayer4->pos[SENTER].x - pPlayer4->Oldpos.x;
						Ymove = pPlayer4->pos[SENTER].y - pPlayer4->Oldpos.y;

						g_aItem[i].pos.x -= Xmove * SPLIT_INA;
						g_aItem[i].pos.y -= Ymove * SPLIT_INA;
					}
				}
			}
			else
			{
				if (g_aItem[i].pos.x <= 0 || g_aItem[i].pos.x >= SCREEN_WIDTH || g_aItem[i].pos.y <= 0 || g_aItem[i].pos.y >= SCREEN_HEIGHT || (bOberP1() && g_aItem[i].pos.x <= SCREEN_WIDTH * SPLIT_INA && g_aItem[i].pos.y <= SCREEN_HEIGHT * SPLIT_INA) || (bOberP2() && g_aItem[i].pos.x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && g_aItem[i].pos.y <= SCREEN_HEIGHT * SPLIT_INA) || (g_aItem[i].pos.x <= SCREEN_WIDTH * SPLIT_INA && g_aItem[i].pos.y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA) || (g_aItem[i].pos.x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && g_aItem[i].pos.y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA))
				{
					g_aItem[i].bDisp = false;
				}
			}
		}
	}
}

//----------------
//�A�C�e���̕`�揈��
//----------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == true&& g_aItem[i].bDisp == true)
		{//�A�C�e�����g�p����Ă���

			if (GetMode()!=MODE_VS||!g_aItem[i].bSenter)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureItem);

				//�A�C�e���̕`��
				pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLESTRIP,//�^�C�v
					i * VT_MAX,//�n�܂�̔ԍ�
					2//�|���S���̌�
				);
			}
		}
	}
}

//-------------------
//�A�C�e���ݒu
//-------------------
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool bSenter, TYPE Type, int nNumber)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == false)
		{//�A�C�e�����g�p����Ă��Ȃ�
			g_aItem[i].pos = pos;
			g_aItem[i].move = move;
			g_aItem[i].col = col;
			g_aItem[i].fRadius = fRadius;

			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

			pVtx[0].col = g_aItem[i].col;
			pVtx[1].col = g_aItem[i].col;
			pVtx[2].col = g_aItem[i].col;
			pVtx[3].col = g_aItem[i].col;

			g_pVtxBuffItem->Unlock();//�o�b�t�@�̃A�����b�N

			g_aItem[i].nLife = nLife;
			g_nItemLifeDef[i] = g_aItem[i].nLife;
			g_aItem[i].bSenter = bSenter;
			g_aItem[i].Type = Type;
			g_aItem[i].bDisp = true;
			g_aItem[i].bUse = true;
			g_aItem[i].nNumber = nNumber;

			break;
		}
	}
}