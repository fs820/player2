//---------------------------------------
//
//�f���v���C�R���s���[�^�[�\�������̒�`�E�錾[democpu.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _DEMOCPU_H_
#define _DEMOCPU_H_

#include"main.h"
#include"player.h"

//�v���C���[���
#define DEMOCPU_WIDTH (160)//��
#define DEMOCPU_HEIGHT (90)//����
#define DEMOCPU_SPEED_DEF (0.0f)//��{�X�s�[�h
#define DEMOCPU_SPEED_X (3.0f)//����
#define DEMOCPU_SPEED_Y (3.0f)//����
#define DEMOCPU_INA (0.2f)//����
#define DEMOCPU_ROT_SPEED (0.05f)//��]���x
#define DEMOCPU_SIZE_SPEED (3.0f)//�g��k���X�s�[�h
#define DEMOCPU_SIZE_MAX (300.0f)//�傫���̍ő�
#define DEMOCPU_SIZE_MIN (5.0f)//�傫���̍ŏ�

#define U_MAX_C (1)//�e�N�X�`���̐�(��)
#define V_MAX_C (1)//�e�N�X�`���̐�(�c)

//CPU�\����
typedef struct
{
	int nCounterAnim;//CPU�̍X�V�Ԋu
	int nPatternAnim;//CPU�̃A�j���[�V�����p�^�[���Ǘ�
	D3DXVECTOR3 pos;//CPU���W�Ǘ�
	D3DXVECTOR3 move;//CPU�X�s�[�h�Ǘ�
	D3DXVECTOR3 rot;//CPU�����Ǘ�
	D3DXVECTOR3 rotDef;//CPU�����Ǘ�
	float fLength;//�Ίp���̒���(�傫��)
	float fLengthDef;//�Ίp���̒���(�傫��)�̏����l
	float fAngle;//�Ίp���̊p�x
	int nLife;//�̗�
	PLAYERSTATE state;//���
	int nCounterState;//��ԃJ�E���^�[
}DEMOCPU;

void InitDemoCpu(void);//CPU�̏���������
void UninitDemoCpu(void);//CPU�̏I������
void UpdateDemoCpu(void);//CPU�̍X�V����
void DrawDemoCpu(void);//�|CPU�̕`�揈��
void HitDemoCpu(int nDamage);//�q�b�g����
PDIRECT3DVERTEXBUFFER9 GetDemoCpuBuffer(void);//CPU�o�b�t�@�̎擾
DEMOCPU* GetDemoCpu(void);//CPU���擾

#endif _DEMOCPU_H_