//----------------------------------------
//
//プレイヤー表示処理[player3.cpp]
//Author fuma sato
//
//----------------------------------------

#include"player2.h"
#include"player3.h"
#include"player4.h"
#include"enemy.h"
#include"boss.h"
#include"input.h"
#include"back.h"
#include"bullet.h"
#include"explosion.h"
#include"exef.h"
#include"score.h"
#include"p3back.h"
#include"vs.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer3;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer3 = NULL;//バッファのポインタ
Player g_Player3;//プレイヤー情報
D3DXVECTOR3 g_moveBullet3;//弾発射向き・スピード管理
bool g_bOber3 = false;

//----------------------
//プレイヤーの初期化処理
//----------------------
void InitPlayer3(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * POS_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer3,
		NULL
	);

	g_Player3.nCounterAnim = 0;
	g_Player3.nPatternAnim = 0;
	g_Player3.pos[SENTER] = D3DXVECTOR3(PLAYER_WIDTH, SCREEN_HEIGHT - SCREEN_HEIGHT / 3, 0);//初期位置
	if (GetMode() == MODE_VS)
	{
		g_Player3.pos[LEFTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player3.pos[RIGHTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player3.pos[LEFTDOWN] = D3DXVECTOR3((SCREEN_WIDTH * SPLIT_INA_VS) * 0.5f, SCREEN_HEIGHT - ((SCREEN_HEIGHT * SPLIT_INA_VS) * 0.5f), 0);//初期位置
		g_Player3.pos[RIGHTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
	}
	else
	{
		g_Player3.pos[LEFTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player3.pos[RIGHTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player3.pos[LEFTDOWN] = D3DXVECTOR3((SCREEN_WIDTH * SPLIT_INA) * 0.5f, SCREEN_HEIGHT - ((SCREEN_HEIGHT * SPLIT_INA) * 0.5f), 0);//初期位置
		g_Player3.pos[RIGHTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
	}
	g_Player3.Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
	g_Player3.move = D3DXVECTOR3(PLAYER_SPEED_DEF, PLAYER_SPEED_DEF, PLAYER_SPEED_DEF);//プレイヤースピード
	g_Player3.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
	g_Player3.rotDef = g_Player3.rot;//初期保存
	g_Player3.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//対角線の長さ
	g_Player3.fLengthDef = g_Player3.fLength;//初期保存
	g_Player3.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//角度
	g_Player3.BulletType = TYPE_NORMAL;
	g_Player3.BulletCount = 0;
	g_Player3.nRema = 3;
	g_Player3.nLife = 100;
	g_Player3.state = PLAYERSTATE_NORMAL;
	g_Player3.nCounterState = 0;
	for (int i = 0; i < POS_MAX; i++)
	{
		g_Player3.bDisp[i] = true;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_PLAYER3,
		&g_pTexturePlayer3
	);

	g_pVtxBuffPlayer3->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	//座標設定
	pVtx[0].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[0].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[1].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength;
	pVtx[2].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength;
	pVtx[3].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);

	for (int i = 1; i < POS_MAX; i++)
	{
		pVtx += VT_MAX;

		if (GetMode() == MODE_VS)
		{
			//座標設定
			pVtx[0].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[3].pos.z = 0.0f;

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);
		}
		else
		{
			//座標設定
			pVtx[0].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[3].pos.z = 0.0f;

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);
		}
	}

	g_pVtxBuffPlayer3->Unlock();//プレイヤーバッファのアンロック
}

//-------------------
//プレイヤー終了処理
//-------------------
void UninitPlayer3(void)
{
	SetVibrate(0.0f, CONTROLLER_3);
	//テクスチャの破棄
	if (g_pTexturePlayer3 != NULL)
	{
		g_pTexturePlayer3->Release();
		g_pTexturePlayer3 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer3 != NULL)
	{
		g_pVtxBuffPlayer3->Release();
		g_pVtxBuffPlayer3 = NULL;
	}
}

//-------------------
//プレイヤー更新処理
//-------------------
void UpdatePlayer3(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	float* pStick;

	//過去の位置の保存
	g_Player3.Oldpos = g_Player3.pos[SENTER];

	//移動処理

	//X
	if (IsXInputControllerConnected(CONTROLLER_3))
	{
		//左スティック処理
		pStick = GetJoyStick(STICK_LEFT, CONTROLLER_3);
		g_Player3.move.x += *pStick * PLAYER_SPEED_X;
		g_Player3.move.y += -*(pStick + 1) * PLAYER_SPEED_Y;

		//右スティック処理
		pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_3);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player3.rot.z = atan2f(*(pStick + 1), *pStick);
		}
	}
	//d
	else if (IsDirectInputControllerConnected(CONTROLLER_3))
	{

		//左スティック処理
		pStick = GetdJoyStick(STICK_LEFT, CONTROLLER_3);
		g_Player3.move.x += *pStick * PLAYER_SPEED_X;
		g_Player3.move.y += *(pStick + 1) * PLAYER_SPEED_Y;

		//右スティック処理
		pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_3);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player3.rot.z = -atan2f(*(pStick + 1), *pStick);
		}
	}

	if (GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_3) == true)//Aキー
	{
		if (GetJoykeyPress(JOYKEY_UP, CONTROLLER_3) == true)//Wキー
		{//左上
			g_Player3.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_3) == true)//Sキー
		{//左下
			g_Player3.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//左
			g_Player3.move.x += -PLAYER_SPEED_X;
		}
	}
	else if (GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_3) == true)//Dキー
	{
		if (GetJoykeyPress(JOYKEY_UP, CONTROLLER_3) == true)//Wキー
		{//右上
			g_Player3.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_3) == true)//Sキー
		{//右下
			g_Player3.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//右
			g_Player3.move.x += PLAYER_SPEED_X;
		}
	}
	else if (GetJoykeyPress(JOYKEY_UP, CONTROLLER_3) == true)//Wキー
	{//上
		g_Player3.move.y += -PLAYER_SPEED_Y;
	}
	else if (GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_3) == true)//Sキー
	{//下
		g_Player3.move.y += PLAYER_SPEED_Y;
	}

	if (GetJoykeyPress(JOYKEY_B, CONTROLLER_3) == true)
	{
		g_Player3.rot.z += PLAYER_ROT_SPEED;
	}

	if (GetJoykeyPress(JOYKEY_Y, CONTROLLER_3) == true)
	{
		g_Player3.rot.z += -PLAYER_ROT_SPEED;
	}

	if (GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_3) == true)
	{
		g_Player3.rot = g_Player3.rotDef;
	}

	if (GetJoykeyPress(JOYKEY_A, CONTROLLER_3) == true)
	{
		if (g_Player3.fLength >= 0.0f && g_Player3.fLength <= PLAYER_SIZE_MAX)
		{
			g_Player3.fLength += PLAYER_SIZE_SPEED;
		}
	}

	if (GetJoykeyPress(JOYKEY_X, CONTROLLER_3) == true)
	{
		if (g_Player3.fLength >= PLAYER_SIZE_MIN)
		{
			g_Player3.fLength += -PLAYER_SIZE_SPEED;
		}
	}

	if (GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_3) == true)
	{
		g_Player3.fLength = g_Player3.fLengthDef;
	}

	if (g_Player3.state != PLAYERSTATE_APPEAR && g_Player3.state != PLAYERSTATE_DIE)
	{
		if (GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_3) == true)
		{
			switch (g_Player3.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//弾の設定
				SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);
				break;
			case TYPE_3WEY:
				//弾の設定
				Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//弾の設定
				SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//弾の設定
				SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
		else if (GetJoykeyRepeat(JOYKEY_RB, CONTROLLER_3) == true)
		{
			switch (g_Player3.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//弾の設定
				SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);
				break;
			case TYPE_3WEY:
				//弾の設定
				Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_ALL:
				//弾の設定
				SetAllBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SEARCH:
				//弾の設定
				SetSearchBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//弾の設定
				SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//弾の設定
				SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

				if (g_Player3.BulletCount <= 0)
				{
					g_Player3.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
	}

	if (IsDirectInputControllerConnected(CONTROLLER_3))
	{
		if (GetdJoyPov(POV_UP, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.y += -PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHTUP, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.x += PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_RIGHTDOWN, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.y += PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFTDOWN, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.x += -PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_LEFTUP, 0, CONTROLLER_3) == true)
		{
			g_Player3.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player3.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
	}

	if (!strcmp(ControllerName(CONTROLLER_3), ELE_CON))
	{
		if (GetdJoykeyPress(ELEKEY_B, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(ELEKEY_Y, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(ELEKEY_L3, CONTROLLER_3) == true)
		{
			g_Player3.rot = g_Player3.rotDef;
		}

		if (GetdJoykeyPress(ELEKEY_A, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= 0.0f && g_Player3.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player3.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(ELEKEY_X, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player3.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(ELEKEY_R3, CONTROLLER_3) == true)
		{
			g_Player3.fLength = g_Player3.fLengthDef;
		}

		if (g_Player3.state != PLAYERSTATE_APPEAR && g_Player3.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(ELEKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(ELEKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_3), PS_CON))
	{
		if (GetdJoykeyPress(PSKEY_CR, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(PSKEY_SQ, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(PSKEY_L3, CONTROLLER_3) == true)
		{
			g_Player3.rot = g_Player3.rotDef;
		}

		if (GetdJoykeyPress(PSKEY_CI, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= 0.0f && g_Player3.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player3.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(PSKEY_TRA, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player3.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(PSKEY_R3, CONTROLLER_3) == true)
		{
			g_Player3.fLength = g_Player3.fLengthDef;
		}

		if (g_Player3.state != PLAYERSTATE_APPEAR && g_Player3.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(PSKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(PSKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_3), NIN_CON))
	{
		if (GetdJoykeyPress(NINKEY_A, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(NINKEY_X, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(NINKEY_L3, CONTROLLER_3) == true)
		{
			g_Player3.rot = g_Player3.rotDef;
		}

		if (GetdJoykeyPress(NINKEY_B, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= 0.0f && g_Player3.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player3.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(NINKEY_Y, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player3.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(NINKEY_R3, CONTROLLER_3) == true)
		{
			g_Player3.fLength = g_Player3.fLengthDef;
		}

		if (g_Player3.state != PLAYERSTATE_APPEAR && g_Player3.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(NINKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(NINKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}
	else if (!IsXInputControllerConnected(CONTROLLER_3) && IsDirectInputControllerConnected(CONTROLLER_3))
	{
		if (GetdJoykeyPress(DKEY_B, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(DKEY_Y, CONTROLLER_3) == true)
		{
			g_Player3.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(DKEY_L3, CONTROLLER_3) == true)
		{
			g_Player3.rot = g_Player3.rotDef;
		}

		if (GetdJoykeyPress(DKEY_A, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= 0.0f && g_Player3.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player3.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(DKEY_X, CONTROLLER_3) == true)
		{
			if (g_Player3.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player3.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(DKEY_R3, CONTROLLER_3) == true)
		{
			g_Player3.fLength = g_Player3.fLengthDef;
		}

		if (g_Player3.state != PLAYERSTATE_APPEAR && g_Player3.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(DKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, 1);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(DKEY_RB, CONTROLLER_3) == true)
			{
				switch (g_Player3.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet3.x = sinf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet3.y = cosf(g_Player3.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player3.pos[SENTER], g_moveBullet3, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player3.pos[SENTER], g_Player3.rot.z, g_Player3.fLength, BULLET_LIFE, BULLETTYPE_PLAYER3, BULLET_INTER);

					if (g_Player3.BulletCount <= 0)
					{
						g_Player3.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}

	if (g_Player3.state != PLAYERSTATE_APPEAR && g_Player3.state != PLAYERSTATE_DIE)
	{
		Enemy* pEnemy;
		Boss* pBoss;
		Player* pPlayer, * pPlayer2, * pPlayer4;
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;

		pEnemy = GetEnemy();
		pBoss = GetBoss();
		pPlayer = GetPlayer();
		pPlayer2 = GetPlayer2();
		pPlayer4 = GetPlayer4();
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{//敵が使用されている
				switch (pEnemy->nType)
				{
				case 0:
					EnemyWidth = ENEMY_WIDTH;
					EnemyHeight = ENEMY_HEIGHT;
					break;
				case 1:
					EnemyWidth = ENEMY_WIDTH2;
					EnemyHeight = ENEMY_HEIGHT2;
					break;
				case 2:
					EnemyWidth = ENEMY_WIDTH3;
					EnemyHeight = ENEMY_HEIGHT3;
					break;
				case 3:
					EnemyWidth = ENEMY_WIDTH4;
					EnemyHeight = ENEMY_HEIGHT4;
					break;
				}
				if (pEnemy->pos.x - EnemyWidth / 2 <= g_Player3.pos[SENTER].x + g_Player3.fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_Player3.pos[SENTER].x - g_Player3.fLength && pEnemy->pos.y - EnemyHeight / 2 <= g_Player3.pos[SENTER].y + g_Player3.fLength && pEnemy->pos.y + EnemyHeight / 2 >= g_Player3.pos[SENTER].y - g_Player3.fLength)
				{
					if (g_Player3.pos[SENTER].x <= pEnemy->pos.x - EnemyWidth / 2)
					{
						g_Player3.pos[SENTER].x = (pEnemy->pos.x - EnemyWidth / 2) - g_Player3.fLength;
					}
					else if (g_Player3.pos[SENTER].x >= pEnemy->pos.x + EnemyWidth / 2)
					{
						g_Player3.pos[SENTER].x = (pEnemy->pos.x + EnemyWidth / 2) + g_Player3.fLength;
					}
					else if (g_Player3.pos[SENTER].y <= pEnemy->pos.y - EnemyHeight / 2)
					{
						g_Player3.pos[SENTER].y = (pEnemy->pos.y - EnemyHeight / 2) - g_Player3.fLength;
					}
					else if (g_Player3.pos[SENTER].y >= pEnemy->pos.y + EnemyHeight / 2)
					{
						g_Player3.pos[SENTER].y = (pEnemy->pos.y + EnemyHeight / 2) + g_Player3.fLength;
					}
				}
			}
		}

		for (int i = 0; i < MAX_BOSS; i++, pBoss++)
		{
			if (pBoss->bUse == true)
			{//敵が使用されている
				switch (pBoss->nType)
				{
				case 0:
					EnemyWidth = BOSS_WIDTH;
					EnemyHeight = BOSS_HEIGHT;
					break;
				case 1:
					EnemyWidth = BOSS_WIDTH2;
					EnemyHeight = BOSS_HEIGHT2;
					break;
				case 2:
					EnemyWidth = BOSS_WIDTH3;
					EnemyHeight = BOSS_HEIGHT3;
					break;
				case 3:
					EnemyWidth = BOSS_WIDTH4;
					EnemyHeight = BOSS_HEIGHT4;
					break;
				}
				if (pBoss->pos.x - EnemyWidth / 2 <= g_Player3.pos[SENTER].x + g_Player3.fLength && pBoss->pos.x + EnemyWidth / 2 >= g_Player3.pos[SENTER].x - g_Player3.fLength && pBoss->pos.y - EnemyHeight / 2 <= g_Player3.pos[SENTER].y + g_Player3.fLength && pBoss->pos.y + EnemyHeight / 2 >= g_Player3.pos[SENTER].y - g_Player3.fLength)
				{
					if (g_Player3.pos[SENTER].x <= pBoss->pos.x - EnemyWidth / 2)
					{
						g_Player3.pos[SENTER].x = (pBoss->pos.x - EnemyWidth / 2) - g_Player3.fLength;
					}
					else if (g_Player3.pos[SENTER].x >= pBoss->pos.x + EnemyWidth / 2)
					{
						g_Player3.pos[SENTER].x = (pBoss->pos.x + EnemyWidth / 2) + g_Player3.fLength;
					}
					else if (g_Player3.pos[SENTER].y <= pBoss->pos.y - EnemyHeight / 2)
					{
						g_Player3.pos[SENTER].y = (pBoss->pos.y - EnemyHeight / 2) - g_Player3.fLength;
					}
					else if (g_Player3.pos[SENTER].y >= pBoss->pos.y + EnemyHeight / 2)
					{
						g_Player3.pos[SENTER].y = (pBoss->pos.y + EnemyHeight / 2) + g_Player3.fLength;
					}
				}
			}
		}


		if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
		{
			if (pPlayer->pos[SENTER].x - pPlayer->fLength <= g_Player3.pos[SENTER].x + g_Player3.fLength && pPlayer->pos[SENTER].x + pPlayer->fLength >= g_Player3.pos[SENTER].x - g_Player3.fLength && pPlayer->pos[SENTER].y - pPlayer->fLength <= g_Player3.pos[SENTER].y + g_Player3.fLength && pPlayer->pos[SENTER].y + pPlayer->fLength >= g_Player3.pos[SENTER].y - g_Player3.fLength)
			{
				if (g_Player3.pos[SENTER].x <= pPlayer->pos[SENTER].x - pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_3) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer->pos[SENTER].x = (g_Player3.pos[SENTER].x + g_Player3.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].x = (pPlayer->pos[SENTER].x - pPlayer->fLength) - g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer->pos[SENTER].x = (g_Player3.pos[SENTER].x + g_Player3.fLength) + pPlayer->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].x = (pPlayer->pos[SENTER].x - pPlayer->fLength) - g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].x >= pPlayer->pos[SENTER].x + pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_3) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer->pos[SENTER].x = (g_Player3.pos[SENTER].x - g_Player3.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].x = (pPlayer->pos[SENTER].x + pPlayer->fLength) + g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer->pos[SENTER].x = (g_Player3.pos[SENTER].x - g_Player3.fLength) - pPlayer->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].x = (pPlayer->pos[SENTER].x + pPlayer->fLength) + g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].y <= pPlayer->pos[SENTER].y - pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_3) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer->pos[SENTER].y = (g_Player3.pos[SENTER].y + g_Player3.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].y = (pPlayer->pos[SENTER].y - pPlayer->fLength) - g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer->pos[SENTER].y = (g_Player3.pos[SENTER].y + g_Player3.fLength) + pPlayer->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].y = (pPlayer->pos[SENTER].y - pPlayer->fLength) - g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].y >= pPlayer->pos[SENTER].y + pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_3) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer->pos[SENTER].y = (g_Player3.pos[SENTER].y - g_Player3.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].y = (pPlayer->pos[SENTER].y + pPlayer->fLength) + g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer->pos[SENTER].y = (g_Player3.pos[SENTER].y - g_Player3.fLength) - pPlayer->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].y = (pPlayer->pos[SENTER].y + pPlayer->fLength) + g_Player3.fLength;
						}
					}
				}

				//pPlayer->move = g_Player3.move;
				//g_Player3.move = pPlayer->move;
			}
		}
		if (pPlayer2->state != PLAYERSTATE_APPEAR && pPlayer2->state != PLAYERSTATE_DIE)
		{
			if (pPlayer2->pos[SENTER].x - pPlayer2->fLength <= g_Player3.pos[SENTER].x + g_Player3.fLength && pPlayer2->pos[SENTER].x + pPlayer2->fLength >= g_Player3.pos[SENTER].x - g_Player3.fLength && pPlayer2->pos[SENTER].y - pPlayer2->fLength <= g_Player3.pos[SENTER].y + g_Player3.fLength && pPlayer2->pos[SENTER].y + pPlayer2->fLength >= g_Player3.pos[SENTER].y - g_Player3.fLength)
			{
				if (g_Player3.pos[SENTER].x <= pPlayer2->pos[SENTER].x - pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_3) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer2->pos[SENTER].x = (g_Player3.pos[SENTER].x + g_Player3.fLength) + pPlayer2->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].x = (pPlayer2->pos[SENTER].x - pPlayer2->fLength) - g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer2->pos[SENTER].x = (g_Player3.pos[SENTER].x + g_Player3.fLength) + pPlayer2->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].x = (pPlayer2->pos[SENTER].x - pPlayer2->fLength) - g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].x >= pPlayer2->pos[SENTER].x + pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_3) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer2->pos[SENTER].x = (g_Player3.pos[SENTER].x - g_Player3.fLength) - pPlayer2->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].x = (pPlayer2->pos[SENTER].x + pPlayer2->fLength) + g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer2->pos[SENTER].x = (g_Player3.pos[SENTER].x - g_Player3.fLength) - pPlayer2->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].x = (pPlayer2->pos[SENTER].x + pPlayer2->fLength) + g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].y <= pPlayer2->pos[SENTER].y - pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_3) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer2->pos[SENTER].y = (g_Player3.pos[SENTER].y + g_Player3.fLength) + pPlayer2->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].y = (pPlayer2->pos[SENTER].y - pPlayer2->fLength) - g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer2->pos[SENTER].y = (g_Player3.pos[SENTER].y + g_Player3.fLength) + pPlayer2->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].y = (pPlayer2->pos[SENTER].y - pPlayer2->fLength) - g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].y >= pPlayer2->pos[SENTER].y + pPlayer2->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_3) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer2->pos[SENTER].y = (g_Player3.pos[SENTER].y - g_Player3.fLength) - pPlayer2->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].y = (pPlayer2->pos[SENTER].y + pPlayer2->fLength) + g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer2->pos[SENTER].y = (g_Player3.pos[SENTER].y - g_Player3.fLength) - pPlayer2->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].y = (pPlayer2->pos[SENTER].y + pPlayer2->fLength) + g_Player3.fLength;
						}
					}
				}

				//pPlayer2->move = g_Player3.move;
				//g_Player3.move = pPlayer2->move;
			}
		}
		if (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE)
		{
			if (pPlayer4->pos[SENTER].x - pPlayer4->fLength <= g_Player3.pos[SENTER].x + g_Player3.fLength && pPlayer4->pos[SENTER].x + pPlayer4->fLength >= g_Player3.pos[SENTER].x - g_Player3.fLength && pPlayer4->pos[SENTER].y - pPlayer4->fLength <= g_Player3.pos[SENTER].y + g_Player3.fLength && pPlayer4->pos[SENTER].y + pPlayer4->fLength >= g_Player3.pos[SENTER].y - g_Player3.fLength)
			{
				if (g_Player3.pos[SENTER].x <= pPlayer4->pos[SENTER].x - pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_3) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer4->pos[SENTER].x = (g_Player3.pos[SENTER].x + g_Player3.fLength) + pPlayer4->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].x = (pPlayer4->pos[SENTER].x - pPlayer4->fLength) - g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer4->pos[SENTER].x = (g_Player3.pos[SENTER].x + g_Player3.fLength) + pPlayer4->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].x = (pPlayer4->pos[SENTER].x - pPlayer4->fLength) - g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].x >= pPlayer4->pos[SENTER].x + pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_3) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer4->pos[SENTER].x = (g_Player3.pos[SENTER].x - g_Player3.fLength) - pPlayer4->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].x = (pPlayer4->pos[SENTER].x + pPlayer4->fLength) + g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer4->pos[SENTER].x = (g_Player3.pos[SENTER].x - g_Player3.fLength) - pPlayer4->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].x = (pPlayer4->pos[SENTER].x + pPlayer4->fLength) + g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].y <= pPlayer4->pos[SENTER].y - pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_3) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer4->pos[SENTER].y = (g_Player3.pos[SENTER].y + g_Player3.fLength) + pPlayer4->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].y = (pPlayer4->pos[SENTER].y - pPlayer4->fLength) - g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer4->pos[SENTER].y = (g_Player3.pos[SENTER].y + g_Player3.fLength) + pPlayer4->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].y = (pPlayer4->pos[SENTER].y - pPlayer4->fLength) - g_Player3.fLength;
						}
					}
				}
				else if (g_Player3.pos[SENTER].y >= pPlayer4->pos[SENTER].y + pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_3) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_3))
					{
						pPlayer4->pos[SENTER].y = (g_Player3.pos[SENTER].y - g_Player3.fLength) - pPlayer4->fLength;
					}
					else
					{
						g_Player3.pos[SENTER].y = (pPlayer4->pos[SENTER].y + pPlayer4->fLength) + g_Player3.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_3), IsDirectInputControllerConnected(CONTROLLER_3))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_3) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_3))
						{
							pPlayer4->pos[SENTER].y = (g_Player3.pos[SENTER].y - g_Player3.fLength) - pPlayer4->fLength;
						}
						else
						{
							g_Player3.pos[SENTER].y = (pPlayer4->pos[SENTER].y + pPlayer4->fLength) + g_Player3.fLength;
						}
					}
				}

				//pPlayer4->move = g_Player3.move;
				//g_Player3.move = pPlayer4->move;
			}
		}
	}

	//位置の更新
	Boss* pBoss;
	pBoss = GetBoss();
	for (int i = 0; i < MAX_BOSS; i++, pBoss++)
	{
		if (pBoss->bUse == true)
		{
			g_Player3.pos[SENTER].x += (g_Player3.move.x - pBoss->move.x * PLAYER_FIELD_INA);
			g_Player3.pos[SENTER].y += (g_Player3.move.y - pBoss->move.y * PLAYER_FIELD_INA);
			break;
		}
		else if (i == MAX_BOSS - 1)
		{
			g_Player3.pos[SENTER].x += g_Player3.move.x;
			g_Player3.pos[SENTER].y += g_Player3.move.y;
		}
	}

	if (GetMode() == MODE_VS)
	{
		//離れすぎ防止
		Player* pPlayer;
		pPlayer = GetPlayer();
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player3.pos[SENTER].x - pPlayer->pos[SENTER].x;
		Ylong = g_Player3.pos[SENTER].y - pPlayer->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player3.pos[SENTER].x = pPlayer->pos[SENTER].x + Xlong * LongIna;
			g_Player3.pos[SENTER].y = pPlayer->pos[SENTER].y + Ylong * LongIna;
		}

		//離れすぎ防止
		Player* pPlayer2;
		pPlayer2 = GetPlayer2();
		Xlong = g_Player3.pos[SENTER].x - pPlayer2->pos[SENTER].x;
		Ylong = g_Player3.pos[SENTER].y - pPlayer2->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player3.pos[SENTER].x = pPlayer2->pos[SENTER].x + Xlong * LongIna;
			g_Player3.pos[SENTER].y = pPlayer2->pos[SENTER].y + Ylong * LongIna;
		}

		//離れすぎ防止
		Player* pPlayer4;
		pPlayer4 = GetPlayer4();
		Xlong = g_Player3.pos[SENTER].x - pPlayer4->pos[SENTER].x;
		Ylong = g_Player3.pos[SENTER].y - pPlayer4->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player3.pos[SENTER].x = pPlayer4->pos[SENTER].x + Xlong * LongIna;
			g_Player3.pos[SENTER].y = pPlayer4->pos[SENTER].y + Ylong * LongIna;
		}
	}
	else
	{
		//離れすぎ防止
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player3.pos[SENTER].x - pBoss->pos.x;
		Ylong = g_Player3.pos[SENTER].y - pBoss->pos.y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player3.pos[SENTER].x = pBoss->pos.x + Xlong * LongIna;
			g_Player3.pos[SENTER].y = pBoss->pos.y + Ylong * LongIna;
		}
	}

	if (bOberP1())
	{
		Player* pPlayer;
		pPlayer = GetPlayer();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer->pos[SENTER].x - g_Player3.pos[SENTER].x;
		Ylong = pPlayer->pos[SENTER].y - g_Player3.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player3.bDisp[LEFTUP] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player3.pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA_VS, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player3.pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player3.bDisp[LEFTUP] = false;
		}
	}
	if (bOberP2())
	{
		Player* pPlayer2;
		pPlayer2 = GetPlayer2();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer2->pos[SENTER].x - g_Player3.pos[SENTER].x;
		Ylong = pPlayer2->pos[SENTER].y - g_Player3.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player3.bDisp[RIGHTUP] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player3.pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA_VS, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player3.pos[RIGHTUP] = D3DXVECTOR3(pPlayer2->pos[RIGHTUP].x - Xlong * SPLIT_INA, pPlayer2->pos[RIGHTUP].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player3.bDisp[RIGHTUP] = false;
		}
	}
	if (bOberP4())
	{
		Player* pPlayer4;
		pPlayer4 = GetPlayer4();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer4->pos[SENTER].x - g_Player3.pos[SENTER].x;
		Ylong = pPlayer4->pos[SENTER].y - g_Player3.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player3.bDisp[RIGHTDOWN] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player3.pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player3.pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player3.bDisp[RIGHTDOWN] = false;
		}
	}

	//移動量の更新(減衰)
	g_Player3.move.x += (PLAYER_SPEED_DEF - g_Player3.move.x) * PLAYER_INA;
	g_Player3.move.y += (PLAYER_SPEED_DEF - g_Player3.move.y) * PLAYER_INA;

	g_pVtxBuffPlayer3->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		//座標設定
	pVtx[0].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[0].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[1].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength;
	pVtx[2].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player3.pos[SENTER].x + sinf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength;
	pVtx[3].pos.y = g_Player3.pos[SENTER].y + cosf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength;
	pVtx[3].pos.z = 0.0f;


	pVtx += VT_MAX;

	for (int i = 1; i < POS_MAX; i++)
	{
		if (GetMode() == MODE_VS)
		{
			//座標設定
			pVtx[0].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA_VS;
			pVtx[3].pos.z = 0.0f;
		}
		else
		{
			//座標設定
			pVtx[0].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI - g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - (D3DX_PI + g_Player3.fAngle)) * g_Player3.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z - g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player3.pos[i].x + sinf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player3.pos[i].y + cosf(g_Player3.rot.z + g_Player3.fAngle) * g_Player3.fLength * SPLIT_INA;
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += VT_MAX;
	}

	g_pVtxBuffPlayer3->Unlock();//プレイヤーバッファのアンロック

	g_Player3.nCounterAnim++;
	if ((g_Player3.nCounterAnim % UP_TIME) == 0)//更新期間
	{
		g_Player3.nPatternAnim = (g_Player3.nPatternAnim + 1) % (U_MAX * V_MAX);

		g_pVtxBuffPlayer3->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		for (int i = 0; i < POS_MAX; i++)
		{
			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player3.nPatternAnim, UV_DEF / V_MAX * (g_Player3.nPatternAnim / U_MAX));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player3.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player3.nPatternAnim / U_MAX));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player3.nPatternAnim, UV_DEF / V_MAX * (g_Player3.nPatternAnim / U_MAX) + UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player3.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player3.nPatternAnim / U_MAX) + UV_DEF / V_MAX);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer3->Unlock();//プレイヤーバッファのアンロック
	}

	if (g_Player3.state == PLAYERSTATE_DIE)
	{
		g_bOber3 = false;
	}
	else if (g_Player3.pos[SENTER].x + g_Player3.fLength <= 0 || g_Player3.pos[SENTER].x - g_Player3.fLength >= SCREEN_WIDTH || g_Player3.pos[SENTER].y + g_Player3.fLength <= 0 || g_Player3.pos[SENTER].y - g_Player3.fLength >= SCREEN_HEIGHT)//画面右端
	{//画面外
		g_bOber3 = true;
	}
	else
	{//画面内
		g_bOber3 = false;
	}

	switch (g_Player3.state)
	{
	case PLAYERSTATE_NORMAL:
		SetVibrate(0.0f, CONTROLLER_1);

		g_pVtxBuffPlayer3->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		for (int i = 0; i < POS_MAX; i++)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer3->Unlock();//プレイヤーバッファのアンロック
		break;
	case PLAYERSTATE_APPEAR:
		g_Player3.nCounterState++;
		if (g_Player3.nCounterState >= 60)
		{
			g_Player3.state = PLAYERSTATE_NORMAL;
			g_Player3.bDisp[SENTER] = true;
		}
		else if (g_Player3.nCounterState % 5 == 0)
		{
			g_Player3.bDisp[SENTER] = !g_Player3.bDisp[SENTER];
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player3.nCounterState--;
		if (g_Player3.nCounterState <= 0)
		{
			g_Player3.state = PLAYERSTATE_NORMAL;

			g_pVtxBuffPlayer3->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer3->Unlock();//プレイヤーバッファのアンロック
		}
		break;
	case PLAYERSTATE_DIE:
		g_Player3.bDisp[SENTER] = false;
		break;
	}

	if (GetMode() == MODE_VS)
	{
		g_bOber3 = true;
	}
}

