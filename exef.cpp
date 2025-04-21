//---------------------------------------
//
//�e�\������[exef.cpp]
//Author fuma sato
//
//---------------------------------------

#include"exef.h"
#include"bullet.h"
#include"player.h"
#include"enemy.h"
#include"effect.h"
#include"sound.h"

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	float fLength;
	int nLife;//����
	bool bSenter;
	bool bUse;//�g�p���Ă��邩�ǂ���
}Exef;

//�O���[�o��
Exef g_aExef[MAX_EXEF];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitExef(void)
{
	for (int i = 0; i < MAX_EXEF; i++)
	{
		g_aExef[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExef[i].fLength = 0.0f;
		g_aExef[i].nLife = EXEF_LIFE;
		g_aExef[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//----------------
//�e�̏I������
//----------------
void UninitExef(void)
{

}

//----------------
//�e�̍X�V����
//----------------
void UpdateExef(void)
{

	for (int i = 0; i < MAX_EXEF; i++)
	{
		if (g_aExef[i].bUse == true)
		{//�e���g�p����Ă���

			for (int i2 = 0; i2 < EF_NUM; i2++)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f), move = D3DXVECTOR3(0.0f,0.0f,0.0f);
				D3DCOLOR col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
				float fRadius=0.0f,fangle=0.0f,fspeed=0.0f;
				int nLife=0;
				pos=g_aExef[i].pos;
				fangle = (float)(rand() % 629 - 314) / 100.0f;
				fspeed = (float)(rand() % 100) / 10.0f + 1.0f;
				move.x = sinf(fangle) * fspeed;
				move.y = cosf(fangle) * fspeed;
				col = D3DXCOLOR((float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 51) / 100.0f);
				fRadius = (float)(rand() % (int)(g_aExef[i].fLength+1.0f) * 2.0f);
				nLife = g_aExef[i].nLife;

				SetEffect(pos, move, col, fRadius, nLife, g_aExef[i].bSenter);
			}

			g_aExef[i].nLife--;
			g_aExef[i].fLength *= ((float)g_aExef[i].nLife) / ((float)EXEF_LIFE);

			if (g_aExef[i].nLife <= 0)
			{
				g_aExef[i].bUse = false;
			}
		}
	}
}

//----------------
//�e�̕`�揈��
//----------------
void DrawExef(void)
{

}

//-------------------
//���ʏ�e
//-------------------
void SetExef(D3DXVECTOR3 pos, float fLength, bool bSenter)
{

	for (int i = 0; i < MAX_EXEF; i++)
	{
		if (g_aExef[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aExef[i].pos = pos;
			g_aExef[i].fLength=fLength;
			g_aExef[i].nLife = EXEF_LIFE;
			g_aExef[i].bSenter = bSenter;
			g_aExef[i].bUse = true;

			break;
		}
	}
	PlaySound(SOUND_LABEL_EXPLOSION);
}