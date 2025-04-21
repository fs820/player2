//----------------------------------------
//
//�v���C���[�\������[player.cpp]
//Author fuma sato
//
//----------------------------------------

#include"player.h"
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
#include"p1back.h"
#include"vs.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;//�o�b�t�@�̃|�C���^
Player g_Player;//�v���C���[���
D3DXVECTOR3 g_moveBullet;//�e���ˌ����E�X�s�[�h�Ǘ�
bool g_bOber = false;

//----------------------
//�v���C���[�̏���������
//----------------------
void InitPlayer(void)
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
		&g_pVtxBuffPlayer,
		NULL
	);

	g_Player.nCounterAnim = 0;
	g_Player.nPatternAnim = 0;
	g_Player.pos[SENTER] = D3DXVECTOR3(PLAYER_WIDTH, SCREEN_HEIGHT / 3, 0);//�����ʒu
	if (GetMode()==MODE_VS)
	{
		g_Player.pos[LEFTUP] = D3DXVECTOR3((SCREEN_WIDTH * SPLIT_INA_VS) * 0.5f, (SCREEN_HEIGHT * SPLIT_INA_VS) * 0.5f, 0);//�����ʒu
		g_Player.pos[RIGHTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player.pos[LEFTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player.pos[RIGHTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
	}
	else
	{
		g_Player.pos[LEFTUP] = D3DXVECTOR3((SCREEN_WIDTH * SPLIT_INA) * 0.5f, (SCREEN_HEIGHT * SPLIT_INA) * 0.5f, 0);//�����ʒu
		g_Player.pos[RIGHTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player.pos[LEFTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
		g_Player.pos[RIGHTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
	}
	g_Player.Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����ʒu
	g_Player.move = D3DXVECTOR3(PLAYER_SPEED_DEF, PLAYER_SPEED_DEF, PLAYER_SPEED_DEF);//�v���C���[�X�s�[�h
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
	g_Player.rotDef = g_Player.rot;//�����ۑ�
	g_Player.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//�Ίp���̒���
	g_Player.fLengthDef = g_Player.fLength;//�����ۑ�
	g_Player.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//�p�x
	g_Player.BulletType = TYPE_NORMAL;
	g_Player.BulletCount = 0;
	g_Player.nRema = 3;
	g_Player.nLife = 100;
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.nCounterState = 0;
	for (int i = 0; i < POS_MAX; i++)
	{
		g_Player.bDisp[i] = true;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_PLAYER1,
		&g_pTexturePlayer
	);

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	//���W�ݒ�
	pVtx[0].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
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

		if (GetMode()==MODE_VS)
		{
			//���W�ݒ�
			pVtx[0].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
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
			pVtx[0].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
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

	g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//-------------------
//�v���C���[�I������
//-------------------
void UninitPlayer(void)
{
	SetVibrate(0.0f, CONTROLLER_1);
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//-------------------
//�v���C���[�X�V����
//-------------------
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	float* pStick;
	float* pMouseMove;
	float fMouseWheel;

	//�ߋ��̈ʒu�̕ۑ�
	g_Player.Oldpos = g_Player.pos[SENTER];

	//�ړ�����

	//X
	if(IsXInputControllerConnected(CONTROLLER_1))
	{
		//���X�e�B�b�N����
		pStick = GetJoyStick(STICK_LEFT, CONTROLLER_1);
		g_Player.move.x += *pStick * PLAYER_SPEED_X;
		g_Player.move.y += -*(pStick + 1) * PLAYER_SPEED_Y;

		//�E�X�e�B�b�N����
		pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_1);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player.rot.z = atan2f(*(pStick + 1), *pStick);
		}
	}
	//d
	else if (IsDirectInputControllerConnected(CONTROLLER_1))
	{

		//���X�e�B�b�N����
		pStick = GetdJoyStick(STICK_LEFT, CONTROLLER_1);
		g_Player.move.x += *pStick * PLAYER_SPEED_X;
		g_Player.move.y += *(pStick + 1) * PLAYER_SPEED_Y;

		//�E�X�e�B�b�N����
		pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_1);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player.rot.z = -atan2f(*(pStick + 1), *pStick);
		}
	}

	//�}�E�X����
	pMouseMove = GetMouseMove();
	g_Player.move.x += *pMouseMove * PLAYER_SPEED_X;
	g_Player.move.y += *(pMouseMove + 1) * PLAYER_SPEED_Y;

	fMouseWheel = GetMouseWheel();
	g_Player.rot.z += fMouseWheel;

	if (GetKeyboradPress(DIK_A) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//A�L�[
	{
		if (GetKeyboradPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//����
			g_Player.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//����
			g_Player.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//��
			g_Player.move.x += -PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_D) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//D�L�[
	{
		if (GetKeyboradPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
		{//�E��
			g_Player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
		{//�E��
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//�E
			g_Player.move.x += PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_W) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) == true)//W�L�[
	{//��
		g_Player.move.y += -PLAYER_SPEED_Y;
	}
	else if (GetKeyboradPress(DIK_S) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) == true)//S�L�[
	{//��
		g_Player.move.y += PLAYER_SPEED_Y;
	}

	if (GetKeyboradPress(DIK_Q) == true || GetJoykeyPress(JOYKEY_B, CONTROLLER_1) == true)
	{
		g_Player.rot.z += PLAYER_ROT_SPEED;
	}

	if (GetKeyboradPress(DIK_E) == true || GetJoykeyPress(JOYKEY_Y, CONTROLLER_1) == true)
	{
		g_Player.rot.z += -PLAYER_ROT_SPEED;
	}

	if (GetKeyboradTrigger(DIK_R) == true || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_1) == true)
	{
		g_Player.rot = g_Player.rotDef;
	}

	if (GetKeyboradPress(DIK_Z) == true ||  GetMousePress(MOUSE_SENTER) == true || GetJoykeyPress(JOYKEY_A, CONTROLLER_1) == true)
	{
		if (g_Player.fLength >= 0.0f && g_Player.fLength <= PLAYER_SIZE_MAX)
		{
			g_Player.fLength += PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradPress(DIK_C) == true|| GetMousePress(MOUSE_RIGHT) == true || GetJoykeyPress(JOYKEY_X, CONTROLLER_1) == true)
	{
		if (g_Player.fLength >= PLAYER_SIZE_MIN)
		{
			g_Player.fLength += -PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradTrigger(DIK_V) == true || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_1) == true)
	{
		g_Player.fLength = g_Player.fLengthDef;
	}

	if (g_Player.state != PLAYERSTATE_APPEAR && g_Player.state != PLAYERSTATE_DIE)
	{
		if (GetKeyboradTrigger(DIK_SPACE) == true || GetMouseTrigger(MOUSE_LEFT) == true || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1) == true)
		{
			switch (g_Player.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
				break;
			case TYPE_3WEY:
				//�e�̐ݒ�
				Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//�e�̐ݒ�
				SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//�e�̐ݒ�
				SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
		else if (GetKeyboradRepeat(DIK_SPACE) == true || GetMouseRepeat(MOUSE_LEFT) == true || GetJoykeyRepeat(JOYKEY_RB, CONTROLLER_1) == true)
		{
			switch (g_Player.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
				break;
			case TYPE_3WEY:
				//�e�̐ݒ�
				Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_ALL:
				//�e�̐ݒ�
				SetAllBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SEARCH:
				//�e�̐ݒ�
				SetSearchBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//�e�̐ݒ�
				SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//�e�̐ݒ�
				SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

				if (g_Player.BulletCount <= 0)
				{
					g_Player.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
	}

	if (IsDirectInputControllerConnected(CONTROLLER_1))
	{
		if (GetdJoyPov(POV_UP, 0, CONTROLLER_1) == true)
		{
			g_Player.move.y += -PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHTUP, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_RIGHTDOWN, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_1) == true)
		{
			g_Player.move.y += PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFTDOWN, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += -PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_LEFTUP, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
	}

	if (!strcmp(ControllerName(CONTROLLER_1), ELE_CON))
	{
		if (GetdJoykeyPress(ELEKEY_B, CONTROLLER_1) == true)
		{
			g_Player.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(ELEKEY_Y, CONTROLLER_1) == true)
		{
			g_Player.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(ELEKEY_L3, CONTROLLER_1) == true)
		{
			g_Player.rot = g_Player.rotDef;
		}

		if (GetdJoykeyPress(ELEKEY_A, CONTROLLER_1) == true)
		{
			if (g_Player.fLength >= 0.0f && g_Player.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(ELEKEY_X, CONTROLLER_1) == true)
		{
			if (g_Player.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(ELEKEY_R3, CONTROLLER_1) == true)
		{
			g_Player.fLength = g_Player.fLengthDef;
		}

		if (g_Player.state != PLAYERSTATE_APPEAR && g_Player.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(ELEKEY_RB, CONTROLLER_1) == true)
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(ELEKEY_RB, CONTROLLER_1) == true)
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_1), PS_CON))
	{
		if (GetdJoykeyPress(PSKEY_CR, CONTROLLER_1) == true)
		{
			g_Player.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(PSKEY_SQ, CONTROLLER_1) == true)
		{
			g_Player.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(PSKEY_L3, CONTROLLER_1) == true)
		{
			g_Player.rot = g_Player.rotDef;
		}

		if (GetdJoykeyPress(PSKEY_CI, CONTROLLER_1) == true)
		{
			if (g_Player.fLength >= 0.0f && g_Player.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(PSKEY_TRA, CONTROLLER_1) == true)
		{
			if (g_Player.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(PSKEY_R3, CONTROLLER_1) == true)
		{
			g_Player.fLength = g_Player.fLengthDef;
		}

		if (g_Player.state != PLAYERSTATE_APPEAR && g_Player.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(PSKEY_RB, CONTROLLER_1) == true)
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(PSKEY_RB, CONTROLLER_1) == true)
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_1), NIN_CON))
	{
	    if (GetdJoykeyPress(NINKEY_A, CONTROLLER_1) == true)
	    {
			g_Player.rot.z += PLAYER_ROT_SPEED;
		}

	    if (GetdJoykeyPress(NINKEY_X, CONTROLLER_1) == true)
	    {
			g_Player.rot.z += -PLAYER_ROT_SPEED;
		}

	    if (GetdJoykeyTrigger(NINKEY_L3, CONTROLLER_1) == true)
	    {
			g_Player.rot = g_Player.rotDef;
		}

	    if (GetdJoykeyPress(NINKEY_B, CONTROLLER_1) == true)
	    {
			if (g_Player.fLength >= 0.0f && g_Player.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player.fLength += PLAYER_SIZE_SPEED;
			}
		}

	    if (GetdJoykeyPress(NINKEY_Y, CONTROLLER_1) == true)
	    {
			if (g_Player.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player.fLength += -PLAYER_SIZE_SPEED;
			}
		}

	    if (GetdJoykeyTrigger(NINKEY_R3, CONTROLLER_1) == true)
	    {
			g_Player.fLength = g_Player.fLengthDef;
		}

		if (g_Player.state != PLAYERSTATE_APPEAR && g_Player.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(NINKEY_RB, CONTROLLER_1) == true)
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(NINKEY_RB, CONTROLLER_1))
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
		}
    }
	else if (!IsXInputControllerConnected(CONTROLLER_1) && IsDirectInputControllerConnected(CONTROLLER_1))
	{
		if (GetdJoykeyPress(DKEY_B, CONTROLLER_1) == true)
		{
			g_Player.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(DKEY_Y, CONTROLLER_1) == true)
		{
			g_Player.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(DKEY_L3, CONTROLLER_1) == true)
		{
			g_Player.rot = g_Player.rotDef;
		}

		if (GetdJoykeyPress(DKEY_A, CONTROLLER_1) == true)
		{
			if (g_Player.fLength >= 0.0f && g_Player.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(DKEY_X, CONTROLLER_1) == true)
		{
			if (g_Player.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(DKEY_R3, CONTROLLER_1) == true)
		{
			g_Player.fLength = g_Player.fLengthDef;
		}

		if (g_Player.state != PLAYERSTATE_APPEAR && g_Player.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(DKEY_RB, CONTROLLER_1) == true)
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(DKEY_RB, CONTROLLER_1) == true)
			{
				switch (g_Player.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet.x = sinf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet.y = cosf(g_Player.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//�e�̐ݒ�
					SetBullet(g_Player.pos[SENTER], g_moveBullet, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//�e�̐ݒ�
					Set3Bullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//�e�̐ݒ�
					SetAllBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//�e�̐ݒ�
					SetSearchBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//�e�̐ݒ�
					SetBomBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//�e�̐ݒ�
					SetSPBullet(g_Player.pos[SENTER], g_Player.rot.z, g_Player.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);

					if (g_Player.BulletCount <= 0)
					{
						g_Player.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
		}
	}

	if (g_Player.state != PLAYERSTATE_APPEAR && g_Player.state != PLAYERSTATE_DIE)
	{
		Enemy* pEnemy;
		Boss* pBoss;
		Player* pPlayer2, * pPlayer3, * pPlayer4;
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;

		pEnemy = GetEnemy();
		pBoss = GetBoss();
		pPlayer2 = GetPlayer2();
		pPlayer3 = GetPlayer3();
		pPlayer4 = GetPlayer4();
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
				if (pEnemy->pos.x - EnemyWidth / 2 <= g_Player.pos[SENTER].x + g_Player.fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_Player.pos[SENTER].x - g_Player.fLength && pEnemy->pos.y - EnemyHeight / 2 <= g_Player.pos[SENTER].y + g_Player.fLength && pEnemy->pos.y + EnemyHeight / 2 >= g_Player.pos[SENTER].y - g_Player.fLength)
				{
					if (g_Player.pos[SENTER].x <= pEnemy->pos.x - EnemyWidth / 2)
					{
						g_Player.pos[SENTER].x = (pEnemy->pos.x - EnemyWidth / 2) - g_Player.fLength;
					}
					else if (g_Player.pos[SENTER].x >= pEnemy->pos.x + EnemyWidth / 2)
					{
						g_Player.pos[SENTER].x = (pEnemy->pos.x + EnemyWidth / 2) + g_Player.fLength;
					}
					else if (g_Player.pos[SENTER].y <= pEnemy->pos.y - EnemyHeight / 2)
					{
						g_Player.pos[SENTER].y = (pEnemy->pos.y - EnemyHeight / 2) - g_Player.fLength;
					}
					else if (g_Player.pos[SENTER].y >= pEnemy->pos.y + EnemyHeight / 2)
					{
						g_Player.pos[SENTER].y = (pEnemy->pos.y + EnemyHeight / 2) + g_Player.fLength;
					}
				}
			}
		}

		for (int i = 0; i < MAX_BOSS; i++,pBoss++)
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
				if (pBoss->pos.x - EnemyWidth / 2 <= g_Player.pos[SENTER].x + g_Player.fLength && pBoss->pos.x + EnemyWidth / 2 >= g_Player.pos[SENTER].x - g_Player.fLength && pBoss->pos.y - EnemyHeight / 2 <= g_Player.pos[SENTER].y + g_Player.fLength && pBoss->pos.y + EnemyHeight / 2 >= g_Player.pos[SENTER].y - g_Player.fLength)
				{
					if (g_Player.pos[SENTER].x <= pBoss->pos.x - EnemyWidth / 2)
					{
						g_Player.pos[SENTER].x = (pBoss->pos.x - EnemyWidth / 2) - g_Player.fLength;
					}
					else if (g_Player.pos[SENTER].x >= pBoss->pos.x + EnemyWidth / 2)
					{
						g_Player.pos[SENTER].x = (pBoss->pos.x + EnemyWidth / 2) + g_Player.fLength;
					}
					else if (g_Player.pos[SENTER].y <= pBoss->pos.y - EnemyHeight / 2)
					{
						g_Player.pos[SENTER].y = (pBoss->pos.y - EnemyHeight / 2) - g_Player.fLength;
					}
					else if (g_Player.pos[SENTER].y >= pBoss->pos.y + EnemyHeight / 2)
					{
						g_Player.pos[SENTER].y = (pBoss->pos.y + EnemyHeight / 2) + g_Player.fLength;
					}
				}
			}
		}

		if (pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE)
		{
			if (pPlayer2->pos[SENTER].x - pPlayer2->fLength <= g_Player.pos[SENTER].x + g_Player.fLength && pPlayer2->pos[SENTER].x + pPlayer2->fLength >= g_Player.pos[SENTER].x - g_Player.fLength && pPlayer2->pos[SENTER].y - pPlayer2->fLength <= g_Player.pos[SENTER].y + g_Player.fLength && pPlayer2->pos[SENTER].y + pPlayer2->fLength >= g_Player.pos[SENTER].y - g_Player.fLength)
			{
				if (g_Player.pos[SENTER].x <= pPlayer2->pos[SENTER].x - pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer2->pos[SENTER].x = (g_Player.pos[SENTER].x + g_Player.fLength) + pPlayer2->fLength;
					}
					else
					{
						g_Player.pos[SENTER].x = (pPlayer2->pos[SENTER].x - pPlayer2->fLength) - g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer2->pos[SENTER].x = (g_Player.pos[SENTER].x + g_Player.fLength) + pPlayer2->fLength;
						}
						else
						{
							g_Player.pos[SENTER].x = (pPlayer2->pos[SENTER].x - pPlayer2->fLength) - g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].x >= pPlayer2->pos[SENTER].x + pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer2->pos[SENTER].x = (g_Player.pos[SENTER].x - g_Player.fLength) - pPlayer2->fLength;
					}
					else
					{
						g_Player.pos[SENTER].x = (pPlayer2->pos[SENTER].x + pPlayer2->fLength) + g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer2->pos[SENTER].x = (g_Player.pos[SENTER].x - g_Player.fLength) - pPlayer2->fLength;
						}
						else
						{
							g_Player.pos[SENTER].x = (pPlayer2->pos[SENTER].x + pPlayer2->fLength) + g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].y <= pPlayer2->pos[SENTER].y - pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer2->pos[SENTER].y = (g_Player.pos[SENTER].y + g_Player.fLength) + pPlayer2->fLength;
					}
					else
					{
						g_Player.pos[SENTER].y = (pPlayer2->pos[SENTER].y - pPlayer2->fLength) - g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer2->pos[SENTER].y = (g_Player.pos[SENTER].y + g_Player.fLength) + pPlayer2->fLength;
						}
						else
						{
							g_Player.pos[SENTER].y = (pPlayer2->pos[SENTER].y - pPlayer2->fLength) - g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].y >= pPlayer2->pos[SENTER].y + pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer2->pos[SENTER].y = (g_Player.pos[SENTER].y - g_Player.fLength) - pPlayer2->fLength;
					}
					else
					{
						g_Player.pos[SENTER].y = (pPlayer2->pos[SENTER].y + pPlayer2->fLength) + g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer2->pos[SENTER].y = (g_Player.pos[SENTER].y - g_Player.fLength) - pPlayer2->fLength;
						}
						else
						{
							g_Player.pos[SENTER].y = (pPlayer2->pos[SENTER].y + pPlayer2->fLength) + g_Player.fLength;
						}
					}
				}

				//pPlayer2->move = g_Player.move;
				//g_Player.move = pPlayer2->move;
			}
		}
		if (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE)
		{
			if (pPlayer3->pos[SENTER].x - pPlayer3->fLength <= g_Player.pos[SENTER].x + g_Player.fLength && pPlayer3->pos[SENTER].x + pPlayer3->fLength >= g_Player.pos[SENTER].x - g_Player.fLength && pPlayer3->pos[SENTER].y - pPlayer3->fLength <= g_Player.pos[SENTER].y + g_Player.fLength && pPlayer3->pos[SENTER].y + pPlayer3->fLength >= g_Player.pos[SENTER].y - g_Player.fLength)
			{
				if (g_Player.pos[SENTER].x <= pPlayer3->pos[SENTER].x - pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer3->pos[SENTER].x = (g_Player.pos[SENTER].x + g_Player.fLength) + pPlayer3->fLength;
					}
					else
					{
						g_Player.pos[SENTER].x = (pPlayer3->pos[SENTER].x - pPlayer3->fLength) - g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer3->pos[SENTER].x = (g_Player.pos[SENTER].x + g_Player.fLength) + pPlayer3->fLength;
						}
						else
						{
							g_Player.pos[SENTER].x = (pPlayer3->pos[SENTER].x - pPlayer3->fLength) - g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].x >= pPlayer3->pos[SENTER].x + pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer3->pos[SENTER].x = (g_Player.pos[SENTER].x - g_Player.fLength) - pPlayer3->fLength;
					}
					else
					{
						g_Player.pos[SENTER].x = (pPlayer3->pos[SENTER].x + pPlayer3->fLength) + g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer3->pos[SENTER].x = (g_Player.pos[SENTER].x - g_Player.fLength) - pPlayer3->fLength;
						}
						else
						{
							g_Player.pos[SENTER].x = (pPlayer3->pos[SENTER].x + pPlayer3->fLength) + g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].y <= pPlayer3->pos[SENTER].y - pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer3->pos[SENTER].y = (g_Player.pos[SENTER].y + g_Player.fLength) + pPlayer3->fLength;
					}
					else
					{
						g_Player.pos[SENTER].y = (pPlayer3->pos[SENTER].y - pPlayer3->fLength) - g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer3->pos[SENTER].y = (g_Player.pos[SENTER].y + g_Player.fLength) + pPlayer3->fLength;
						}
						else
						{
							g_Player.pos[SENTER].y = (pPlayer3->pos[SENTER].y - pPlayer3->fLength) - g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].y >= pPlayer3->pos[SENTER].y + pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer3->pos[SENTER].y = (g_Player.pos[SENTER].y - g_Player.fLength) - pPlayer3->fLength;
					}
					else
					{
						g_Player.pos[SENTER].y = (pPlayer3->pos[SENTER].y + pPlayer3->fLength) + g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer3->pos[SENTER].y = (g_Player.pos[SENTER].y - g_Player.fLength) - pPlayer3->fLength;
						}
						else
						{
							g_Player.pos[SENTER].y = (pPlayer3->pos[SENTER].y + pPlayer3->fLength) + g_Player.fLength;
						}
					}
				}

				//pPlayer3->move = g_Player.move;
				//g_Player.move = pPlayer3->move;
			}
		}
		if (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE)
		{
			if (pPlayer4->pos[SENTER].x - pPlayer4->fLength <= g_Player.pos[SENTER].x + g_Player.fLength && pPlayer4->pos[SENTER].x + pPlayer4->fLength >= g_Player.pos[SENTER].x - g_Player.fLength && pPlayer4->pos[SENTER].y - pPlayer4->fLength <= g_Player.pos[SENTER].y + g_Player.fLength && pPlayer4->pos[SENTER].y + pPlayer4->fLength >= g_Player.pos[SENTER].y - g_Player.fLength)
			{
				if (g_Player.pos[SENTER].x <= pPlayer4->pos[SENTER].x - pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer4->pos[SENTER].x = (g_Player.pos[SENTER].x + g_Player.fLength) + pPlayer4->fLength;
					}
					else
					{
						g_Player.pos[SENTER].x = (pPlayer4->pos[SENTER].x - pPlayer4->fLength) - g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer4->pos[SENTER].x = (g_Player.pos[SENTER].x + g_Player.fLength) + pPlayer4->fLength;
						}
						else
						{
							g_Player.pos[SENTER].x = (pPlayer4->pos[SENTER].x - pPlayer4->fLength) - g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].x >= pPlayer4->pos[SENTER].x + pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer4->pos[SENTER].x = (g_Player.pos[SENTER].x - g_Player.fLength) - pPlayer4->fLength;
					}
					else
					{
						g_Player.pos[SENTER].x = (pPlayer4->pos[SENTER].x + pPlayer4->fLength) + g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer4->pos[SENTER].x = (g_Player.pos[SENTER].x - g_Player.fLength) - pPlayer4->fLength;
						}
						else
						{
							g_Player.pos[SENTER].x = (pPlayer4->pos[SENTER].x + pPlayer4->fLength) + g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].y <= pPlayer4->pos[SENTER].y - pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer4->pos[SENTER].y = (g_Player.pos[SENTER].y + g_Player.fLength) + pPlayer4->fLength;
					}
					else
					{
						g_Player.pos[SENTER].y = (pPlayer4->pos[SENTER].y - pPlayer4->fLength) - g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer4->pos[SENTER].y = (g_Player.pos[SENTER].y + g_Player.fLength) + pPlayer4->fLength;
						}
						else
						{
							g_Player.pos[SENTER].y = (pPlayer4->pos[SENTER].y - pPlayer4->fLength) - g_Player.fLength;
						}
					}
				}
				else if (g_Player.pos[SENTER].y >= pPlayer4->pos[SENTER].y + pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer4->pos[SENTER].y = (g_Player.pos[SENTER].y - g_Player.fLength) - pPlayer4->fLength;
					}
					else
					{
						g_Player.pos[SENTER].y = (pPlayer4->pos[SENTER].y + pPlayer4->fLength) + g_Player.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
						{
							pPlayer4->pos[SENTER].y = (g_Player.pos[SENTER].y - g_Player.fLength) - pPlayer4->fLength;
						}
						else
						{
							g_Player.pos[SENTER].y = (pPlayer4->pos[SENTER].y + pPlayer4->fLength) + g_Player.fLength;
						}
					}
				}

				//pPlayer4->move = g_Player.move;
				//g_Player.move = pPlayer4->move;
			}
		}
	}

	//�ʒu�̍X�V
	Boss* pBoss;
	pBoss = GetBoss();
	for (int i = 0; i < MAX_BOSS; i++, pBoss++)
	{
		if (pBoss->bUse==true)
		{
			g_Player.pos[SENTER].x += (g_Player.move.x - pBoss->move.x * PLAYER_FIELD_INA);
			g_Player.pos[SENTER].y += (g_Player.move.y - pBoss->move.y * PLAYER_FIELD_INA);
			break;
		}
		else if (i==MAX_BOSS-1)
		{
			g_Player.pos[SENTER].x += g_Player.move.x;
			g_Player.pos[SENTER].y += g_Player.move.y;
		}
	}

	if (GetMode() == MODE_VS)
	{
		//���ꂷ���h�~
		Player* pPlayer2;
		pPlayer2 = GetPlayer2();
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player.pos[SENTER].x - pPlayer2->pos[SENTER].x;
		Ylong = g_Player.pos[SENTER].y - pPlayer2->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player.pos[SENTER].x = pPlayer2->pos[SENTER].x + Xlong * LongIna;
			g_Player.pos[SENTER].y = pPlayer2->pos[SENTER].y + Ylong * LongIna;
		}

		//���ꂷ���h�~
		Player* pPlayer3;
		pPlayer3 = GetPlayer3();
		Xlong = g_Player.pos[SENTER].x - pPlayer3->pos[SENTER].x;
		Ylong = g_Player.pos[SENTER].y - pPlayer3->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player.pos[SENTER].x = pPlayer3->pos[SENTER].x + Xlong * LongIna;
			g_Player.pos[SENTER].y = pPlayer3->pos[SENTER].y + Ylong * LongIna;
		}

		//���ꂷ���h�~
		Player* pPlayer4;
		pPlayer4 = GetPlayer4();
		Xlong = g_Player.pos[SENTER].x - pPlayer4->pos[SENTER].x;
		Ylong = g_Player.pos[SENTER].y - pPlayer4->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player.pos[SENTER].x = pPlayer4->pos[SENTER].x + Xlong * LongIna;
			g_Player.pos[SENTER].y = pPlayer4->pos[SENTER].y + Ylong * LongIna;
		}
	}
	else
	{
		//���ꂷ���h�~
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player.pos[SENTER].x - pBoss->pos.x;
		Ylong = g_Player.pos[SENTER].y - pBoss->pos.y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * 2)
		{
			// �v���C���[�̈ʒu�𐧌�
			float LongIna = (SCREEN_WIDTH * 2) / Long;
			g_Player.pos[SENTER].x = pBoss->pos.x + Xlong * LongIna;
			g_Player.pos[SENTER].y = pBoss->pos.y + Ylong * LongIna;
		}
	}

	if (bOberP2())
	{
		Player* pPlayer2;
		pPlayer2 = GetPlayer2();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer2->pos[SENTER].x - g_Player.pos[SENTER].x;
		Ylong = pPlayer2->pos[SENTER].y - g_Player.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player.bDisp[RIGHTUP] = true;
			if (GetMode()==MODE_VS)
			{
				g_Player.pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA_VS, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player.pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player.bDisp[RIGHTUP] = false;
		}
	}
	if (bOberP3())
	{
		Player* pPlayer3;
		pPlayer3 = GetPlayer3();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer3->pos[SENTER].x - g_Player.pos[SENTER].x;
		Ylong = pPlayer3->pos[SENTER].y - g_Player.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player.bDisp[LEFTDOWN] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player.pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player.pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player.bDisp[LEFTDOWN] = false;
		}
	}
	if (bOberP4())
	{
		Player* pPlayer4;
		pPlayer4 = GetPlayer4();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer4->pos[SENTER].x - g_Player.pos[SENTER].x;
		Ylong = pPlayer4->pos[SENTER].y - g_Player.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player.bDisp[RIGHTDOWN] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player.pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player.pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player.bDisp[RIGHTDOWN] = false;
		}
	}

	//�ړ��ʂ̍X�V(����)
	g_Player.move.x += (PLAYER_SPEED_DEF - g_Player.move.x) * PLAYER_INA;
	g_Player.move.y += (PLAYER_SPEED_DEF - g_Player.move.y) * PLAYER_INA;

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

			//���W�ݒ�
	pVtx[0].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos[SENTER].x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos[SENTER].y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	pVtx += VT_MAX;

	for (int i = 1; i < POS_MAX; i++)
	{
		if (GetMode() == MODE_VS)
		{
			//���W�ݒ�
			pVtx[0].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA_VS;
			pVtx[3].pos.z = 0.0f;
		}
		else
		{
			//���W�ݒ�
			pVtx[0].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player.pos[i].x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player.pos[i].y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength * SPLIT_INA;
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += VT_MAX;

	}

	g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	g_Player.nCounterAnim++;
	if ((g_Player.nCounterAnim % UP_TIME) == 0)//�X�V����
	{
		g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % (U_MAX * V_MAX);

		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

		for (int i = 0; i < POS_MAX; i++)
		{
			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX) + UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX) + UV_DEF / V_MAX);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}

	if (g_Player.state == PLAYERSTATE_DIE)
	{
		g_bOber = false;
	}
	else if (g_Player.pos[SENTER].x + g_Player.fLength <= 0 || g_Player.pos[SENTER].x - g_Player.fLength >= SCREEN_WIDTH || g_Player.pos[SENTER].y + g_Player.fLength <= 0 || g_Player.pos[SENTER].y - g_Player.fLength >= SCREEN_HEIGHT)//��ʉE�[
	{//��ʊO
		g_bOber = true;
	}
	else
	{//��ʓ�
		g_bOber = false;
	}

	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		SetVibrate(0.0f, CONTROLLER_1);

		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

		for (int i = 0; i < POS_MAX; i++)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		break;
	case PLAYERSTATE_APPEAR:
		g_Player.nCounterState++;
		if (g_Player.nCounterState >= 60)
		{
			g_Player.state = PLAYERSTATE_NORMAL;
			g_Player.bDisp[SENTER] = true;
		}
		else if (g_Player.nCounterState % 5 == 0)
		{
			g_Player.bDisp[SENTER] = !g_Player.bDisp[SENTER];
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player.nCounterState--;
		if (g_Player.nCounterState <= 0)
		{
			g_Player.state = PLAYERSTATE_NORMAL;

			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
		break;
	case PLAYERSTATE_DIE:
		g_Player.bDisp[SENTER] = false;
		break;
	}

	if (GetMode()==MODE_VS)
	{
		g_bOber = true;
	}
}

