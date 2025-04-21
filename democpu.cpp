//---------------------------------------
//
//�f���v���C�R���s���[�^�[�\������[democpu.cpp]
//Author fuma sato
//
//---------------------------------------

#include"democpu.h"
#include"back.h"
#include"bullet.h"
#include"explosion.h"
#include"exef.h"
#include"score.h"
#include"boss.h"
#include"enemy.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureDemoCpu;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDemoCpu = NULL;//�o�b�t�@�̃|�C���^
DEMOCPU g_DemoCpu;//�v���C���[���
D3DXVECTOR3 g_moveBulletCpu;//�e���ˌ����E�X�s�[�h�Ǘ�

//----------------------
//�v���C���[�̏���������
//----------------------
void InitDemoCpu(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDemoCpu,
		NULL
	);

	g_DemoCpu.nCounterAnim = 0;
	g_DemoCpu.nPatternAnim = 0;
	g_DemoCpu.pos = D3DXVECTOR3(DEMOCPU_WIDTH, SCREEN_HEIGHT / 2, 0);//�����ʒu
	g_DemoCpu.move = D3DXVECTOR3(DEMOCPU_SPEED_DEF, DEMOCPU_SPEED_DEF, DEMOCPU_SPEED_DEF);//�v���C���[�X�s�[�h
	g_DemoCpu.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
	g_DemoCpu.rotDef = g_DemoCpu.rot;//�����ۑ�
	g_DemoCpu.fLength = sqrtf(DEMOCPU_WIDTH * DEMOCPU_WIDTH + DEMOCPU_HEIGHT * DEMOCPU_HEIGHT) / 2.0f;//�Ίp���̒���
	g_DemoCpu.fLengthDef = g_DemoCpu.fLength;//�����ۑ�
	g_DemoCpu.fAngle = atan2f(DEMOCPU_WIDTH, DEMOCPU_HEIGHT);//�p�x
	g_DemoCpu.nLife = 100;
	g_DemoCpu.state = PLAYERSTATE_NORMAL;
	g_DemoCpu.nCounterState = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_PLAYER1,
		&g_pTextureDemoCpu
	);

	g_pVtxBuffDemoCpu->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	//���W�ݒ�
	pVtx[0].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[0].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[1].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[2].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[3].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_C, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_C);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_C, UV_DEF / V_MAX_C);

	g_pVtxBuffDemoCpu->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//-------------------
//�v���C���[�I������
//-------------------
void UninitDemoCpu(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureDemoCpu != NULL)
	{
		g_pTextureDemoCpu->Release();
		g_pTextureDemoCpu = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDemoCpu != NULL)
	{
		g_pVtxBuffDemoCpu->Release();
		g_pVtxBuffDemoCpu = NULL;
	}
}

