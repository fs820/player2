//---------------------------------------
//
//�G�\�������̒�`�E�錾[enemy.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ENEMY_H_

#define _ENEMY_H_

#include"main.h"

//�}�N����`
#define MAX_ENEMY (128)//�G�̍ő吔
#define NUM_ENEMY (4)//�G�̎��
#define ENEMY_WIDTH (200)//��
#define ENEMY_HEIGHT (200)//����
#define ENEMY_WIDTH2 (193)//��
#define ENEMY_HEIGHT2 (109.5f)//����
#define ENEMY_WIDTH3 (246.5f)//��
#define ENEMY_HEIGHT3 (184.5f)//����
#define ENEMY_WIDTH4 (111)//��
#define ENEMY_HEIGHT4 (139)//����

//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,//�ʏ�
	ENEMYSTATE_DAMAGE,//�_���[�W
	ENEMYSTATE_MAX,//��
}ENEMYSTATE;

//�G�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	int nType;//���
	ENEMYSTATE state;//���
	int nCounterState;//��ԃJ�E���^�[
	float fLength;//�傫��
	int nLife;//�̗�
	bool bUse;//�����Ă��邩
}Enemy;

void InitEnemy(void);//�G�̏���������
void UninitEnemy(void);//�G�̏I������
void UpdateEnemy(void);//�G�̍X�V����
void DrawEnemy(void);//�G�̕`�揈��
void SetEnemy(D3DXVECTOR3 pos, int nType);//�G�ݒ�
void HitEnemy(int nCntEnemy, int nDamage);//�G�q�b�g
Enemy* GetEnemy(void);//�G���擾
int GetNumEnemy(void);//�G�̐��擾

#endif _ENEMY_H_