//-------------------
//�v���C���[�`�揈��
//-------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Player.bDisp[SENTER])
	{
		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

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

		if (g_bOber)
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
		if (bOberP2() && g_Player.bDisp[RIGHTUP])
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX*2,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
		if (bOberP3() && g_Player.bDisp[LEFTDOWN])
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX*3,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
		if (bOberP4() && g_Player.bDisp[RIGHTDOWN])
		{
			//�v���C���[�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX*4,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//-------------------
//�q�b�g����
//-------------------
void HitPlayer(int nDamage)
{
	if (g_Player.state!=PLAYERSTATE_DIE&& g_Player.state != PLAYERSTATE_APPEAR)
	{
		VERTEX_2D* pVtx;

		g_Player.nLife -= nDamage;

		if (g_Player.nLife <= 0)
		{
			if (g_Player.nRema <= 0)
			{
				//SetExplosion(g_Player.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player.fLength);
				SetExef(g_Player.pos[SENTER], g_Player.fLength, true);
				if (GetMode()==MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player.pos[LEFTUP], g_Player.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player.pos[RIGHTUP], g_Player.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player.pos[LEFTDOWN], g_Player.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player.pos[RIGHTDOWN], g_Player.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player.pos[LEFTUP], g_Player.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player.pos[RIGHTUP], g_Player.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player.pos[LEFTDOWN], g_Player.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player.pos[RIGHTDOWN], g_Player.fLength * SPLIT_INA, false);
					}
				}
				g_Player.state = PLAYERSTATE_DIE;
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
			}
			else
			{
				//SetExplosion(g_Player.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player.fLength);
				SetExef(g_Player.pos[SENTER], g_Player.fLength, true);
				if (GetMode() == MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player.pos[LEFTUP], g_Player.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player.pos[RIGHTUP], g_Player.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player.pos[LEFTDOWN], g_Player.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player.pos[RIGHTDOWN], g_Player.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player.pos[LEFTUP], g_Player.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player.pos[RIGHTUP], g_Player.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player.pos[LEFTDOWN], g_Player.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player.pos[RIGHTDOWN], g_Player.fLength * SPLIT_INA, false);
					}
				}
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
				g_Player.state = PLAYERSTATE_APPEAR;
				g_Player.nCounterState = 0;
				g_Player.nRema--;
				g_Player.nLife = 100;
			}
			SetVibrate(1.0f, CONTROLLER_1);
		}
		else
		{
			g_Player.state = PLAYERSTATE_DAMAGE;
			g_Player.nCounterState = 5;
			if (GetMode()==MODE_GAME)
			{
				AddScore(-nDamage * SCORE_MISS);
			}

			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer->Unlock();//�o�b�t�@�̃A�����b�N
		}
	}
}

//-------------------------
//�v���C���[�o�b�t�@�̎擾
//-------------------------
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer(void)
{
	return g_pVtxBuffPlayer;
}

//-------------------------
//�v���C���[�擾
//-------------------------
Player* GetPlayer(void)
{
	return &g_Player;
}

//------------------
//��ʊO
//------------------
bool bOberP1(void)
{
	return g_bOber;
}