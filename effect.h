//---------------------------------------
//
//�G�t�F�N�g�\�������̒�`�E�錾[effect.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define MAX_EFFECT (655360)//�ő吔

void InitEffect(void);//�G�t�F�N�g�̏���������
void UninitEffect(void);//�G�t�F�N�g�̏I������
void UpdateEffect(void);//�G�t�F�N�g�̍X�V����
void DrawEffect(void);//�G�t�F�N�g�̕`�揈��
void SetEffect(D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col, float fRadius, int nLife, bool bSenter);//�G�t�F�N�g�ݒ�

#endif _EFFECT_H_
