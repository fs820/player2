//------------------------------------------
//
//�ΐ탁�C���̏����̒�`�E�錾[vs.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _VS_H_
#define _VS_H_

#include"main.h"

#define INTER (60)//�I���P�\
#define SPLIT_INA_VS (0.5f)//��ʕ����T�C�Y

//�o�[�T�X�̗񋓌^��`
typedef enum
{
	VSSTATE_NONE = 0,//�o�[�T�X�łȂ�
	VSSTATE_NORMAL,//VS��
	VSSTATE_PAUSE,//�|�[�Y��
	VSSTATE_END,//�I��
	VSSTATE_MAX//��
}VSSTATE;

void InitVs(void);//�o�[�T�X����������
void UninitVs(void);//�o�[�T�X�I������
void UpdateVs(void);//�o�[�T�X�X�V����
void DrawVs(void);//�o�[�T�X�`�揈��
void SetVsState(VSSTATE state);//�o�[�T�X��Ԑݒ菈��
VSSTATE GetVsState(void);//�o�[�T�X��Ԏ擾����

#endif _VS_H_
