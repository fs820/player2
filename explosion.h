//---------------------------------------
//
//�����\�������̒�`�E�錾[explosion.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#define U_MAX_E (8)//�e�N�X�`���̐�(��)
#define V_MAX_E (8)//�e�N�X�`���̐�(�c)

#include"main.h"

//�v���g�^�C�v�錾
void InitExplosion(void);//�����̏���������
void UninitExplosion(void);//�����̏I������
void UpdateExplosion(void);//�����̍X�V����
void DrawExplosion(void);//�����̕`�揈��
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fLengthBullet);//�����ݒ�

#endif _EXPLOSION_H_