//-------------------
//プレイヤー描画処理
//-------------------
void DrawPlayer3(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_Player3.bDisp[SENTER])
	{
		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer3, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer3);

		if (GetMode() != MODE_VS)
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,//始まりの番号
				2//ポリゴンの個数
			);
		}

		if (bOberP1() && g_Player3.bDisp[LEFTUP])
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX,//始まりの番号
				3//ポリゴンの個数
			);
		}
		if (bOberP2() && g_Player3.bDisp[RIGHTUP])
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX * 2,//始まりの番号
				2//ポリゴンの個数
			);
		}
		if (g_bOber3)
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX * 3,//始まりの番号
				2//ポリゴンの個数
			);
		}
		if (bOberP4()&&g_Player3.bDisp[RIGHTDOWN])
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX * 4,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//-------------------
//ヒット処理
//-------------------
void HitPlayer3(int nDamage)
{
	if (g_Player3.state != PLAYERSTATE_DIE && g_Player3.state != PLAYERSTATE_APPEAR)
	{
		VERTEX_2D* pVtx;

		g_Player3.nLife -= nDamage;

		if (g_Player3.nLife <= 0)
		{
			if (g_Player3.nRema <= 0)
			{
				//SetExplosion(g_Player3.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player3.fLength);
				SetExef(g_Player3.pos[SENTER], g_Player3.fLength, true);
				if (GetMode() == MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player3.pos[LEFTUP], g_Player3.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player3.pos[RIGHTUP], g_Player3.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player3.pos[LEFTDOWN], g_Player3.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player3.pos[RIGHTDOWN], g_Player3.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player3.pos[LEFTUP], g_Player3.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player3.pos[RIGHTUP], g_Player3.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player3.pos[LEFTDOWN], g_Player3.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player3.pos[RIGHTDOWN], g_Player3.fLength * SPLIT_INA, false);
					}
				}
				g_Player3.state = PLAYERSTATE_DIE;
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
			}
			else
			{
				//SetExplosion(g_Player3.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player3.fLength);
				SetExef(g_Player3.pos[SENTER], g_Player3.fLength, true);
				if (GetMode() == MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player3.pos[LEFTUP], g_Player3.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player3.pos[RIGHTUP], g_Player3.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player3.pos[LEFTDOWN], g_Player3.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player3.pos[RIGHTDOWN], g_Player3.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player3.pos[LEFTUP], g_Player3.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player3.pos[RIGHTUP], g_Player3.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player3.pos[LEFTDOWN], g_Player3.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player3.pos[RIGHTDOWN], g_Player3.fLength * SPLIT_INA, false);
					}
				}
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
				g_Player3.state = PLAYERSTATE_APPEAR;
				g_Player3.nCounterState = 0;
				g_Player3.nRema--;
				g_Player3.nLife = 100;
			}
			SetVibrate(1.0f, CONTROLLER_3);
		}
		else
		{
			g_Player3.state = PLAYERSTATE_DAMAGE;
			g_Player3.nCounterState = 5;
			if (GetMode() == MODE_GAME)
			{
				AddScore(-nDamage * SCORE_MISS);
			}

			g_pVtxBuffPlayer3->Lock(0, 0, (void**)&pVtx, 0);

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer3->Unlock();//バッファのアンロック
		}
	}
}

//-------------------------
//プレイヤーバッファの取得
//-------------------------
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer3(void)
{
	return g_pVtxBuffPlayer3;
}

//-------------------------
//プレイヤー取得
//-------------------------
Player* GetPlayer3(void)
{
	return &g_Player3;
}

//------------------
//画面外
//------------------
bool bOberP3(void)
{
	return g_bOber3;
}