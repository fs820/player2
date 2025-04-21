//---------------------------------------
//
//�A�C�e���\�������̒�`�E�錾[item.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ITEM_H_
#define _ITEM_H_

#include"main.h"

#define MAX_ITEM (128)//�ő吔
#define ITEM_SIZE (50)
#define ITEM_LIFE (1200)
#define SPOITEM_TIME (100)
#define SPLIT_ITEMSP (10)
#define WEY_NUM (20)
#define ALL_NUM (15)
#define SEARCH_NUM (10)
#define BOM_NUM (1)
#define SP_NUM (1)


//�A�C�e���^�C�v�̗񋓌^��`
typedef enum
{
	TYPE_NORMAL = 0,
	TYPE_3WEY,
	TYPE_ALL,
	TYPE_SEARCH,
	TYPE_BOM,
	TYPE_SP,
	TYPE_MAX
}TYPE;

void InitItem(void);//�G�t�F�N�g�̏���������
void UninitItem(void);//�G�t�F�N�g�̏I������
void UpdateItem(void);//�G�t�F�N�g�̍X�V����
void DrawItem(void);//�G�t�F�N�g�̕`�揈��
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool bSenter,TYPE Type, int nNumber);//�G�t�F�N�g�ݒ�

#endif _ITEM_H_
