//---------------------------------------
//
//�w�i�\������[p3back.cpp]
//Author fuma sato
//
//---------------------------------------

#include"main.h"
#include"p3back.h"
#include"player3.h"
#include"boss.h"
#include"game.h"
#include"vs.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureP3Back = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffP3Back = NULL;//�o�b�t�@�̃|�C���^
float g_PosTexUP3;
float g_PosTexVP3;

//----------------------
//�w�i�̏���������
//----------------------
void InitP3Back(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffP3Back,
		NULL
	);

	//1��

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_FIELD,
		&g_pTextureP3Back
	);

	g_PosTexUP3 = 0.0f;
	g_PosTexVP3 = 0.0f;

	g_pVtxBuffP3Back->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	if (GetMode() == MODE_VS)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH * SPLIT_INA_VS, SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA_VS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH * SPLIT_INA_VS, SCREEN_HEIGHT, 0.0f);
	}
	else
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH * SPLIT_INA, SCREEN_HEIGHT - SCREEN_HEIGHT * SPLIT_INA, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH * SPLIT_INA, SCREEN_HEIGHT, 0.0f);
	}

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
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3 + 1.0f);

	g_pVtxBuffP3Back->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�I������
//-------------------
void UninitP3Back(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureP3Back != NULL)
	{
		g_pTextureP3Back->Release();
		g_pTextureP3Back = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffP3Back != NULL)
	{
		g_pVtxBuffP3Back->Release();
		g_pVtxBuffP3Back = NULL;
	}
}

//-------------------
//�w�i�X�V����
//-------------------
void UpdateP3Back(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	Player* pPlayer3;
	Boss* pBoss;
	float Xmove = 0.0f, Ymove = 0.0f;

	pPlayer3 = GetPlayer3();
	pBoss = GetBoss();

	Xmove = pPlayer3->pos[SENTER].x - pPlayer3->Oldpos.x;
	Ymove = pPlayer3->pos[SENTER].y - pPlayer3->Oldpos.y;
	Xmove += pBoss->move.x * PLAYER_FIELD_INA;
	Ymove += pBoss->move.y * PLAYER_FIELD_INA;

	g_PosTexUP3 += Xmove * 0.001f;
	g_PosTexVP3 += Ymove * 0.001f;

	g_pVtxBuffP3Back->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3 + 1.0f);

	g_pVtxBuffP3Back->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�`�揈��
//-------------------
void DrawP3Back(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffP3Back, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureP3Back);

	//�w�i�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,//�n�܂�̔ԍ�
		2//�|���S���̌�
	);
}