//------------------------------------
//
//�v���C���[�����}�[�N����[mark.cpp]
//Author fuma sato
//
//------------------------------------
#include"mark.h"
#include"player.h"
#include"player2.h"
#include"player3.h"
#include"player4.h"

#define MARK_MAX (4)
#define MARK_SIZE (64.0f)
#define MARK_SPEED (20.0f)

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;//�v���C���[���W�Ǘ�
	D3DXVECTOR3 move;//�v���C���[�X�s�[�h�Ǘ�
	D3DXVECTOR3 rot;//�v���C���[�����Ǘ�
	D3DCOLOR col;//�F
	float fLength;//�Ίp���̒���(�傫��)
	float fAngle;//�Ίp���̊p�x
}MARK;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMark = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMark = NULL;//�o�b�t�@�̃|�C���^
MARK g_Mark[MARK_MAX];
//-------------------
//�}�[�N�̏���������
//-------------------
void InitMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MARK_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMark,
		NULL
	);

	if (GetMode()==MODE_VS)
	{
		//�e�N�X�`���̓ǂݍ���
		HRESULT hr = D3DXCreateTextureFromFile
		(
			pDevice,
			TEXTURE_EFFECT,
			&g_pTextureMark
		);

		for (int i = 0; i < MARK_MAX; i++)
		{
			g_Mark[i].pos = D3DXVECTOR3(MARK_SIZE * VS_MARK_SIZE_INA, MARK_SIZE * VS_MARK_SIZE_INA, 0.0f);
			g_Mark[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_Mark[i].fLength = sqrtf(MARK_SIZE * VS_MARK_SIZE_INA * MARK_SIZE * VS_MARK_SIZE_INA + MARK_SIZE * VS_MARK_SIZE_INA * MARK_SIZE * VS_MARK_SIZE_INA) / 2.0f;//�Ίp���̒���
			g_Mark[i].fAngle = atan2f(MARK_SIZE * VS_MARK_SIZE_INA, MARK_SIZE * VS_MARK_SIZE_INA);//�p�x
		}
	}
	else
	{
		//�e�N�X�`���̓ǂݍ���
		HRESULT hr = D3DXCreateTextureFromFile
		(
			pDevice,
			TEXTURE_NAME_MARK,
			&g_pTextureMark
		);

		for (int i = 0; i < MARK_MAX; i++)
		{
			g_Mark[i].pos = D3DXVECTOR3(MARK_SIZE, MARK_SIZE, 0.0f);
			g_Mark[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_Mark[i].fLength = sqrtf(MARK_SIZE * MARK_SIZE + MARK_SIZE * MARK_SIZE) / 2.0f;//�Ίp���̒���
			g_Mark[i].fAngle = atan2f(MARK_SIZE, MARK_SIZE);//�p�x
		}
	}

	g_Mark[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.8f);
	g_Mark[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
	g_Mark[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
	g_Mark[3].col = D3DXCOLOR(0.5f, 0.0f, 0.5f, 0.8f);

	g_pVtxBuffMark->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	for (int i = 0; i < MARK_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�J���[
		pVtx[0].col = g_Mark[i].col;
		pVtx[1].col = g_Mark[i].col;
		pVtx[2].col = g_Mark[i].col;
		pVtx[3].col = g_Mark[i].col;
		
		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffMark->Unlock();//�o�b�t�@�̃A�����b�N
}

//--------------------
//�}�[�N�̏I������
//--------------------
void UninitMark(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMark != NULL)
	{
		g_pTextureMark->Release();
		g_pTextureMark = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMark != NULL)
	{
		g_pVtxBuffMark->Release();
		g_pVtxBuffMark = NULL;
	}
}

//---------------------
//�}�[�N�̍X�V����
//---------------------
void UpdateMark(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^

	Player* pPlayer, * pPlayer2, * pPlayer3, * pPlayer4;
	pPlayer = GetPlayer();
	pPlayer2 = GetPlayer2();
	pPlayer3 = GetPlayer3();
	pPlayer4 = GetPlayer4();
	float Xlong[MARK_MAX] = { 0.0f }, Ylong[MARK_MAX] = { 0.0f };

	if (GetMode()==MODE_VS)
	{
		Xlong[0] = pPlayer->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[0] = pPlayer->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[0] *= VS_MARK_INA;
		Ylong[0] *= VS_MARK_INA;
		g_Mark[0].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[0], -Ylong[0]));
		g_Mark[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[0], SCREEN_HEIGHT / 2 + Ylong[0], 0.0f);

		Xlong[1] = pPlayer2->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[1] = pPlayer2->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[1] *= VS_MARK_INA;
		Ylong[1] *= VS_MARK_INA;
		g_Mark[1].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[1], -Ylong[1]));
		g_Mark[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[1], SCREEN_HEIGHT / 2 + Ylong[1], 0.0f);

		Xlong[2] = pPlayer3->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[2] = pPlayer3->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[2] *= VS_MARK_INA;
		Ylong[2] *= VS_MARK_INA;
		g_Mark[2].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[2], -Ylong[2]));
		g_Mark[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[2], SCREEN_HEIGHT / 2 + Ylong[2], 0.0f);

		Xlong[3] = pPlayer4->pos[SENTER].x - SCREEN_WIDTH / 2;
		Ylong[3] = pPlayer4->pos[SENTER].y - SCREEN_HEIGHT / 2;
		Xlong[3] *= VS_MARK_INA;
		Ylong[3] *= VS_MARK_INA;
		g_Mark[3].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[3], -Ylong[3]));
		g_Mark[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + Xlong[3], SCREEN_HEIGHT / 2 + Ylong[3], 0.0f);
	}
	else
	{
		Xlong[0] = pPlayer->pos[SENTER].x - g_Mark[0].pos.x;
		Ylong[0] = pPlayer->pos[SENTER].y - g_Mark[0].pos.y;
		g_Mark[0].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[0], -Ylong[0]));
		g_Mark[0].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[0], -Ylong[0])) * MARK_SPEED, -cosf(atan2f(-Xlong[0], -Ylong[0])) * MARK_SPEED, 0.0f);

		Xlong[1] = pPlayer2->pos[SENTER].x - g_Mark[1].pos.x;
		Ylong[1] = pPlayer2->pos[SENTER].y - g_Mark[1].pos.y;
		g_Mark[1].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[1], -Ylong[1]));
		g_Mark[1].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, -cosf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, 0.0f);

		Xlong[2] = pPlayer3->pos[SENTER].x - g_Mark[2].pos.x;
		Ylong[2] = pPlayer3->pos[SENTER].y - g_Mark[2].pos.y;
		g_Mark[2].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[2], -Ylong[2]));
		g_Mark[2].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[2], -Ylong[2])) * MARK_SPEED, -cosf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, 0.0f);

		Xlong[3] = pPlayer4->pos[SENTER].x - g_Mark[3].pos.x;
		Ylong[3] = pPlayer4->pos[SENTER].y - g_Mark[3].pos.y;
		g_Mark[3].rot = D3DXVECTOR3(0.0f, 0.0f, atan2f(-Xlong[3], -Ylong[3]));
		g_Mark[3].move = D3DXVECTOR3(-sinf(atan2f(-Xlong[3], -Ylong[3])) * MARK_SPEED, -cosf(atan2f(-Xlong[1], -Ylong[1])) * MARK_SPEED, 0.0f);

		for (int i = 0; i < MARK_MAX; i++)
		{
			g_Mark[i].pos.x += g_Mark[i].move.x;
			g_Mark[i].pos.y += g_Mark[i].move.y;

			if (g_Mark[i].pos.x - g_Mark[i].fLength < 0)
			{
				g_Mark[i].pos.x = g_Mark[i].fLength;
			}
			if (g_Mark[i].pos.x + g_Mark[i].fLength > SCREEN_WIDTH)
			{
				g_Mark[i].pos.x = SCREEN_WIDTH - g_Mark[i].fLength;
			}
			if (g_Mark[i].pos.y - g_Mark[i].fLength < 0)
			{
				g_Mark[i].pos.y = g_Mark[i].fLength;
			}
			if (g_Mark[i].pos.y + g_Mark[i].fLength > SCREEN_HEIGHT)
			{
				g_Mark[i].pos.y = SCREEN_HEIGHT - g_Mark[i].fLength;
			}
		}
	}

	g_pVtxBuffMark->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	for (int i = 0; i < MARK_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI - g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - (D3DX_PI + g_Mark[i].fAngle)) * g_Mark[i].fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z - g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Mark[i].pos.x + sinf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, g_Mark[i].pos.y + cosf(g_Mark[i].rot.z + g_Mark[i].fAngle) * g_Mark[i].fLength, 0.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffMark->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------------
//�}�[�N�̕`�揈��
//----------------------
void DrawMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

    //�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffMark, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMark);

	if (bOberP1()|| GetMode() == MODE_VS)
	{
		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			0,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}

	if (bOberP2() || GetMode() == MODE_VS)
	{
		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			VT_MAX,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}

	if (bOberP3() || GetMode() == MODE_VS)
	{
		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			VT_MAX * 2,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}

	if (bOberP4() || GetMode() == MODE_VS)
	{
		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			VT_MAX * 3,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}