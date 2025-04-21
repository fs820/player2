//----------------------------------------
//
//�v���C���[�\������[player4.cpp]
//Author fuma sato
//
//----------------------------------------

#include"player2.h"
#include"player3.h"
#include"player4.h"
#include"enemy.h"
#include"boss.h"
#include"input.h"
#include"back.h"
#include"bullet.h"
#include"explosion.h"
#include"exef.h"
#include"score.h"
#include"p4back.h"
#include"vs.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer4;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer4 = NULL;//�o�b�t�@�̃|�C���^
Player g_Player4;//�v���C���[���
D3DXVECTOR3 g_moveBullet4;//�e���ˌ����E�X�s�[�h�Ǘ�
bool g_bOber4 = false;

//----------------------
//�v���C���[�̏���������
//----------------------
void InitPlayer4(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * POS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer4,
		NULL
	);

	g_Player4.nCounterAnim = 0;
	g_Player4.nPatternAnim = 0;
	g_Player4.pos[SENTER] = D3DXVECTOR3(SCREEN_WIDTH - PLAYER_WIDTH, SCREEN_HEIGHT - SCREEN_HEIGHT / 3, 0);//�����ʒu
	if (GetMode() == MODE_VS)
	{
		g_Player4.pos[LEFTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player4.pos[RIGHTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player4.pos[LEFTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player4.pos[RIGHTDOWN] = D3DXVECTOR3(SCREEN_WIDTH - ((SCREEN_WIDTH * SPLIT_INA_VS) * 0.5f), SCREEN_HEIGHT - ((SCREEN_HEIGHT * SPLIT_INA_VS) * 0.5f), 0);//�����ʒu
	}
	else
	{
		g_Player4.pos[LEFTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player4.pos[RIGHTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player4.pos[LEFTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player4.pos[RIGHTDOWN] = D3DXVECTOR3(SCREEN_WIDTH - ((SCREEN_WIDTH * SPLIT_INA) * 0.5f), SCREEN_HEIGHT - ((SCREEN_HEIGHT * SPLIT_INA) * 0.5f), 0);//�����ʒu
	}
	g_Player4.Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
	g_Player4.move = D3DXVECTOR3(PLAYER_SPEED_DEF, PLAYER_SPEED_DEF, PLAYER_SPEED_DEF);//�v���C���[�X�s�[�h
	g_Player4.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);//����
	g_Player4.rotDef = g_Player4.rot;//�����ۑ�
	g_Player4.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//�Ίp���̒���
	g_Player4.fLengthDef = g_Player4.fLength;//�����ۑ�
	g_Player4.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//�p�x
	g_Player4.BulletType = TYPE_NORMAL;
	g_Player4.BulletCount = 0;
	g_Player4.nRema = 3;
	g_Player4.nLife = 100;
	g_Player4.state = PLAYERSTATE_NORMAL;
	g_Player4.nCounterState = 0;
	for (int i = 0; i < POS_MAX; i++)
	{
		g_Player4.bDisp[i] = true;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_PLAYER4,
		&g_pTexturePlayer4
	);

	g_pVtxBuffPlayer4->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	//���W�ݒ�
	pVtx[0].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[0].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[1].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength;
	pVtx[2].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength;
	pVtx[3].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength;
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
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);

	for (int i = 1; i < POS_MAX; i++)
	{
		pVtx += VT_MAX;

		if (GetMode() == MODE_VS)
		{
			//���W�ݒ�
			pVtx[0].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
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
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);
		}
		else
		{
			//���W�ݒ�
			pVtx[0].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
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
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);
		}
	}

	g_pVtxBuffPlayer4->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//-------------------
//�v���C���[�I������
//-------------------
void UninitPlayer4(void)
{
	SetVibrate(0.0f, CONTROLLER_4);
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer4 != NULL)
	{
		g_pTexturePlayer4->Release();
		g_pTexturePlayer4 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer4 != NULL)
	{
		g_pVtxBuffPlayer4->Release();
		g_pVtxBuffPlayer4 = NULL;
	}
}

//-------------------
//�v���C���[�X�V����
//-------------------
void UpdatePlayer4(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	float* pStick;

	//�ߋ��̈ʒu�̕ۑ�
	g_Player4.Oldpos = g_Player4.pos[SENTER];

	//�ړ�����

	//X
	if (IsXInputControllerConnected(CONTROLLER_4))
	{
		//���X�e�B�b�N����
		pStick = GetJoyStick(STICK_LEFT, CONTROLLER_4);
		g_Player4.move.x += *pStick * PLAYER_SPEED_X;
		g_Player4.move.y += -*(pStick + 1) * PLAYER_SPEED_Y;

		//�E�X�e�B�b�N����
		pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_4);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player4.rot.z = atan2f(*(pStick + 1), *pStick);
		}
	}
	//d
	else if (IsDirectInputControllerConnected(CONTROLLER_4))
	{

		//���X�e�B�b�N����
		pStick = GetdJoyStick(STICK_LEFT, CONTROLLER_4);
		g_Player4.move.x += *pStick * PLAYER_SPEED_X;
		g_Player4.move.y += *(pStick + 1) * PLAYER_SPEED_Y;

		//�E�X�e�B�b�N����
		pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_4);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player4.rot.z = -atan2f(*(pStick + 1), *pStick);
		}
	}

	if (GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_4) == true)//A�L�[
	{
		if (GetJoykeyPress(JOYKEY_UP, CONTROLLER_4) == true)//W�L�[
		{//����
			g_Player4.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_4) == true)//S�L�[
		{//����
			g_Player4.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//��
			g_Player4.move.x += -PLAYER_SPEED_X;
		}
	}
	else if (GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_4) == true)//D�L�[
	{
		if (GetJoykeyPress(JOYKEY_UP, CONTROLLER_4) == true)//W�L�[
		{//�E��
			g_Player4.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_4) == true)//S�L�[
		{//�E��
			g_Player4.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//�E
			g_Player4.move.x += PLAYER_SPEED_X;
		}
	}
	else if (GetJoykeyPress(JOYKEY_UP, CONTROLLER_4) == true)//W�L�[
	{//��
		g_Player4.move.y += -PLAYER_SPEED_Y;
	}
	else if (GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_4) == true)//S�L�[
	{//��
		g_Player4.move.y += PLAYER_SPEED_Y;
	}

	if (GetJoykeyPress(JOYKEY_B, CONTROLLER_4) == true)
	{
		g_Player4.rot.z += PLAYER_ROT_SPEED;
	}

	if (GetJoykeyPress(JOYKEY_Y, CONTROLLER_4) == true)
	{
		g_Player4.rot.z += -PLAYER_ROT_SPEED;
	}

	if (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_4) == true)
	{
		g_Player4.rot = g_Player4.rotDef;
	}

	if (GetJoykeyPress(JOYKEY_A, CONTROLLER_4) == true)
	{
		if (g_Player4.fLength >= 0.0f && g_Player4.fLength <= PLAYER_SIZE_MAX)
		{
			g_Player4.fLength += PLAYER_SIZE_SPEED;
		}
	}

	if (GetJoykeyPress(JOYKEY_X, CONTROLLER_4) == true)
	{
		if (g_Player4.fLength >= PLAYER_SIZE_MIN)
		{
			g_Player4.fLength += -PLAYER_SIZE_SPEED;
		}
	}

	if (GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_4) == true)
	{
		g_Player4.fLength = g_Player4.fLengthDef;
	}

	if (g_Player4.state != PLAYERSTATE_APPEAR && g_Player4.state != PLAYERSTATE_DIE)
	{
		if (GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_4) == true)
		{
			switch (g_Player4.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);
				break;
			case TYPE_3WEY:
				//�e�̐ݒ�
				Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//�e�̐ݒ�
				SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//�e�̐ݒ�
				SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
		else if (GetJoykeyRepeat(JOYKEY_RB, CONTROLLER_4) == true)
		{
			switch (g_Player4.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);
				break;
			case TYPE_3WEY:
				//�e�̐ݒ�
				Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_ALL:
				//�e�̐ݒ�
				SetAllBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SEARCH:
				//�e�̐ݒ�
				SetSearchBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//�e�̐ݒ�
				SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//�e�̐ݒ�
				SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

				if (g_Player4.BulletCount <= 0)
				{
					g_Player4.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
	}

	if (IsDirectInputControllerConnected(CONTROLLER_4))
	{
		if (GetdJoyPov(POV_UP, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.y += -PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHTUP, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.x += PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_RIGHTDOWN, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.y += PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFTDOWN, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.x += -PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_LEFTUP, 0, CONTROLLER_4) == true)
		{
			g_Player4.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player4.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
	}

	if (!strcmp(ControllerName(CONTROLLER_4), ELE_CON))
	{
		if (GetdJoykeyPress(ELEKEY_B, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(ELEKEY_Y, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(ELEKEY_L3, CONTROLLER_4) == true)
		{
			g_Player4.rot = g_Player4.rotDef;
		}

		if (GetdJoykeyPress(ELEKEY_A, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= 0.0f && g_Player4.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player4.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(ELEKEY_X, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player4.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(ELEKEY_R3, CONTROLLER_4) == true)
		{
			g_Player4.fLength = g_Player4.fLengthDef;
		}

		if (g_Player4.state != PLAYERSTATE_APPEAR && g_Player4.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(ELEKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(ELEKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_4), PS_CON))
	{
		if (GetdJoykeyPress(PSKEY_CR, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(PSKEY_SQ, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(PSKEY_L3, CONTROLLER_4) == true)
		{
			g_Player4.rot = g_Player4.rotDef;
		}

		if (GetdJoykeyPress(PSKEY_CI, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= 0.0f && g_Player4.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player4.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(PSKEY_TRA, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player4.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(PSKEY_R3, CONTROLLER_4) == true)
		{
			g_Player4.fLength = g_Player4.fLengthDef;
		}

		if (g_Player4.state != PLAYERSTATE_APPEAR && g_Player4.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(PSKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(PSKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_4), NIN_CON))
	{
		if (GetdJoykeyPress(NINKEY_A, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(NINKEY_X, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(NINKEY_L3, CONTROLLER_4) == true)
		{
			g_Player4.rot = g_Player4.rotDef;
		}

		if (GetdJoykeyPress(NINKEY_B, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= 0.0f && g_Player4.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player4.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(NINKEY_Y, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player4.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(NINKEY_R3, CONTROLLER_4) == true)
		{
			g_Player4.fLength = g_Player4.fLengthDef;
		}

		if (g_Player4.state != PLAYERSTATE_APPEAR && g_Player4.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(NINKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(NINKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}
	else if (!IsXInputControllerConnected(CONTROLLER_4) && IsDirectInputControllerConnected(CONTROLLER_4))
	{
		if (GetdJoykeyPress(DKEY_B, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(DKEY_Y, CONTROLLER_4) == true)
		{
			g_Player4.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(DKEY_L3, CONTROLLER_4) == true)
		{
			g_Player4.rot = g_Player4.rotDef;
		}

		if (GetdJoykeyPress(DKEY_A, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= 0.0f && g_Player4.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player4.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(DKEY_X, CONTROLLER_4) == true)
		{
			if (g_Player4.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player4.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(DKEY_R3, CONTROLLER_4) == true)
		{
			g_Player4.fLength = g_Player4.fLengthDef;
		}

		if (g_Player4.state != PLAYERSTATE_APPEAR && g_Player4.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(DKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, 1);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(DKEY_RB, CONTROLLER_4) == true)
			{
				switch (g_Player4.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet4.x = sinf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet4.y = cosf(g_Player4.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player4.pos[SENTER], g_moveBullet4, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player4.pos[SENTER], g_Player4.rot.z, g_Player4.fLength, BULLET_LIFE, BULLETTYPE_PLAYER4, BULLET_INTER);

					if (g_Player4.BulletCount <= 0)
					{
						g_Player4.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}

	if (g_Player4.state != PLAYERSTATE_APPEAR && g_Player4.state != PLAYERSTATE_DIE)
	{
		Enemy* pEnemy;
		Boss* pBoss;
		Player* pPlayer, * pPlayer2, * pPlayer3;
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;

		pEnemy = GetEnemy();
		pBoss = GetBoss();
		pPlayer = GetPlayer();
		pPlayer2 = GetPlayer2();
		pPlayer3 = GetPlayer3();
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
				if (pEnemy->pos.x - EnemyWidth / 2 <= g_Player4.pos[SENTER].x + g_Player4.fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_Player4.pos[SENTER].x - g_Player4.fLength && pEnemy->pos.y - EnemyHeight / 2 <= g_Player4.pos[SENTER].y + g_Player4.fLength && pEnemy->pos.y + EnemyHeight / 2 >= g_Player4.pos[SENTER].y - g_Player4.fLength)
				{
					if (g_Player4.pos[SENTER].x <= pEnemy->pos.x - EnemyWidth / 2)
					{
						g_Player4.pos[SENTER].x = (pEnemy->pos.x - EnemyWidth / 2) - g_Player4.fLength;
					}
					else if (g_Player4.pos[SENTER].x >= pEnemy->pos.x + EnemyWidth / 2)
					{
						g_Player4.pos[SENTER].x = (pEnemy->pos.x + EnemyWidth / 2) + g_Player4.fLength;
					}
					else if (g_Player4.pos[SENTER].y <= pEnemy->pos.y - EnemyHeight / 2)
					{
						g_Player4.pos[SENTER].y = (pEnemy->pos.y - EnemyHeight / 2) - g_Player4.fLength;
					}
					else if (g_Player4.pos[SENTER].y >= pEnemy->pos.y + EnemyHeight / 2)
					{
						g_Player4.pos[SENTER].y = (pEnemy->pos.y + EnemyHeight / 2) + g_Player4.fLength;
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
				if (pBoss->pos.x - EnemyWidth / 2 <= g_Player4.pos[SENTER].x + g_Player4.fLength && pBoss->pos.x + EnemyWidth / 2 >= g_Player4.pos[SENTER].x - g_Player4.fLength && pBoss->pos.y - EnemyHeight / 2 <= g_Player4.pos[SENTER].y + g_Player4.fLength && pBoss->pos.y + EnemyHeight / 2 >= g_Player4.pos[SENTER].y - g_Player4.fLength)
				{
					if (g_Player4.pos[SENTER].x <= pBoss->pos.x - EnemyWidth / 2)
					{
						g_Player4.pos[SENTER].x = (pBoss->pos.x - EnemyWidth / 2) - g_Player4.fLength;
					}
					else if (g_Player4.pos[SENTER].x >= pBoss->pos.x + EnemyWidth / 2)
					{
						g_Player4.pos[SENTER].x = (pBoss->pos.x + EnemyWidth / 2) + g_Player4.fLength;
					}
					else if (g_Player4.pos[SENTER].y <= pBoss->pos.y - EnemyHeight / 2)
					{
						g_Player4.pos[SENTER].y = (pBoss->pos.y - EnemyHeight / 2) - g_Player4.fLength;
					}
					else if (g_Player4.pos[SENTER].y >= pBoss->pos.y + EnemyHeight / 2)
					{
						g_Player4.pos[SENTER].y = (pBoss->pos.y + EnemyHeight / 2) + g_Player4.fLength;
					}
				}
			}
		}


		if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
		{
			if (pPlayer->pos[SENTER].x - pPlayer->fLength <= g_Player4.pos[SENTER].x + g_Player4.fLength && pPlayer->pos[SENTER].x + pPlayer->fLength >= g_Player4.pos[SENTER].x - g_Player4.fLength && pPlayer->pos[SENTER].y - pPlayer->fLength <= g_Player4.pos[SENTER].y + g_Player4.fLength && pPlayer->pos[SENTER].y + pPlayer->fLength >= g_Player4.pos[SENTER].y - g_Player4.fLength)
			{
				if (g_Player4.pos[SENTER].x <= pPlayer->pos[SENTER].x - pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_4) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer->pos[SENTER].x = (g_Player4.pos[SENTER].x + g_Player4.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].x = (pPlayer->pos[SENTER].x - pPlayer->fLength) - g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer->pos[SENTER].x = (g_Player4.pos[SENTER].x + g_Player4.fLength) + pPlayer->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].x = (pPlayer->pos[SENTER].x - pPlayer->fLength) - g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].x >= pPlayer->pos[SENTER].x + pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_4) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer->pos[SENTER].x = (g_Player4.pos[SENTER].x - g_Player4.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].x = (pPlayer->pos[SENTER].x + pPlayer->fLength) + g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer->pos[SENTER].x = (g_Player4.pos[SENTER].x - g_Player4.fLength) - pPlayer->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].x = (pPlayer->pos[SENTER].x + pPlayer->fLength) + g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].y <= pPlayer->pos[SENTER].y - pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_4) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer->pos[SENTER].y = (g_Player4.pos[SENTER].y + g_Player4.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].y = (pPlayer->pos[SENTER].y - pPlayer->fLength) - g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer->pos[SENTER].y = (g_Player4.pos[SENTER].y + g_Player4.fLength) + pPlayer->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].y = (pPlayer->pos[SENTER].y - pPlayer->fLength) - g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].y >= pPlayer->pos[SENTER].y + pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_4) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer->pos[SENTER].y = (g_Player4.pos[SENTER].y - g_Player4.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].y = (pPlayer->pos[SENTER].y + pPlayer->fLength) + g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer->pos[SENTER].y = (g_Player4.pos[SENTER].y - g_Player4.fLength) - pPlayer->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].y = (pPlayer->pos[SENTER].y + pPlayer->fLength) + g_Player4.fLength;
						}
					}
				}

				//pPlayer->move = g_Player4.move;
				//g_Player4.move = pPlayer->move;
			}
		}
		if (pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE)
		{
			if (pPlayer2->pos[SENTER].x - pPlayer2->fLength <= g_Player4.pos[SENTER].x + g_Player4.fLength && pPlayer2->pos[SENTER].x + pPlayer2->fLength >= g_Player4.pos[SENTER].x - g_Player4.fLength && pPlayer2->pos[SENTER].y - pPlayer2->fLength <= g_Player4.pos[SENTER].y + g_Player4.fLength && pPlayer2->pos[SENTER].y + pPlayer2->fLength >= g_Player4.pos[SENTER].y - g_Player4.fLength)
			{
				if (g_Player4.pos[SENTER].x <= pPlayer2->pos[SENTER].x - pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_4) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer2->pos[SENTER].x = (g_Player4.pos[SENTER].x + g_Player4.fLength) + pPlayer2->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].x = (pPlayer2->pos[SENTER].x - pPlayer2->fLength) - g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer2->pos[SENTER].x = (g_Player4.pos[SENTER].x + g_Player4.fLength) + pPlayer2->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].x = (pPlayer2->pos[SENTER].x - pPlayer2->fLength) - g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].x >= pPlayer2->pos[SENTER].x + pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_4) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer2->pos[SENTER].x = (g_Player4.pos[SENTER].x - g_Player4.fLength) - pPlayer2->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].x = (pPlayer2->pos[SENTER].x + pPlayer2->fLength) + g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer2->pos[SENTER].x = (g_Player4.pos[SENTER].x - g_Player4.fLength) - pPlayer2->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].x = (pPlayer2->pos[SENTER].x + pPlayer2->fLength) + g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].y <= pPlayer2->pos[SENTER].y - pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_4) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer2->pos[SENTER].y = (g_Player4.pos[SENTER].y + g_Player4.fLength) + pPlayer2->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].y = (pPlayer2->pos[SENTER].y - pPlayer2->fLength) - g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer2->pos[SENTER].y = (g_Player4.pos[SENTER].y + g_Player4.fLength) + pPlayer2->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].y = (pPlayer2->pos[SENTER].y - pPlayer2->fLength) - g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].y >= pPlayer2->pos[SENTER].y + pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_4) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer2->pos[SENTER].y = (g_Player4.pos[SENTER].y - g_Player4.fLength) - pPlayer2->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].y = (pPlayer2->pos[SENTER].y + pPlayer2->fLength) + g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer2->pos[SENTER].y = (g_Player4.pos[SENTER].y - g_Player4.fLength) - pPlayer2->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].y = (pPlayer2->pos[SENTER].y + pPlayer2->fLength) + g_Player4.fLength;
						}
					}
				}

				//pPlayer2->move = g_Player4.move;
				//g_Player4.move = pPlayer2->move;
			}
		}
		if (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE)
		{
			if (pPlayer3->pos[SENTER].x - pPlayer3->fLength <= g_Player4.pos[SENTER].x + g_Player4.fLength && pPlayer3->pos[SENTER].x + pPlayer3->fLength >= g_Player4.pos[SENTER].x - g_Player4.fLength && pPlayer3->pos[SENTER].y - pPlayer3->fLength <= g_Player4.pos[SENTER].y + g_Player4.fLength && pPlayer3->pos[SENTER].y + pPlayer3->fLength >= g_Player4.pos[SENTER].y - g_Player4.fLength)
			{
				if (g_Player4.pos[SENTER].x <= pPlayer3->pos[SENTER].x - pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_4) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer3->pos[SENTER].x = (g_Player4.pos[SENTER].x + g_Player4.fLength) + pPlayer3->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].x = (pPlayer3->pos[SENTER].x - pPlayer3->fLength) - g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer3->pos[SENTER].x = (g_Player4.pos[SENTER].x + g_Player4.fLength) + pPlayer3->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].x = (pPlayer3->pos[SENTER].x - pPlayer3->fLength) - g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].x >= pPlayer3->pos[SENTER].x + pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_4) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer3->pos[SENTER].x = (g_Player4.pos[SENTER].x - g_Player4.fLength) - pPlayer3->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].x = (pPlayer3->pos[SENTER].x + pPlayer3->fLength) + g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer3->pos[SENTER].x = (g_Player4.pos[SENTER].x - g_Player4.fLength) - pPlayer3->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].x = (pPlayer3->pos[SENTER].x + pPlayer3->fLength) + g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].y <= pPlayer3->pos[SENTER].y - pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_4) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer3->pos[SENTER].y = (g_Player4.pos[SENTER].y + g_Player4.fLength) + pPlayer3->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].y = (pPlayer3->pos[SENTER].y - pPlayer3->fLength) - g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer3->pos[SENTER].y = (g_Player4.pos[SENTER].y + g_Player4.fLength) + pPlayer3->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].y = (pPlayer3->pos[SENTER].y - pPlayer3->fLength) - g_Player4.fLength;
						}
					}
				}
				else if (g_Player4.pos[SENTER].y >= pPlayer3->pos[SENTER].y + pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_4) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_4))
					{
						pPlayer3->pos[SENTER].y = (g_Player4.pos[SENTER].y - g_Player4.fLength) - pPlayer3->fLength;
					}
					else
					{
						g_Player4.pos[SENTER].y = (pPlayer3->pos[SENTER].y + pPlayer3->fLength) + g_Player4.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_4), IsDirectInputControllerConnected(CONTROLLER_4))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_4) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_4))
						{
							pPlayer3->pos[SENTER].y = (g_Player4.pos[SENTER].y - g_Player4.fLength) - pPlayer3->fLength;
						}
						else
						{
							g_Player4.pos[SENTER].y = (pPlayer3->pos[SENTER].y + pPlayer3->fLength) + g_Player4.fLength;
						}
					}
				}

				//pPlayer3->move = g_Player4.move;
				//g_Player4.move = pPlayer3->move;
			}
		}
	}

	//�ʒu�̍X�V
	Boss* pBoss;
	pBoss = GetBoss();
	for (int i = 0; i < MAX_BOSS; i++, pBoss++)
	{
		if (pBoss->bUse == true)
		{
			g_Player4.pos[SENTER].x += (g_Player4.move.x - pBoss->move.x * PLAYER_FIELD_INA);
			g_Player4.pos[SENTER].y += (g_Player4.move.y - pBoss->move.y * PLAYER_FIELD_INA);
			break;
		}
		else if (i == MAX_BOSS - 1)
		{
			g_Player4.pos[SENTER].x += g_Player4.move.x;
			g_Player4.pos[SENTER].y += g_Player4.move.y;
		}
	}

	if (GetMode() == MODE_VS)
	{
		//���ꂷ���h�~
		Player* pPlayer;
		pPlayer = GetPlayer();
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player4.pos[SENTER].x - pPlayer->pos[SENTER].x;
		Ylong = g_Player4.pos[SENTER].y - pPlayer->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player4.pos[SENTER].x = pPlayer->pos[SENTER].x + Xlong * LongIna;
			g_Player4.pos[SENTER].y = pPlayer->pos[SENTER].y + Ylong * LongIna;
		}

		//���ꂷ���h�~
		Player* pPlayer2;
		pPlayer2 = GetPlayer2();
		Xlong = g_Player4.pos[SENTER].x - pPlayer2->pos[SENTER].x;
		Ylong = g_Player4.pos[SENTER].y - pPlayer2->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player4.pos[SENTER].x = pPlayer2->pos[SENTER].x + Xlong * LongIna;
			g_Player4.pos[SENTER].y = pPlayer2->pos[SENTER].y + Ylong * LongIna;
		}

		//���ꂷ���h�~
		Player* pPlayer3;
		pPlayer3 = GetPlayer3();
		Xlong = g_Player4.pos[SENTER].x - pPlayer3->pos[SENTER].x;
		Ylong = g_Player4.pos[SENTER].y - pPlayer3->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player4.pos[SENTER].x = pPlayer3->pos[SENTER].x + Xlong * LongIna;
			g_Player4.pos[SENTER].y = pPlayer3->pos[SENTER].y + Ylong * LongIna;
		}
	}
	else
	{
		//���ꂷ���h�~
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player4.pos[SENTER].x - pBoss->pos.x;
		Ylong = g_Player4.pos[SENTER].y - pBoss->pos.y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player4.pos[SENTER].x = pBoss->pos.x + Xlong * LongIna;
			g_Player4.pos[SENTER].y = pBoss->pos.y + Ylong * LongIna;
		}
	}

	if (bOberP1())
	{
		Player* pPlayer;
		pPlayer = GetPlayer();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer->pos[SENTER].x - g_Player4.pos[SENTER].x;
		Ylong = pPlayer->pos[SENTER].y - g_Player4.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player4.bDisp[LEFTUP] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player4.pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA_VS, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player4.pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player4.bDisp[LEFTUP] = false;
		}
	}
	if (bOberP2())
	{
		Player* pPlayer2;
		pPlayer2 = GetPlayer2();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer2->pos[SENTER].x - g_Player4.pos[SENTER].x;
		Ylong = pPlayer2->pos[SENTER].y - g_Player4.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player4.bDisp[RIGHTUP] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player4.pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA_VS, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player4.pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player4.bDisp[RIGHTUP] = false;
		}
	}
	if (bOberP3())
	{
		Player* pPlayer3;
		pPlayer3 = GetPlayer3();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer3->pos[SENTER].x - g_Player4.pos[SENTER].x;
		Ylong = pPlayer3->pos[SENTER].y - g_Player4.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player4.bDisp[LEFTDOWN] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player4.pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player4.pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player4.bDisp[LEFTDOWN] = false;
		}
	}

	//�ړ��ʂ̍X�V(����)
	g_Player4.move.x += (PLAYER_SPEED_DEF - g_Player4.move.x) * PLAYER_INA;
	g_Player4.move.y += (PLAYER_SPEED_DEF - g_Player4.move.y) * PLAYER_INA;

	g_pVtxBuffPlayer4->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

		//���W�ݒ�
	pVtx[0].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[0].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[1].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength;
	pVtx[2].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player4.pos[SENTER].x + sinf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength;
	pVtx[3].pos.y = g_Player4.pos[SENTER].y + cosf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength;
	pVtx[3].pos.z = 0.0f;


	pVtx += VT_MAX;

	for (int i = 1; i < POS_MAX; i++)
	{
		if (GetMode() == MODE_VS)
		{
			//���W�ݒ�
			pVtx[0].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA_VS;
			pVtx[3].pos.z = 0.0f;
		}
		else
		{
			//���W�ݒ�
			pVtx[0].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI - g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - (D3DX_PI + g_Player4.fAngle)) * g_Player4.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z - g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player4.pos[i].x + sinf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player4.pos[i].y + cosf(g_Player4.rot.z + g_Player4.fAngle) * g_Player4.fLength * SPLIT_INA;
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += VT_MAX;
	}

	g_pVtxBuffPlayer4->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	g_Player4.nCounterAnim++;
	if ((g_Player4.nCounterAnim % UP_TIME) == 0)//�X�V����
	{
		g_Player4.nPatternAnim = (g_Player4.nPatternAnim + 1) % (U_MAX * V_MAX);

		g_pVtxBuffPlayer4->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

		for (int i = 0; i < POS_MAX; i++)
		{
			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player4.nPatternAnim, UV_DEF / V_MAX * (g_Player4.nPatternAnim / U_MAX));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player4.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player4.nPatternAnim / U_MAX));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player4.nPatternAnim, UV_DEF / V_MAX * (g_Player4.nPatternAnim / U_MAX) + UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player4.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player4.nPatternAnim / U_MAX) + UV_DEF / V_MAX);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer4->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}

	if (g_Player4.state == PLAYERSTATE_DIE)
	{
		g_bOber4 = false;
	}
	else if (g_Player4.pos[SENTER].x + g_Player4.fLength <= 0 || g_Player4.pos[SENTER].x - g_Player4.fLength >= SCREEN_WIDTH || g_Player4.pos[SENTER].y + g_Player4.fLength <= 0 || g_Player4.pos[SENTER].y - g_Player4.fLength >= SCREEN_HEIGHT)//��ʉE�[
	{//��ʊO
		g_bOber4 = true;
	}
	else
	{//��ʓ�
		g_bOber4 = false;
	}

	switch (g_Player4.state)
	{
	case PLAYERSTATE_NORMAL:
		SetVibrate(0.0f, CONTROLLER_1);

		g_pVtxBuffPlayer4->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

		for (int i = 0; i < POS_MAX; i++)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer4->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		break;
	case PLAYERSTATE_APPEAR:
		g_Player4.nCounterState++;
		if (g_Player4.nCounterState >= 60)
		{
			g_Player4.state = PLAYERSTATE_NORMAL;
			g_Player4.bDisp[SENTER] = true;
		}
		else if (g_Player4.nCounterState % 5 == 0)
		{
			g_Player4.bDisp[SENTER] = !g_Player4.bDisp[SENTER];
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player4.nCounterState--;
		if (g_Player4.nCounterState <= 0)
		{
			g_Player4.state = PLAYERSTATE_NORMAL;

			g_pVtxBuffPlayer4->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer4->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
		break;
	case PLAYERSTATE_DIE:
		g_Player4.bDisp[SENTER] = false;
		break;
	}

	if (GetMode() == MODE_VS)
	{
		g_bOber4 = true;
	}
}

//-------------------
//�v���C���[�`�揈��
//-------------------
void DrawPlayer4(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Player4.bDisp[SENTER])
	{
		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer4, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer4);

		if (GetMode() != MODE_VS)
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				0,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}

		if (bOberP1() && g_Player4.bDisp[LEFTUP])
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
		if (bOberP2() && g_Player4.bDisp[RIGHTUP])
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX * 2,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
		if (bOberP3() && g_Player4.bDisp[LEFTDOWN])
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX * 3,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
		if (g_bOber4)
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX * 4,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//-------------------
//�q�b�g����
//-------------------
void HitPlayer4(int nDamage)
{
	if (g_Player4.state != PLAYERSTATE_DIE && g_Player4.state != PLAYERSTATE_APPEAR)
	{
		VERTEX_2D* pVtx;

		g_Player4.nLife -= nDamage;

		if (g_Player4.nLife <= 0)
		{
			if (g_Player4.nRema <= 0)
			{
				//SetExplosion(g_Player4.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player4.fLength);
				SetExef(g_Player4.pos[SENTER], g_Player4.fLength, true);
				if (GetMode() == MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player4.pos[LEFTUP], g_Player4.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player4.pos[RIGHTUP], g_Player4.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player4.pos[LEFTDOWN], g_Player4.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player4.pos[RIGHTDOWN], g_Player4.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player4.pos[LEFTUP], g_Player4.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player4.pos[RIGHTUP], g_Player4.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player4.pos[LEFTDOWN], g_Player4.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player4.pos[RIGHTDOWN], g_Player4.fLength * SPLIT_INA, false);
					}
				}
				g_Player4.state = PLAYERSTATE_DIE;
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
			}
			else
			{
				//SetExplosion(g_Player4.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player4.fLength);
				SetExef(g_Player4.pos[SENTER], g_Player4.fLength, true);
				if (GetMode() == MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player4.pos[LEFTUP], g_Player4.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player4.pos[RIGHTUP], g_Player4.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player4.pos[LEFTDOWN], g_Player4.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player4.pos[RIGHTDOWN], g_Player4.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player4.pos[LEFTUP], g_Player4.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player4.pos[RIGHTUP], g_Player4.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player4.pos[LEFTDOWN], g_Player4.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player4.pos[RIGHTDOWN], g_Player4.fLength * SPLIT_INA, false);
					}
				}
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
				g_Player4.state = PLAYERSTATE_APPEAR;
				g_Player4.nCounterState = 0;
				g_Player4.nRema--;
				g_Player4.nLife = 100;
			}
			SetVibrate(1.0f, CONTROLLER_4);
		}
		else
		{
			g_Player4.state = PLAYERSTATE_DAMAGE;
			g_Player4.nCounterState = 5;
			if (GetMode() == MODE_GAME)
			{
				AddScore(-nDamage * SCORE_MISS);
			}

			g_pVtxBuffPlayer4->Lock(0, 0, (void**)&pVtx, 0);

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer4->Unlock();//�o�b�t�@�̃A�����b�N
		}
	}
}

//-------------------------
//�v���C���[�o�b�t�@�̎擾
//-------------------------
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer4(void)
{
	return g_pVtxBuffPlayer4;
}

//-------------------------
//�v���C���[�擾
//-------------------------
Player* GetPlayer4(void)
{
	return &g_Player4;
}

//------------------
//��ʊO
//------------------
bool bOberP4(void)
{
	return g_bOber4;
}