//------------------------------------------
//
//�`���[�g���A���̒�`�E�錾[tuto.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _TUTO_H_
#define _TUTO_H_

#include"game.h"

void InitTuto(void);//�`���[�g���A������������
void UninitTuto(void);//�`���[�g���A���I������
void UpdateTuto(void);//�`���[�g���A���X�V����
void DrawTuto(void);//�`���[�g���A���`�揈��
void SetTutoState(GAMESTATE state);//�`���[�g���A����Ԑݒ菈��
GAMESTATE GetTutoState(void);//�`���[�g���A����Ԏ擾����

#endif _TUTO_H_