//----------------------------------------
//
//�{�X�\������[boss.cpp]
//Author fuma sato
//
//----------------------------------------
#include"boss.h"
#include"bullet.h"
#include"explosion.h"
#include"score.h"
#include"exef.h"
#include"input.h"

//�}�N����`
#define BOSS_BULLET_INTER (10)//�o���b�g�Ԋu
#define BOSS_BULLET_SPEED (10.0f)//�o���b�g�X�s�[�h
#define BOSS_BULLET_SIZE (100.0f)//�o���b�g�T�C�Y
#define BOSS_TERN_COUNT (60)//�����]���Ԋu
#define BOSS_SPEED (0.3f)//�{�X�̃X�s�[�h
#define BOSS_SPEED2 (0.6f)//�{�X�̃X�s�[�h
#define BOSS_SPEED3 (0.1f)//�{�X�̃X�s�[�h
#define BOSS_SPEED4 (0.0f)//�{�X�̃X�s�[�h
#define BOSS_LIFE (2000)
#define BOSS_LIFE2 (3000)
#define BOSS_LIFE3 (2500)
#define BOSS_LIFE4 (5000)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureBoss[NUM_BOSS] = {};//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;//�o�b�t�@�̃|�C���^
Boss g_aBoss[MAX_BOSS];//�{�X���
int g_nNumBoss = 0;//�{�X�̐�

