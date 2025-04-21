//---------------------------------------
//
//�e�\������[bullet.cpp]
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

#define BULLET_DAMAGE (10)//�ʏ�e�_���[�W
#define BULLET3_DAMAGE (12)//3WEY�e�_���[�W
#define BULLETALL_DAMAGE (2)//�S���ʒe�_���[�W
#define BULLETSEARCH_DAMAGE (1)//�U���e�_���[�W
#define BULLETBOM_DAMAGE (500)//����e�_���[�W
#define BULLETSP_DAMAGE (50)//�X�y�V�����C�_���[�W
#define BULLETSEARCH_INA_P (0.3f)//�v���C���[�U���e���x
#define BULLETSEARCH_INA_B (0.1f)//�{�X�U���e���x
#define BULLETSEARCH_INA_V (0.1f)//VS�U���e���x
#define SEARCH_TIME (20)//�{�X�U���e�U������
#define NOSEARCH_TIME (10)//�v���C���[�U���e�U������܂ł̎���
#define BULLET_EFFECT_LIFE (50)

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos[POS_MAX];//�ʒu
	float fLength;//�傫��
	D3DXVECTOR3 move;//�ړ���
	int nLife;//����
	BULLETTYPE Type;//�N�̋ʂ�
	int nType;//�ǂ�ȋʂ�
	bool bDisp;
	bool bUse[POS_MAX];//�g�p���Ă��邩�ǂ���
}Bullet;

//�O���[�o��
LPDIRECT3DTEXTURE9 g_apTextureBullet = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//�o�b�t�@�̃|�C���^
Bullet g_aBullet[MAX_BULLET];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_BULLET; i++)
	{//��񏉊���
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
			g_aBullet[i].bUse[i2] = false;//�g�p���Ă��Ȃ���Ԃɂ���
		}
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BULLET * POS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BULLET,
		&g_apTextureBullet
	);

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BULLET * POS_MAX; i++)
	{//�e�̐������J��Ԃ�
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x - BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y - BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x + BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y - BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x - BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y + BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[i / POS_MAX].pos[i % POS_MAX].x + BULLET_WIDTH / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), g_aBullet[i / POS_MAX].pos[i % POS_MAX].y + BULLET_HEIGHT / 2 * (g_aBullet[i / POS_MAX].fLength / 100.0f), 0.0f);

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
	g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�e�̏I������
