//------------------------------------------
//
//�f����ʂ̒�`�E�錾[demo.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _DEMO_H_
#define _DEMO_H_

#include"game.h"

void InitDemo(void);//�f������������
void UninitDemo(void);//�f���I������
void UpdateDemo(void);//�f���X�V����
void DrawDemo(void);//�f���`�揈��
void SetGameStateDemo(GAMESTATE state);//��Ԑݒ�
GAMESTATE GetGameStateDemo(void);//��Ԏ擾

#endif _DEMO_H_