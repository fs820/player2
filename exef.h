//---------------------------------------
//
//�p�[�e�B�N���\�������̒�`�E�錾[exef.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXEF_H_
#define _EXEF_H_

#include"main.h"

#define MAX_EXEF (1280)//�ő吔
#define EF_NUM (5)//�t���[��������
#define EXEF_LIFE (80)//�c���

void InitExef(void);//�p�[�e�B�N���̏���������
void UninitExef(void);//�p�[�e�B�N���̏I������
void UpdateExef(void);//�p�[�e�B�N���̍X�V����
void DrawExef(void);//�p�[�e�B�N���̕`�揈��
void SetExef(D3DXVECTOR3 pos,float fLength,bool bSenter);//�p�[�e�B�N���ݒ�

#endif _EXEF_H_