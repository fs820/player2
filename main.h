//------------------------------------------
//
//�V���[�e�B���O�Q�[���v���O�����̃��C���̏����̒�`�E�錾[main.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

#include<windows.h>
#include<stdio.h>
#include<time.h>
#include <strsafe.h>
#include"d3dx9.h"
#include"dinput.h" // ���͂ɕK�v�ȃt�@�C��
#include"Xinput.h" // Xinput(�p�b�h����)
#include"xaudio2.h"

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")

// �}�N����`
#define FPS (1000/60) // �X�V�X�s�[�h
#define UP_TIME (3) // �e�N�X�`���X�s�[�h

// ���O
#define CLASS_NAME "WindowClass" // �N���X��
#define WINDOW_NAME "�E�҂͌��Ǖ��킪�������ƂɋC�t�����悤�ł�" // �E�C���h�E��
#define DATA_FILE "data\\DATA\\data.bin"//�X�R�A�ۑ��p�t�@�C��
#define TEXTURE_CURSOR "data\\TEXTURE\\MouseCursor.png"//�J�[�\��
#define TEXTURE_TITLE "data\\TEXTURE\\field003.jpg"//�^�C�g���w�i
#define TEXTURE_TITLEROGO "data\\TEXTURE\\TitleRogo.png"//�^�C�g�����S
#define TEXTURE_TITLEROGOTXT "data\\TEXTURE\\Rogo.png"//�^�C�g������
#define TEXTURE_SELECT1 "data\\TEXTURE\\Enter or Start.png"//�Z���N�g�\��
#define TEXTURE_SELECT2 "data\\TEXTURE\\Select.png"//�X�N���[���^�Z���N�g
#define TEXTURE_RESULT "data\\TEXTURE\\field002.jpg"//���U���g�w�i
#define TEXTURE_SELECTRESTART "data\\TEXTURE\\Restart.png"//���X�^�[�g
#define TEXTURE_PLAYER1 "data\\TEXTURE\\yuusiya.png" // �v���C���[
#define TEXTURE_PLAYER2 "data\\TEXTURE\\mahoutukai.png" // �v���C���[
#define TEXTURE_PLAYER3 "data\\TEXTURE\\riyoukisi.png" // �v���C���[
#define TEXTURE_PLAYER4 "data\\TEXTURE\\souryo.png" // �v���C���[
#define TEXTURE_NAME_BULLET "data\\TEXTURE\\bullet000.png" // �e
#define TEXTURE_NAME_FIELD "data\\TEXTURE\\field002.jpg"//�Q�[���t�B�[���h
#define TEXTURE_NAME_EXP "data\\TEXTURE\\explosion001.png"//����
#define TEXTURE_NAME_ENE "data\\TEXTURE\\gi.jpg"//�G
#define TEXTURE_NAME_ENE2 "data\\TEXTURE\\anko.png"//�G
#define TEXTURE_NAME_ENE3 "data\\TEXTURE\\anko2.png"//�G
#define TEXTURE_NAME_ENE4 "data\\TEXTURE\\cat.png"//�G
#define TEXTURE_SCORE "data\\TEXTURE\\number000.png"//�X�R�A
#define TEXTURE_SELECTBACK "data\\TEXTURE\\Back to Title.png"//�^�C�g���ɖ߂�
#define TEXTURE_SELECTRETRY "data\\TEXTURE\\Retry.png"//���g���C
#define TEXTURE_SELECTEXIT "data\\TEXTURE\\Exit.png"//�I��
#define TEXTURE_SCORENUMBER "data\\TEXTURE\\Number.png"//���ʕ\��
#define TEXTURE_EFFECT "data\\TEXTURE\\effect002.png"//�G�t�F�N�g
#define TEXTURE_ITEM "data\\TEXTURE\\item.png"//�A�C�e��
#define TEXTURE_NAME_MARK "data\\TEXTURE\\Mark001.png"//�}�[�N
#define TEXTURE_NAME_BOSS "data\\TEXTURE\\kerberos.png"//�G
#define TEXTURE_NAME_BOSS2 "data\\TEXTURE\\dragon.png"//�G
#define TEXTURE_NAME_BOSS3 "data\\TEXTURE\\maou.png"//�G
#define TEXTURE_NAME_BOSS4 "data\\TEXTURE\\zeus.png"//�G

// �傫��
#define SCREEN_WIDTH (1280) // ��
#define SCREEN_HEIGHT (720) // ����

// ���_
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define VT_MAX (4)//���_��
#define UV_DEF (1.0f) // �e�N�X�`���̃f�t�H���g�̃T�C�Y

// �R���g���[���[
#define	ELE_CON "GP30X-Dinput"//�w�Z�x���R���g���[��
#define PS_CON "Wireless Controller"//�v���X�e4
#define NIN_CON "Pro Controller"//�v���R��

//�f�o�b�O�p
#define FPS_TIME (500)
#define FONT_SIZE (20)

// ���_���̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos; // ���W
	float rhw; // ���W�ϊ��W��(1.0f)
	D3DCOLOR col; // �J���[
	D3DXVECTOR2 tex; // �e�N�X�`�����W
} VERTEX_2D;

// ��ʂ̎��
typedef enum
{
	MODE_TITLE = 0,//�^�C�g��
	MODE_DEMO,//�f��
	MODE_TUTO,//�`���[�g���A��
	MODE_PLAYSELECT,//�v���C�Z���N�g
	MODE_GAME,//�Q�[��
	MODE_VS,//�o�[�T�X
	MODE_RESULT,//���U���g
	MODE_RANK,//�����L���O
	MODE_OPTION,//�I�v�V����
	MODE_MAX//���[�h��
} MODE;

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // �E�C���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // ����������
void Uninit(void); // �I������
void Update(void); // �X�V����
void Draw(void); // �`�揈��
LPDIRECT3DDEVICE9 GetDevice(void); // �f�o�C�X�̎擾
void SetMode(MODE mode);//���[�h�ݒ�
MODE GetMode(void);//���[�h�擾
HWND GethWnd(void);//�n���h���擾
HINSTANCE GethInstanse(void);//�C���X�^���X�擾

void ConfineCursorToWindow(HWND hwnd);//�J�[�\������
void ReleaseCursor(void);//�J�[�\���������

void ToggleFullScreen(HWND hWnd);//�t���X�N���[��
void ResetDevice(void);//�f�o�C�X�č\�z

//�f�o�b�O�p
void DrawDebug(int nData, float *fData, char *cData);

#endif // _MAIN_H_