//-----------------------
//�{�X�̏���������
//-----------------------
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BOSS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS,
		&g_apTextureBoss[0]
	);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS2,
		&g_apTextureBoss[1]
	);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS3,
		&g_apTextureBoss[2]
	);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BOSS4,
		&g_apTextureBoss[3]
	);

	for (int i = 0; i < MAX_BOSS; i++)
	{//��񏉊���
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

	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < MAX_BOSS; i++)
	{
		//���_���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH / 2, g_aBoss[i].pos.y - BOSS_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH / 2, g_aBoss[i].pos.y - BOSS_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BOSS_WIDTH / 2, g_aBoss[i].pos.y + BOSS_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BOSS_WIDTH / 2, g_aBoss[i].pos.y + BOSS_HEIGHT / 2, 0.0f);

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

	g_pVtxBuffBoss->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//---------------------
//�{�X�̏I������
//---------------------
void UninitBoss(void)
{
	for (int i = 0; i < NUM_BOSS; i++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBoss[i] != NULL)
		{
			g_apTextureBoss[i]->Release();
			g_apTextureBoss[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//-----------------------
//�{�X�̍X�V����
//-----------------------
void UpdateBoss(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	static int aBossBulletCnt[MAX_BOSS] = { 0 };
	static int nCount[MAX_BOSS] = { 0 };

	for (int i = 0; i < MAX_BOSS; i++)
	{//�{�X�̐������J��Ԃ�
		if (g_aBoss[i].bUse == true)
		{//�{�X���g�p����Ă���
			switch (g_aBoss[i].state)
			{//���
			case BOSSSTATE_NORMAL://�ʏ�
				break;
			case BOSSSTATE_DAMAGE://�_���[�W
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
					g_pVtxBuffBoss->Unlock();//�o�b�t�@�̃A�����b�N
				}
			}

			aBossBulletCnt[i]+=ControllerNum(CONTYPE_D);
			if (ControllerNum(CONTYPE_D)==0)
			{
				aBossBulletCnt[i] ++;
			}

			if (aBossBulletCnt[i] >= BOSS_BULLET_INTER)
			{//�o���b�g�Ԋu
				aBossBulletCnt[i] = 0;
				//�e�̐ݒ�
				int nRand = 0;
				nRand = rand() % 4;
				switch (nRand)
				{//�����_��
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

						g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N
						pVtx += i * VT_MAX;

						//���_���W�ݒ�
						pVtx[0].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BossWidth / 2, g_aBoss[i].pos.y - BossHeight / 2, 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BossWidth / 2, g_aBoss[i].pos.y - BossHeight / 2, 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBoss[i].pos.x - BossWidth / 2, g_aBoss[i].pos.y + BossHeight / 2, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBoss[i].pos.x + BossWidth / 2, g_aBoss[i].pos.y + BossHeight / 2, 0.0f);

						g_pVtxBuffBoss->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

						g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N
						pVtx += i2 * VT_MAX;

						//���_���W�ݒ�
						pVtx[0].pos = D3DXVECTOR3(g_aBoss[i2].pos.x - BossWidth2 / 2, g_aBoss[i2].pos.y - BossHeight2 / 2, 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBoss[i2].pos.x + BossWidth2 / 2, g_aBoss[i2].pos.y - BossHeight2 / 2, 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBoss[i2].pos.x - BossWidth2 / 2, g_aBoss[i2].pos.y + BossHeight2 / 2, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBoss[i2].pos.x + BossWidth2 / 2, g_aBoss[i2].pos.y + BossHeight2 / 2, 0.0f);

						g_pVtxBuffBoss->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
					}
				}
			}

			nCount[i]++;
			if (nCount[i]% BOSS_TERN_COUNT ==0)
			{//�����]���Ԋu
				//�����_���ɓ���
				int nRand = rand() % 201;
				g_aBoss[i].move.x = sinf(D3DX_PI * (((float)nRand / 100.0f) - 1.0f)) * g_aBoss[i].fSpeed;
				g_aBoss[i].move.y = cosf(D3DX_PI * (((float)nRand / 100.0f) - 1.0f)) * g_aBoss[i].fSpeed;
			}
		}
	}
}

//--------------------
//�{�X�̕`�揈��
//--------------------
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BOSS; i++)
	{//�{�X�̐������J��Ԃ�
		if (g_aBoss[i].bUse == true)
		{//�{�X���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBoss[g_aBoss[i].nType]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX * i,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//-------------------
//�{�X�ݒ�
//-------------------
void SetBoss(D3DXVECTOR3 pos, int nType)
{
	for (int i = 0; i < MAX_BOSS; i++)
	{//�{�X�̐������J��Ԃ�
		if (g_aBoss[i].bUse == false)
		{//�{�X���g�p����Ă��Ȃ�
			//������
			g_aBoss[i].pos = pos;
			g_aBoss[i].nType = nType;
			g_aBoss[i].state = BOSSSTATE_NORMAL;
			g_aBoss[i].nCounterState = 0;
			g_aBoss[i].bUse = true;

			//���_���W�ݒ�

			VERTEX_2D* pVtx;

			g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += i * VT_MAX;//�|���S��1���i�߂�

			switch (g_aBoss[i].nType)
			{//�^�C�v�ɂ���č��W��ݒ�
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

			g_pVtxBuffBoss->Unlock();//�o�b�t�@�̃A�����b�N
			g_nNumBoss++;//�{�X�̐����L�^
			break;
		}
	}
}

//-------------------
//�q�b�g����
//-------------------
void HitBoss(int nCntBoss, int nDamage)
{
	VERTEX_2D* pVtx;

	g_aBoss[nCntBoss].nLife -= nDamage;//�_���[�W

	if (g_aBoss[nCntBoss].nLife <= 0)
	{//�̗͂��s����
		//SetExplosion(g_aBoss[nCntBoss].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_aBoss->fLength);
		SetExef(g_aBoss[nCntBoss].pos, g_aBoss[nCntBoss].fLength,true);//�p�[�e�B�N��
		g_aBoss[nCntBoss].bUse = false;//�{�X�𖢎g�p
		g_nNumBoss--;//�{�X�̐������炷
		AddScore(nDamage * SCORE_DES_BOSS);//�X�R�A
	}
	else
	{
		g_aBoss[nCntBoss].state = BOSSSTATE_DAMAGE;//�_���[�W
		g_aBoss[nCntBoss].nCounterState = 5;//��ԃJ�E���^�[
		AddScore(nDamage * SCORE_HIT_BOSS);//�X�R�A

		g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nCntBoss * VT_MAX;
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_pVtxBuffBoss->Unlock();//�o�b�t�@�̃A�����b�N
	}
}

//---------------------
//�{�X���擾
//---------------------
Boss* GetBoss(void)
{
	return &g_aBoss[0];
}

//----------------------
//�{�X�̐��̎擾
//----------------------
int GetNumBoss(void)
{
	return g_nNumBoss;
}