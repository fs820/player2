//---------------------------------------
//
//�G�t�F�N�g�\������[effect.cpp]
//Author fuma sato
//
//---------------------------------------

#include"effect.h"
#include"bullet.h"
#include"player.h"
#include"player2.h"
#include"player3.h"
#include"player4.h"
#include"boss.h"
#include"enemy.h"
#include"vs.h"

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	int nLife;//����
	bool bSenter;
	bool bUse;//�g�p���Ă��邩�ǂ���
}Effect;

//�O���[�o��
LPDIRECT3DTEXTURE9 g_apTextureEffect = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//�o�b�t�@�̃|�C���^
Effect g_aEffect[MAX_EFFECT];//�e�̏��
int g_nLifeDef[MAX_EFFECT] = { 0 };

//-----------------
//�e�̏���������
//-----------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		g_aEffect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[i].fRadius = 0.0f;
		g_aEffect[i].nLife = 0;
		g_aEffect[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT,
		&g_apTextureEffect
	);

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);

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
	g_pVtxBuffEffect->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�e�̏I������
//----------------
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureEffect != NULL)
	{
		g_apTextureEffect->Release();
		g_apTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateEffect(void)
{
	VERTEX_2D* pVtx;
	static D3DXVECTOR3 Oldpos[MAX_EFFECT];

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == true)
		{//�e���g�p����Ă���

			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);

			BYTE alpha = (BYTE)(((float)g_aEffect[i].nLife) / ((float)g_nLifeDef[i]) * 255.0f);
			pVtx[0].col = (pVtx[0].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[1].col = (pVtx[1].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[2].col = (pVtx[2].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[3].col = (pVtx[3].col & 0x00FFFFFF) | (alpha << 24);

			g_pVtxBuffEffect->Unlock();//�o�b�t�@�̃A�����b�N

			Oldpos[i] = g_aEffect[i].pos;

			g_aEffect[i].pos.x += g_aEffect[i].move.x;
			g_aEffect[i].pos.y += g_aEffect[i].move.y;

			g_aEffect[i].nLife--;
			g_aEffect[i].fRadius *= ((float)g_aEffect[i].nLife) / ((float)g_nLifeDef[i]);

			if (g_aEffect[i].nLife <= 0)
			{
				g_aEffect[i].bUse = false;
			}

			if (!g_aEffect[i].bSenter)
			{
				Boss* pBoss;
				pBoss = GetBoss();
				if (GetMode()==MODE_VS)
				{
					if (Oldpos[i].x <= SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y <= SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aEffect[i].pos.x >= SCREEN_WIDTH * SPLIT_INA_VS || g_aEffect[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA_VS || g_aEffect[i].pos.x <= 0 || g_aEffect[i].pos.y <= 0)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer = GetPlayer();

						Xmove = pPlayer->pos[SENTER].x - pPlayer->Oldpos.x;
						Ymove = pPlayer->pos[SENTER].y - pPlayer->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
					else if (Oldpos[i].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y <= SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aEffect[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS || g_aEffect[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA_VS || g_aEffect[i].pos.x >= SCREEN_WIDTH || g_aEffect[i].pos.y <= 0)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer2;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer2 = GetPlayer2();

						Xmove = pPlayer2->pos[SENTER].x - pPlayer2->Oldpos.x;
						Ymove = pPlayer2->pos[SENTER].y - pPlayer2->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
					else if (Oldpos[i].x <= SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aEffect[i].pos.x >= SCREEN_WIDTH * SPLIT_INA_VS || g_aEffect[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS || g_aEffect[i].pos.x <= 0 || g_aEffect[i].pos.y >= SCREEN_HEIGHT)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer3;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer3 = GetPlayer3();

						Xmove = pPlayer3->pos[SENTER].x - pPlayer3->Oldpos.x;
						Ymove = pPlayer3->pos[SENTER].y - pPlayer3->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
					else if (Oldpos[i].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS && Oldpos[i].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS)
					{
						if (g_aEffect[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA_VS || g_aEffect[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS || g_aEffect[i].pos.x >= SCREEN_WIDTH || g_aEffect[i].pos.y >= SCREEN_HEIGHT)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer4;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer4 = GetPlayer4();

						Xmove = pPlayer4->pos[SENTER].x - pPlayer4->Oldpos.x;
						Ymove = pPlayer4->pos[SENTER].y - pPlayer4->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA_VS;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA_VS;
					}
				}
				else
				{
					g_aEffect[i].pos.x -= pBoss->move.x * PLAYER_FIELD_INA * SPLIT_INA;
					g_aEffect[i].pos.y -= pBoss->move.y * PLAYER_FIELD_INA * SPLIT_INA;

					if (Oldpos[i].x <= SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y <= SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aEffect[i].pos.x >= SCREEN_WIDTH * SPLIT_INA || g_aEffect[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA || g_aEffect[i].pos.x <= 0 || g_aEffect[i].pos.y <= 0)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer = GetPlayer();

						Xmove = pPlayer->pos[SENTER].x - pPlayer->Oldpos.x;
						Ymove = pPlayer->pos[SENTER].y - pPlayer->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA;
					}
					else if (Oldpos[i].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y <= SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aEffect[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA || g_aEffect[i].pos.y >= SCREEN_HEIGHT * SPLIT_INA || g_aEffect[i].pos.x >= SCREEN_WIDTH || g_aEffect[i].pos.y <= 0)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer2;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer2 = GetPlayer2();

						Xmove = pPlayer2->pos[SENTER].x - pPlayer2->Oldpos.x;
						Ymove = pPlayer2->pos[SENTER].y - pPlayer2->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA;
					}
					else if (Oldpos[i].x <= SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aEffect[i].pos.x >= SCREEN_WIDTH * SPLIT_INA || g_aEffect[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA || g_aEffect[i].pos.x <= 0 || g_aEffect[i].pos.y >= SCREEN_HEIGHT)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer3;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer3 = GetPlayer3();

						Xmove = pPlayer3->pos[SENTER].x - pPlayer3->Oldpos.x;
						Ymove = pPlayer3->pos[SENTER].y - pPlayer3->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA;
					}
					else if (Oldpos[i].x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && Oldpos[i].y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA)
					{
						if (g_aEffect[i].pos.x <= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA || g_aEffect[i].pos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA || g_aEffect[i].pos.x >= SCREEN_WIDTH || g_aEffect[i].pos.y >= SCREEN_HEIGHT)
						{
							g_aEffect[i].bUse = false;
						}

						Player* pPlayer4;
						float Xmove = 0.0f, Ymove = 0.0f;

						pPlayer4 = GetPlayer4();

						Xmove = pPlayer4->pos[SENTER].x - pPlayer4->Oldpos.x;
						Ymove = pPlayer4->pos[SENTER].y - pPlayer4->Oldpos.y;

						g_aEffect[i].pos.x -= Xmove * SPLIT_INA;
						g_aEffect[i].pos.y -= Ymove * SPLIT_INA;
					}

				}
			}
			else
			{
				Boss* pBoss;
				pBoss = GetBoss();
				g_aEffect[i].pos.x -= pBoss->move.x * PLAYER_FIELD_INA;
				g_aEffect[i].pos.y -= pBoss->move.y * PLAYER_FIELD_INA;

				if (g_aEffect[i].pos.x <= 0 || g_aEffect[i].pos.x >= SCREEN_WIDTH || g_aEffect[i].pos.y <= 0 || g_aEffect[i].pos.y >= SCREEN_HEIGHT || (bOberP1()&&g_aEffect[i].pos.x <= SCREEN_WIDTH * SPLIT_INA && g_aEffect[i].pos.y <= SCREEN_HEIGHT * SPLIT_INA) || (bOberP2()&&g_aEffect[i].pos.x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && g_aEffect[i].pos.y <= SCREEN_HEIGHT * SPLIT_INA) || (g_aEffect[i].pos.x <= SCREEN_WIDTH * SPLIT_INA && g_aEffect[i].pos.y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA) || (g_aEffect[i].pos.x >= SCREEN_WIDTH - SCREEN_WIDTH * SPLIT_INA && g_aEffect[i].pos.y >= SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA))
				{
					g_aEffect[i].bUse = false;
				}
			}
		}
	}
}

//----------------
//�e�̕`�揈��
//----------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DRS_DESTBLENDALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == true)
		{//�e���g�p����Ă���
			if (GetMode()!=MODE_VS||!g_aEffect[i].bSenter)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEffect);

				//�e�̕`��
				pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLESTRIP,//�^�C�v
					i * VT_MAX,//�n�܂�̔ԍ�
					2//�|���S���̌�
				);
			}
		}
	}

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DRS_DESTBLENDALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//�J�����O
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�����x
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-------------------
//���ʏ�e
//-------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool bSenter)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aEffect[i].pos = pos;
			g_aEffect[i].move = move;
			g_aEffect[i].col = col;
			g_aEffect[i].fRadius = fRadius;

			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y - g_aEffect[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x - g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius, g_aEffect[i].pos.y + g_aEffect[i].fRadius, 0.0f);

			pVtx[0].col = g_aEffect[i].col;
			pVtx[1].col = g_aEffect[i].col;
			pVtx[2].col = g_aEffect[i].col;
			pVtx[3].col = g_aEffect[i].col;

			g_pVtxBuffEffect->Unlock();//�o�b�t�@�̃A�����b�N

			g_aEffect[i].nLife = nLife;
			g_nLifeDef[i] = g_aEffect[i].nLife;
			g_aEffect[i].bSenter = bSenter;
			g_aEffect[i].bUse = true;

			break;
		}
	}
}