//------------------------------------------
//
//�Q�[�����C���̏����̒�`�E�錾[game.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"

#define INTER (60)//�I���P�\
#define SPLIT_INA (0.2f)//��ʕ����T�C�Y

//�Q�[����Ԃ̗񋓌^��`
typedef enum
{
	GAMESTATE_NONE=0,//�Q�[���łȂ�
	GAMESTATE_NORMAL,//�Q�[����
	GAMESTATE_PAUSE,//�|�[�Y��
	GAMESTATE_END,//�I��
	GAMESTATE_MAX//��
}GAMESTATE;

//����POS�̗񋓌^��`
typedef enum
{
	SENTER = 0,//����
	LEFTUP,//����
	RIGHTUP,//�E��
	LEFTDOWN,//����
	RIGHTDOWN,//�E��
	POS_MAX
}POS;

void InitGame(void);//�Q�[���̏���������
void UninitGame(void);//�Q�[���̏I������
void UpdateGame(void);//�Q�[���̍X�V����
void DrawGame(void);//�Q�[���̕`�揈��
void SetGameState(GAMESTATE state);//�Q�[���̏�Ԑݒ�
GAMESTATE GetGameState(void);//�Q�[����Ԏ擾
void SetBossNumber(int Boss);
int GetBossNumber(void);
bool GetClear(void);

#endif _GAME_H_