//----------------
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureBullet != NULL)
	{
		g_apTextureBullet->Release();
		g_apTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_BULLET; i++)
	{//�e�̐������J��Ԃ�
		if (g_aBullet[i].bUse[SENTER] == true)
		{//�e���g�p����Ă���
			if (g_aBullet[i].Type == BULLETTYPE_PLAYER)
			{//�v���C���[�̒e
				Enemy* pEnemy;
				Boss* pBoss;
				Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;

				//���擾
				pEnemy = GetEnemy();
				pBoss = GetBoss();
				pPlayer = GetPlayer();
				pPlayer2 = GetPlayer2();
				pPlayer3 = GetPlayer3();
				pPlayer4 = GetPlayer4();
				float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{//�G�̐������J��Ԃ�
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						switch (pEnemy->nType)
						{//�G�̎�ނɂ���ĕς���
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
						{//�G�ɓ�������
							int nDamage;

							switch (g_aBullet[i].nType)
							{//�_���[�W�v�Z
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
				{//�G�̐������J��Ԃ�
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���

						if (g_aBullet[i].nType == 3)
						{//�U���e
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{//�T�[�`���łȂ�
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{//�T�[�`�J�n
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{//�T�[�`
								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);//���̕���
								Xlong[i] = pEnemy->pos.x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pEnemy->pos.y - g_aBullet[i].pos[SENTER].y;
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);//�G�̕���

								fRotDiff[i] = fRotDest[i] - fRotMove[i];//���̊p�x
								if (fRotDiff[i] > D3DX_PI)
								{//�C��
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{//�C��
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;//�p�x��␳

								if (fRotMove[i] > D3DX_PI)
								{//�C��
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{//�C��
									fRotMove[i] += D3DX_PI * 2;
								}

								//�ړ��ʂ̐ݒ�
								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{//�e�̐������J��Ԃ�
								if (g_aBullet[i2].bUse[SENTER] == false)
								{//�g�p���Ă��Ȃ��e�̃T�[�`��؂�
									bSa[i2] = false;
								}
							}
						}
						break;
					}
				}

				for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
				{//�{�X�̐������J��Ԃ�
					if (pBoss->bUse == true)
					{//�{�X���g�p����Ă���
						switch (pBoss->nType)
						{//��ނɂ���Đݒ�
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
						{//�{�X�ɓ�������
							int nDamage;

							switch (g_aBullet[i].nType)
							{//�_���[�W�v�Z
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
				{//�{�X�̐������J��Ԃ�
					if (pBoss->bUse == true)
					{//�{�X���g�p����Ă���
						if (g_aBullet[i].nType == 3)
						{//�U���e
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f }, fRotMove[MAX_BULLET] = { 0.0f }, fRotDest[MAX_BULLET] = { 0.0f }, fRotDiff[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{//�T�[�`���Ă��Ȃ�
								SCount[i]++;
								if (SCount[i] >= NOSEARCH_TIME)
								{//�T�[�`�J�n
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{//�T�[�`
								fRotMove[i] = atan2f(g_aBullet[i].move.x, g_aBullet[i].move.y);//���̊p�x
								Xlong[i] = pBoss->pos.x - g_aBullet[i].pos[SENTER].x;
								Ylong[i] = pBoss->pos.y - g_aBullet[i].pos[SENTER].y;
								fRotDest[i] = atan2f(Xlong[i], Ylong[i]);//�G�̊p�x

								fRotDiff[i] = fRotDest[i] - fRotMove[i];//�p�x�̍�
								if (fRotDiff[i] > D3DX_PI)
								{//�C��
									fRotDiff[i] -= D3DX_PI * 2;
								}
								else if (fRotDiff[i] < -D3DX_PI)
								{//�C��
									fRotDiff[i] += D3DX_PI * 2;
								}

								fRotMove[i] += fRotDiff[i] * BULLETSEARCH_INA_P;//�p�x��␳

								if (fRotMove[i] > D3DX_PI)
								{//�C��
									fRotMove[i] -= D3DX_PI * 2;
								}
								else if (fRotMove[i] < -D3DX_PI)
								{//�C��
									fRotMove[i] += D3DX_PI * 2;
								}

								//�ړ��ʂ̐ݒ�
								g_aBullet[i].move.x = sinf(fRotMove[i]) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(fRotMove[i]) * BULLET_SPEED;
							}

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{//�e�̐������J��Ԃ�
								if (g_aBullet[i2].bUse[SENTER] == false)
								{//�g�p���Ă��Ȃ��e�̃T�[�`��؂�
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
			{//�v���C���[�̒e
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
					{//�G���g�p����Ă���
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
					{//�G���g�p����Ă���
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
					{//�G���g�p����Ă���
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
					{//�G���g�p����Ă���
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
			{//�v���C���[�̒e
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
				{//�G���g�p����Ă���
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
				{//�G���g�p����Ă���
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
				{//�G���g�p����Ă���
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
				{//�G���g�p����Ă���
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
			{//�v���C���[�̒e
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
				{//�G���g�p����Ă���
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
				{//�G���g�p����Ă���
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
				{//�G���g�p����Ă���
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
				{//�G���g�p����Ă���
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
			{//�G�̒e
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
			{//�G�̒e
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

			//���W�ݒ�
			pVtx += i * VT_MAX * POS_MAX;//�|���S��1���i�߂�
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

			g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N

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
//�e�̕`�揈��
//----------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BULLET * POS_MAX; i++)
	{
		if (g_aBullet[i / POS_MAX].bUse[i % POS_MAX] == true)
		{//�e���g�p����Ă���
			if (g_aBullet[i / POS_MAX].bDisp || i % POS_MAX != SENTER)
			{
				if (GetMode() != MODE_VS || i % POS_MAX != SENTER)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_apTextureBullet);
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
	}
}

//-------------------
//���ʏ�e
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
			{//�e���g�p����Ă��Ȃ�
				g_aBullet[i].pos[SENTER] = pos;
				g_aBullet[i].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//���_���W�ݒ�
				pVtx += i * VT_MAX * POS_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N

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
//3wey�e
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
			{//�e���g�p����Ă��Ȃ�
				g_aBullet[i].pos[SENTER] = pos;
				g_aBullet[i + 1].pos[SENTER] = pos;
				g_aBullet[i + 2].pos[SENTER] = pos;
				g_aBullet[i].fLength = fLengthPlayer;
				g_aBullet[i + 1].fLength = fLengthPlayer;
				g_aBullet[i + 2].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//���_���W�ݒ�
				pVtx += i * VT_MAX * POS_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX * POS_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX * POS_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N

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
//�S���ʒe
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
				pVtx += i * VT_MAX * POS_MAX;//�|���S��1���i�߂�
				for (int i2 = 0; i2 < ALL_BULLET; i2++)
				{
					if (g_aBullet[i + i2].bUse[SENTER] == false)
					{//�e���g�p����Ă��Ȃ�

						g_aBullet[i + i2].pos[SENTER] = pos;
						g_aBullet[i + i2].fLength = fLengthPlayer;


						//���_���W�ݒ�
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX * POS_MAX;//�|���S��1���i�߂�

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 2;
						g_aBullet[i + i2].bUse[SENTER] = true;
						fAll += 2.0f / (float)ALL_BULLET;
					}
				}
				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N
				break;
			}
		}
	}
}

//-----------------
//�U���e
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
				pVtx += i * VT_MAX * POS_MAX;//�|���S��1���i�߂�
				for (int i2 = 0; i2 < ALL_BULLETS; i2++)
				{
					if (g_aBullet[i + i2].bUse[SENTER] == false)
					{//�e���g�p����Ă��Ȃ�

						g_aBullet[i + i2].pos[SENTER] = pos;
						g_aBullet[i + i2].fLength = fLengthPlayer;


						//���_���W�ݒ�
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX * POS_MAX;//�|���S��1���i�߂�

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 3;
						g_aBullet[i + i2].bUse[SENTER] = true;
						fAll += 2.0f / (float)ALL_BULLETS;
					}
				}
				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N
				break;
			}
		}
	}
}

//------------
//����e
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
			{//�e���g�p����Ă��Ȃ�
				g_aBullet[i].pos[SENTER] = pos;
				g_aBullet[i].fLength = fLengthPlayer;
				g_aBullet[i].fLength *= BOM_SIZE_INA;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//���_���W�ݒ�
				pVtx += i * VT_MAX * POS_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N

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
//����e
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
				pVtx += i * VT_MAX * POS_MAX;//�|���S��1���i�߂�
				for (int i2 = 0; i2 < ALL_BULLETSP; i2++)
				{
					if (g_aBullet[i + i2].bUse[SENTER] == false)
					{//�e���g�p����Ă��Ȃ�

						g_aBullet[i + i2].pos[SENTER] = pos;
						g_aBullet[i + i2].pos[SENTER] += D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP * i2, cosf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP * i2, 0.0f);
						g_aBullet[i + i2].fLength = fLengthPlayer;
						g_aBullet[i + i2].fLength *= SP_SIZE_INA;

						//���_���W�ݒ�
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos[SENTER].x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos[SENTER].y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX * POS_MAX;//�|���S��1���i�߂�

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP, cosf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED_SP, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 5;
						g_aBullet[i + i2].bUse[SENTER] = true;
					}
				}
				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N
				break;
			}
		}
	}
}