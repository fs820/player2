//---------------------------------------
//
//�v���C���[3�\�������̒�`�E�錾[player3.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER3_H_
#define _PLAYER3_H_

#include"main.h"
#include"player.h"

void InitPlayer3(void);//�v���C���[3�̏���������
void UninitPlayer3(void);//�v���C���[3�̏I������
void UpdatePlayer3(void);//�v���C���[3�̍X�V����
void DrawPlayer3(void);//�v���C���[3�̕`�揈��
void HitPlayer3(int nDamage);//�v���C���[3�q�b�g����
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer3(void);//�v���C���[3�o�b�t�@�̎擾
Player* GetPlayer3(void);//�v���C���[3���擾
bool bOberP3(void);//�v���C���[3����ʂɂ��邩

#endif _PLAYER3_H_