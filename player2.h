//---------------------------------------
//
//�v���C���[2�\�������̒�`�E�錾[player2.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER2_H_
#define _PLAYER2_H_

#include"main.h"
#include"player.h"

void InitPlayer2(void);//�v���C���[2�̏���������
void UninitPlayer2(void);//�v���C���[2�̏I������
void UpdatePlayer2(void);//�v���C���[2�̍X�V����
void DrawPlayer2(void);//�v���C���[2�̕`�揈��
void HitPlayer2(int nDamage);//�v���C���[2�q�b�g����
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer2(void);//�v���C���[2�o�b�t�@�̎擾
Player* GetPlayer2(void);//�v���C���[2���擾
bool bOberP2(void);//�v���C���[2����ʂɂ��邩

#endif _PLAYER2_H_