//---------------------------------------
//
//アイテム表示処理の定義・宣言[item.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ITEM_H_
#define _ITEM_H_

#include"main.h"

#define MAX_ITEM (128)//最大数
#define ITEM_SIZE (50)
#define ITEM_LIFE (1200)
#define SPOITEM_TIME (100)
#define SPLIT_ITEMSP (10)
#define WEY_NUM (20)
#define ALL_NUM (15)
#define SEARCH_NUM (10)
#define BOM_NUM (1)
#define SP_NUM (1)


//アイテムタイプの列挙型定義
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

void InitItem(void);//エフェクトの初期化処理
void UninitItem(void);//エフェクトの終了処理
void UpdateItem(void);//エフェクトの更新処理
void DrawItem(void);//エフェクトの描画処理
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool bSenter,TYPE Type, int nNumber);//エフェクト設定

#endif _ITEM_H_
