//---------------------------------------
//
//�v���C���[4�\�������̒�`�E�錾[player4.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER4_H_
#define _PLAYER4_H_

#include"main.h"
#include"player.h"

void InitPlayer4(void);//�v���C���[4�̏���������
void UninitPlayer4(void);//�v���C���[4�̏I������
void UpdatePlayer4(void);//�v���C���[4�̍X�V����
void DrawPlayer4(void);//�v���C���[4�̕`�揈��
void HitPlayer4(int nDamage);//�v���C���[4�q�b�g����
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer4(void);//�v���C���[4�o�b�t�@�̎擾
Player* GetPlayer4(void);//�v���C���[4���擾
bool bOberP4(void);//�v���C���[4����ʂɂ��邩

#endif _PLAYER4_H_