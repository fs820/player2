//---------------------------------------
//
//�e�\�������̒�`�E�錾[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"

#define MAX_BULLET (512)//�ő吔
#define BULLET_WIDTH (25.0f)//�傫��
#define BULLET_HEIGHT (25.0f)//�傫��
#define BOM_SIZE_INA (20.0f)//����e�T�C�Y
#define SP_SIZE_INA (5.0f)//�X�y�V�����C�T�C�Y
#define BULLET_SPEED (15.0f)//�X�s�[�h
#define BULLET_SPEED_SP (25.0f)//�X�s�[�h
#define BULLET_INTER (20)//���ˊԊu
#define ALL_BULLET (10)//�S���ʒe�e��
#define ALL_BULLETS (6)//�U���e�e��
#define ALL_BULLETSP (200)//�X�y�V�����C�e��
#define BULLET_LIFE (500)//�̗�
#define HIT_INA (0.25f)

//���
typedef enum
{
	BULLETTYPE_PLAYER = 0,//�v���C���[1
	BULLETTYPE_PLAYER2,//�v���C���[2
	BULLETTYPE_PLAYER3,//�v���C���[3
	BULLETTYPE_PLAYER4,//�v���C���[4
	BULLETTYPE_ENEMY,//�G
	BULLETTYPE_BOSS,//�{�X
	BULLETTYPE_MAX//��ސ�
}BULLETTYPE;

void InitBullet(void);//�e�̏���������
void UninitBullet(void);//�e�̏I������
void UpdateBullet(void);//�e�̍X�V����
void DrawBullet(void);//�e�̕`�揈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLengthPlayer, int nLife, BULLETTYPE type,int nInter);//�ʏ�e
void Set3Bullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//3wey�e
void SetAllBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//�S���ʒe
void SetSearchBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//�U���e
void SetBomBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//����e
void SetSPBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//����e

#endif _BULLET_H_
