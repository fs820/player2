//---------------------------------------
//
//�v���C���[�\�������̒�`�E�錾[player.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"main.h"
#include"game.h"
#include"item.h"

//�v���C���[���
#define PLAYER_WIDTH (180)//��
#define PLAYER_HEIGHT (90)//����
#define PLAYER_SPEED_DEF (0.0f)//��{�X�s�[�h
#define PLAYER_SPEED_X (1.5f)//����
#define PLAYER_SPEED_Y (1.5f)//����
#define PLAYER_INA (0.05f)//����
#define PLAYER_ROT_SPEED (0.05f)//��]���x
#define PLAYER_SIZE_SPEED (3.0f)//�g��k�����x
#define PLAYER_SIZE_MAX (300.0f)//�傫���ő�
#define PLAYER_SIZE_MIN (5.0f)//�傫���ŏ�
#define PLAYER_FIELD_INA (13.0f)//�t�B�[���h���x�{��
#define PLAYER_SPARSE_INA (5)//�ǂꂾ��������邩

#define U_MAX (1)//�e�N�X�`���̐�(��)
#define V_MAX (1)//�e�N�X�`���̐�(�c)

//�v���C���[��Ԃ̗񋓌^��`
typedef enum
{
	PLAYERSTATE_APPEAR = 0,//�������
	PLAYERSTATE_NORMAL,//�ʏ�
	PLAYERSTATE_DAMAGE,//�_���[�W
	PLAYERSTATE_DIE,//��
	PLAYERSTATE_MAX//��
}PLAYERSTATE;

//�v���C���[�\����
typedef struct
{
	int nCounterAnim;//�v���C���[�̍X�V�Ԋu
	int nPatternAnim;//�v���C���[�̃A�j���[�V�����p�^�[���Ǘ�
	D3DXVECTOR3 pos[POS_MAX];//�v���C���[���W�Ǘ�
	D3DXVECTOR3 Oldpos;//�O�t���[����pos
	D3DXVECTOR3 move;//�v���C���[�X�s�[�h�Ǘ�
	D3DXVECTOR3 rot;//�v���C���[�����Ǘ�
	D3DXVECTOR3 rotDef;//�v���C���[�����Ǘ�
	float fLength;//�Ίp���̒���(�傫��)
	float fLengthDef;//�Ίp���̒���(�傫��)�̏����l
	float fAngle;//�Ίp���̊p�x
	TYPE BulletType;//���̒e
	int BulletCount;//�e��
	int nRema;//�c�@
	int nLife;//�̗�
	PLAYERSTATE state;//���
	int nCounterState;//��ԃJ�E���^�[
	bool bDisp[POS_MAX];//�\�����
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);//�v���C���[�̏���������
void UninitPlayer(void);//�v���C���[�̏I������
void UpdatePlayer(void);//�v���C���[�̍X�V����
void DrawPlayer(void);//�v���C���[�̕`�揈��
void HitPlayer(int nDamage);//�v���C���[�q�b�g����
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer(void);//�v���C���[�o�b�t�@�̎擾
Player* GetPlayer(void);//�v���C���[���擾
bool bOberP1(void);//�v���C���[����ʂɂ��邩

#endif _PLAYER_H_