//-------------------
//�v���C���[�X�V����
//-------------------
void UpdateDemoCpu(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	int nRand = 0;
	nRand = rand() % 100;

	//�ړ�����
	if (nRand >= 0 && nRand < 30)//A�L�[
	{
		if (nRand >= 0 && nRand < 10)//W�L�[
		{//����
			g_DemoCpu.move.x += sinf(D3DX_PI * -0.75f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * -0.75f) * DEMOCPU_SPEED_Y;
		}
		else if (nRand >= 10 && nRand < 20)//S�L�[
		{//����
			g_DemoCpu.move.x += sinf(D3DX_PI * -0.25f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * -0.25f) * DEMOCPU_SPEED_Y;
		}
		else
		{//��
			g_DemoCpu.move.x += -DEMOCPU_SPEED_X;
		}
	}
	else if (nRand >= 30 && nRand < 60)//D�L�[
	{
		if (nRand >= 30 && nRand < 40)//W�L�[
		{//�E��
			g_DemoCpu.move.x += sinf(D3DX_PI * 0.75f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * 0.75f) * DEMOCPU_SPEED_Y;
		}
		else if (nRand >= 40 && nRand < 50)//S�L�[
		{//�E��
			g_DemoCpu.move.x += sinf(D3DX_PI * 0.25f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * 0.25f) * DEMOCPU_SPEED_Y;
		}
		else
		{//�E
			g_DemoCpu.move.x += DEMOCPU_SPEED_X;
		}
	}
	else if (nRand >= 60 && nRand < 70)//W�L�[
	{//��
		g_DemoCpu.move.y += -DEMOCPU_SPEED_Y;
	}
	else if (nRand >= 70 && nRand < 80)//S�L�[
	{//��
		g_DemoCpu.move.y += DEMOCPU_SPEED_Y;
	}

	if (nRand >= 80 && nRand < 82)
	{
		g_DemoCpu.rot.z += DEMOCPU_ROT_SPEED;
	}
	if (nRand >= 82 && nRand < 84)
	{
		g_DemoCpu.rot.z += -DEMOCPU_ROT_SPEED;
	}
	if (nRand >= 84 && nRand < 86)
	{
		g_DemoCpu.rot = g_DemoCpu.rotDef;
	}

	if (nRand >= 86 && nRand < 88)
	{
		if (g_DemoCpu.fLength >= 0.0f && g_DemoCpu.fLength <= DEMOCPU_SIZE_MAX)
		{
			g_DemoCpu.fLength += DEMOCPU_SIZE_SPEED;
		}
	}
	if (nRand >= 88 && nRand < 90)
	{
		if (g_DemoCpu.fLength >= DEMOCPU_SIZE_MIN)
		{
			g_DemoCpu.fLength += -DEMOCPU_SIZE_SPEED;
		}
	}
	if (nRand >= 90 && nRand < 92)
	{
		g_DemoCpu.fLength = g_DemoCpu.fLengthDef;
	}

	if (nRand >= 92 && nRand < 94)
	{
		g_moveBulletCpu.x = sinf(g_DemoCpu.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
		g_moveBulletCpu.y = cosf(g_DemoCpu.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

		//�e�̐ݒ�
		SetBullet(g_DemoCpu.pos, g_moveBulletCpu, g_DemoCpu.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
	}
	if (nRand >= 94 && nRand < 96)
	{
		//�e�̐ݒ�
		Set3Bullet(g_DemoCpu.pos, g_DemoCpu.rot.z, g_DemoCpu.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
	}
	if (nRand >= 96 && nRand < 98)
	{
		//�e�̐ݒ�
		SetAllBullet(g_DemoCpu.pos, g_DemoCpu.rot.z, g_DemoCpu.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
	}

	if (nRand >= 98 && nRand < 100)
	{
		SetSearchBullet(g_DemoCpu.pos, g_DemoCpu.rot.z, g_DemoCpu.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
	}

	if (g_DemoCpu.state != PLAYERSTATE_APPEAR && g_DemoCpu.state != PLAYERSTATE_DIE)
	{
		Enemy* pEnemy;
		Boss* pBoss;
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;

		pEnemy = GetEnemy();
		pBoss = GetBoss();
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
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
				if (pEnemy->pos.x - EnemyWidth / 2 <= g_DemoCpu.pos.x + g_DemoCpu.fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_DemoCpu.pos.x - g_DemoCpu.fLength && pEnemy->pos.y - EnemyHeight / 2 <= g_DemoCpu.pos.y + g_DemoCpu.fLength && pEnemy->pos.y + EnemyHeight / 2 >= g_DemoCpu.pos.y - g_DemoCpu.fLength)
				{
					if (g_DemoCpu.pos.x <= pEnemy->pos.x - EnemyWidth / 2)
					{
						g_DemoCpu.pos.x = (pEnemy->pos.x - EnemyWidth / 2) - g_DemoCpu.fLength;
					}
					else if (g_DemoCpu.pos.x >= pEnemy->pos.x + EnemyWidth / 2)
					{
						g_DemoCpu.pos.x = (pEnemy->pos.x + EnemyWidth / 2) + g_DemoCpu.fLength;
					}
					else if (g_DemoCpu.pos.y <= pEnemy->pos.y - EnemyHeight / 2)
					{
						g_DemoCpu.pos.y = (pEnemy->pos.y - EnemyHeight / 2) - g_DemoCpu.fLength;
					}
					else if (g_DemoCpu.pos.y >= pEnemy->pos.y + EnemyHeight / 2)
					{
						g_DemoCpu.pos.y = (pEnemy->pos.y + EnemyHeight / 2) + g_DemoCpu.fLength;
					}
				}
			}
		}

		for (int i = 0; i < MAX_BOSS; i++, pBoss++)
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
				if (pEnemy->pos.x - EnemyWidth / 2 <= g_DemoCpu.pos.x + g_DemoCpu.fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_DemoCpu.pos.x - g_DemoCpu.fLength && pEnemy->pos.y - EnemyHeight / 2 <= g_DemoCpu.pos.y + g_DemoCpu.fLength && pEnemy->pos.y + EnemyHeight / 2 >= g_DemoCpu.pos.y - g_DemoCpu.fLength)
				{
					if (g_DemoCpu.pos.x <= pEnemy->pos.x - EnemyWidth / 2)
					{
						g_DemoCpu.pos.x = (pEnemy->pos.x - EnemyWidth / 2) - g_DemoCpu.fLength;
					}
					else if (g_DemoCpu.pos.x >= pEnemy->pos.x + EnemyWidth / 2)
					{
						g_DemoCpu.pos.x = (pEnemy->pos.x + EnemyWidth / 2) + g_DemoCpu.fLength;
					}
					else if (g_DemoCpu.pos.y <= pEnemy->pos.y - EnemyHeight / 2)
					{
						g_DemoCpu.pos.y = (pEnemy->pos.y - EnemyHeight / 2) - g_DemoCpu.fLength;
					}
					else if (g_DemoCpu.pos.y >= pEnemy->pos.y + EnemyHeight / 2)
					{
						g_DemoCpu.pos.y = (pEnemy->pos.y + EnemyHeight / 2) + g_DemoCpu.fLength;
					}
				}
			}
		}
	}

	//�ʒu�̍X�V
	g_DemoCpu.pos.x += g_DemoCpu.move.x;
	g_DemoCpu.pos.y += g_DemoCpu.move.y;

	//�ړ��ʂ̍X�V(����)
	g_DemoCpu.move.x += (DEMOCPU_SPEED_DEF - g_DemoCpu.move.x) * DEMOCPU_INA;
	g_DemoCpu.move.y += (DEMOCPU_SPEED_DEF - g_DemoCpu.move.y) * DEMOCPU_INA;

	g_DemoCpu.nCounterAnim++;
	if ((g_DemoCpu.nCounterAnim % UP_TIME) == 0)//�X�V����
	{
		g_DemoCpu.nCounterAnim = (g_DemoCpu.nPatternAnim + 1) % (U_MAX_C * V_MAX_C);

		g_pVtxBuffDemoCpu->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

	//���W�ݒ�
		pVtx[0].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[0].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[1].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[2].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[3].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[3].pos.z = 0.0f;

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim + UV_DEF / U_MAX_C, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C) + UV_DEF / V_MAX_C);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim + UV_DEF / U_MAX_C, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C) + UV_DEF / V_MAX_C);

		if (pVtx[0].pos.x >= SCREEN_WIDTH || pVtx[1].pos.x >= SCREEN_WIDTH || pVtx[2].pos.x >= SCREEN_WIDTH || pVtx[3].pos.x >= SCREEN_WIDTH)//��ʉE�[
		{
			g_DemoCpu.pos.x = g_DemoCpu.fLength;//��ʍ��[
		}
		else if (pVtx[0].pos.x <= 0 || pVtx[1].pos.x <= 0 || pVtx[2].pos.x <= 0 || pVtx[3].pos.x <= 0)//��ʍ��[
		{
			g_DemoCpu.pos.x = SCREEN_WIDTH - g_DemoCpu.fLength;//��ʉE�[
		}

		if (pVtx[0].pos.y >= SCREEN_HEIGHT || pVtx[1].pos.y >= SCREEN_HEIGHT || pVtx[2].pos.y >= SCREEN_HEIGHT || pVtx[3].pos.y >= SCREEN_HEIGHT)//��ʉ��[
		{
			g_DemoCpu.pos.y = g_DemoCpu.fLength;//��ʏ�[
		}
		else if (pVtx[0].pos.y <= 0 || pVtx[1].pos.y <= 0 || pVtx[2].pos.y <= 0 || pVtx[3].pos.y <= 0)
		{
			g_DemoCpu.pos.y = SCREEN_HEIGHT - g_DemoCpu.fLength;//��ʉ��[
		}

		switch (g_DemoCpu.state)
		{
		case PLAYERSTATE_DAMAGE:
			g_DemoCpu.nCounterState--;
			if (g_DemoCpu.nCounterState <= 0)
			{
				g_DemoCpu.state = PLAYERSTATE_NORMAL;

				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		g_pVtxBuffDemoCpu->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
}

//-------------------
//�v���C���[�`�揈��
//-------------------
void DrawDemoCpu(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffDemoCpu, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureDemoCpu);

	//�v���C���[�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,//�n�܂�̔ԍ�
		2//�|���S���̌�
	);
}

//-------------------
//�q�b�g����
//-------------------
void HitDemoCpu(int nDamage)
{
	VERTEX_2D* pVtx;

	g_DemoCpu.nLife -= nDamage;

	if (g_DemoCpu.nLife <= 0)
	{
		SetExef(g_DemoCpu.pos, g_DemoCpu.fLength, true);
		g_DemoCpu.state = PLAYERSTATE_DIE;
		AddScore(-nDamage * SCORE_DIE);
	}
	else
	{
		g_DemoCpu.state = PLAYERSTATE_DAMAGE;
		g_DemoCpu.nCounterState = 5;
		AddScore(-nDamage * SCORE_MISS);

		g_pVtxBuffDemoCpu->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_pVtxBuffDemoCpu->Unlock();//�o�b�t�@�̃A�����b�N
	}
}

//-------------------------
//�v���C���[�o�b�t�@�̎擾
//-------------------------
PDIRECT3DVERTEXBUFFER9 GetDemoCpuBuffer(void)
{
	return g_pVtxBuffDemoCpu;
}

//-------------------------
//�v���C���[�擾
//-------------------------
DEMOCPU* GetDemoCpu(void)
{
	return &g_DemoCpu;
}