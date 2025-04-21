//---------------------------------------
//
//�e�\������[explosion.cpp]
//Author fuma sato
//
//---------------------------------------

#include"explosion.h"
#include"sound.h"

#define MAX_EXPLOSION (256)
#define EXPLOSION_WIDTH (25.0f)
#define EXPLOSION_HEIGHT (25.0f)

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXCOLOR col;
	int nCntAnim;//�ړ���
	int nPatternAnim;//����
	float fLength;
	bool bUse;//�g�p���Ă��邩�ǂ���
}Explosion;

//�O���[�o��
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;//�o�b�t�@�̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		g_aExplosion[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[i].nCntAnim = 0;
		g_aExplosion[i].nPatternAnim = 0;
		g_aExplosion[i].fLength = 0.0f;
		g_aExplosion[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_EXP,
		&g_pTextureExplosion
	);

	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);

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
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);

		pVtx += VT_MAX;//�|���S��1���i�߂�
	}
	g_pVtxBuffExplosion->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�e�̏I������
//----------------
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateExplosion(void)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == true)
		{//�e���g�p����Ă���

			g_aExplosion[i].nCntAnim++;
			if ((g_aExplosion[i].nCntAnim % UP_TIME) == 0)//�X�V����
			{
				g_aExplosion[i].nCntAnim = (g_aExplosion[i].nCntAnim + 1) % (U_MAX_E * V_MAX_E);

				g_aExplosion[i].nPatternAnim++;
				if (g_aExplosion[i].nPatternAnim >= (U_MAX_E * V_MAX_E))
				{
					g_aExplosion[i].bUse = false;
				}

				g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N
				pVtx += i * VT_MAX;

				//�e�N�X�`��
				pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);
				pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);

				g_pVtxBuffExplosion->Unlock();//�o�b�t�@�̃A�����b�N
			}
		}
	}
}

//----------------
//�e�̕`�揈��
//----------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == true)
		{//�e���g�p����Ă���
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

//-------------------
//�����ݒ�
//-------------------
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fLengthBullet)
{

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aExplosion[i].pos = pos;
			g_aExplosion[i].fLength = fLengthBullet;
			g_aExplosion[i].nCntAnim = 0;
			g_aExplosion[i].nPatternAnim = 0;
			g_aExplosion[i].col = col;
			g_aExplosion[i].bUse = true;

			//���_���W�ݒ�
			VERTEX_2D* pVtx;

			g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);

			g_pVtxBuffExplosion->Unlock();//�o�b�t�@�̃A�����b�N

		    PlaySound(SOUND_LABEL_EXPLOSION);
			break;
		}
	}
}