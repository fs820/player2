//---------------------------------------
//
//�{�X�\�������̒�`�E�錾[boss.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BOSS_H_

#define _BOSS_H_

#include"main.h"

//�}�N����`
#define MAX_BOSS (8)//�ő吔
#define NUM_BOSS (4)//��ސ�
#define BOSS_WIDTH (400)//��
#define BOSS_HEIGHT (360)//����
#define BOSS_WIDTH2 (450)//��
#define BOSS_HEIGHT2 (450)//����
#define BOSS_WIDTH3 (500)//��
#define BOSS_HEIGHT3 (440)//����
#define BOSS_WIDTH4 (400)//��
#define BOSS_HEIGHT4 (400)//����

//�{�X�̏�Ԃ̗񋓌^��`
typedef enum
{
	BOSSSTATE_NORMAL = 0,//�ʏ�
	BOSSSTATE_DAMAGE,//�_���[�W
	BOSSSTATE_MAX,//��Ԑ�
}BOSSSTATE;

//�{�X�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//����
	float fSpeed;
	int nType;//���
	BOSSSTATE state;//���
	int nCounterState;//�_���[�W�J�E���^�[
	float fLength;//�傫��
	int nLife;//�̗�
	bool bUse;//�g�p���Ă��邩
}Boss;

void InitBoss(void);//�{�X�̏���������
void UninitBoss(void);//�{�X�̏I������
void UpdateBoss(void);//�{�X�̍X�V����
void DrawBoss(void);//�{�X�̕`�揈��
void SetBoss(D3DXVECTOR3 pos, int nType);//�{�X�ݒu
void HitBoss(int nCntEnemy, int nDamage);//�{�X�q�b�g����
Boss* GetBoss(void);//�{�X���擾
int GetNumBoss(void);//�{�X�̐��擾

#endif _BOSS_H_

