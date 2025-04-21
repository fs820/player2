//----------------------------------------
//
//プレイヤー表示処理[player2.cpp]
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
#include"p2back.h"
#include"vs.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer2;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer2 = NULL;//バッファのポインタ
Player g_Player2;//プレイヤー情報
D3DXVECTOR3 g_moveBullet2;//弾発射向き・スピード管理
bool g_bOber2 = false;

//----------------------
//プレイヤーの初期化処理
//----------------------
void InitPlayer2(void)
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
		&g_pVtxBuffPlayer2,
		NULL
	);

	g_Player2.nCounterAnim = 0;
	g_Player2.nPatternAnim = 0;
	g_Player2.pos[SENTER] = D3DXVECTOR3(SCREEN_WIDTH - PLAYER_WIDTH, SCREEN_HEIGHT / 3, 0);//初期位置
	if (GetMode()==MODE_VS)
	{
		g_Player2.pos[LEFTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player2.pos[RIGHTUP] = D3DXVECTOR3(SCREEN_WIDTH - ((SCREEN_WIDTH * SPLIT_INA_VS) * 0.5f), (SCREEN_HEIGHT * SPLIT_INA_VS) * 0.5f, 0);//初期位置
		g_Player2.pos[LEFTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player2.pos[RIGHTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
	}
	else
	{
		g_Player2.pos[LEFTUP] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player2.pos[RIGHTUP] = D3DXVECTOR3(SCREEN_WIDTH - ((SCREEN_WIDTH * SPLIT_INA) * 0.5f), (SCREEN_HEIGHT * SPLIT_INA) * 0.5f, 0);//初期位置
		g_Player2.pos[LEFTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
		g_Player2.pos[RIGHTDOWN] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
	}
	g_Player2.Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
	g_Player2.move = D3DXVECTOR3(PLAYER_SPEED_DEF, PLAYER_SPEED_DEF, PLAYER_SPEED_DEF);//プレイヤースピード
	g_Player2.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);//向き
	g_Player2.rotDef = g_Player2.rot;//初期保存
	g_Player2.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//対角線の長さ
	g_Player2.fLengthDef = g_Player2.fLength;//初期保存
	g_Player2.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//角度
	g_Player2.BulletType = TYPE_NORMAL;
	g_Player2.BulletCount = 0;
	g_Player2.nRema = 3;
	g_Player2.nLife = 100;
	g_Player2.state = PLAYERSTATE_NORMAL;
	g_Player2.nCounterState = 0;
	for (int i = 0; i < POS_MAX; i++)
	{
		g_Player2.bDisp[i] = true;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_PLAYER2,
		&g_pTexturePlayer2
	);

	g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	//座標設定
	pVtx[0].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[0].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[1].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
	pVtx[2].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
	pVtx[3].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
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

	for (int i = 1; i < POS_MAX; i++)
	{
		pVtx += VT_MAX;

		if (GetMode()==MODE_VS)
		{
			//座標設定
			pVtx[0].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
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
			pVtx[0].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
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

	g_pVtxBuffPlayer2->Unlock();//プレイヤーバッファのアンロック
}

//-------------------
//プレイヤー終了処理
//-------------------
void UninitPlayer2(void)
{
	SetVibrate(0.0f, CONTROLLER_2);
	//テクスチャの破棄
	if (g_pTexturePlayer2 != NULL)
	{
		g_pTexturePlayer2->Release();
		g_pTexturePlayer2 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer2 != NULL)
	{
		g_pVtxBuffPlayer2->Release();
		g_pVtxBuffPlayer2 = NULL;
	}
}

//-------------------
//プレイヤー更新処理
//-------------------
void UpdatePlayer2(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	float* pStick;

	//過去の位置の保存
	g_Player2.Oldpos = g_Player2.pos[SENTER];

	//移動処理

	//X
	if (IsXInputControllerConnected(CONTROLLER_2))
	{
		//左スティック処理
		pStick = GetJoyStick(STICK_LEFT, CONTROLLER_2);
		g_Player2.move.x += *pStick * PLAYER_SPEED_X;
		g_Player2.move.y += -*(pStick + 1) * PLAYER_SPEED_Y;

		//右スティック処理
		pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_2);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player2.rot.z = atan2f(*(pStick + 1), *pStick);
		}
	}
	//d
	else if (IsDirectInputControllerConnected(CONTROLLER_2))
	{

		//左スティック処理
		pStick = GetdJoyStick(STICK_LEFT, CONTROLLER_2);
		g_Player2.move.x += *pStick * PLAYER_SPEED_X;
		g_Player2.move.y += *(pStick + 1) * PLAYER_SPEED_Y;

		//右スティック処理
		pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_2);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player2.rot.z = -atan2f(*(pStick + 1), *pStick);
		}
	}

	if (GetKeyboradPress(DIK_LEFT) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_2) == true)//Aキー
	{
		if (GetKeyboradPress(DIK_UP) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) == true)//Wキー
		{//左上
			g_Player2.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_DOWN) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) == true)//Sキー
		{//左下
			g_Player2.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//左
			g_Player2.move.x += -PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_RIGHT) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_2) == true)//Dキー
	{
		if (GetKeyboradPress(DIK_UP) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) == true)//Wキー
		{//右上
			g_Player2.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_DOWN) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) == true)//Sキー
		{//右下
			g_Player2.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//右
			g_Player2.move.x += PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_UP) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) == true)//Wキー
	{//上
		g_Player2.move.y += -PLAYER_SPEED_Y;
	}
	else if (GetKeyboradPress(DIK_DOWN) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) == true)//Sキー
	{//下
		g_Player2.move.y += PLAYER_SPEED_Y;
	}

	if (GetKeyboradPress(DIK_7) == true || GetJoykeyPress(JOYKEY_B, CONTROLLER_2) == true)
	{
		g_Player2.rot.z += PLAYER_ROT_SPEED;
	}

	if (GetKeyboradPress(DIK_9) == true || GetJoykeyPress(JOYKEY_Y, CONTROLLER_2) == true)
	{
		g_Player2.rot.z += -PLAYER_ROT_SPEED;
	}

	if (GetKeyboradTrigger(DIK_8) == true || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_2) == true)
	{
		g_Player2.rot = g_Player2.rotDef;
	}

	if (GetKeyboradPress(DIK_1) == true || GetJoykeyPress(JOYKEY_A, CONTROLLER_2) == true)
	{
		if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
		{
			g_Player2.fLength += PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradPress(DIK_3) == true|| GetJoykeyPress(JOYKEY_X, CONTROLLER_2) == true)
	{
		if (g_Player2.fLength >= PLAYER_SIZE_MIN)
		{
			g_Player2.fLength += -PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradTrigger(DIK_2) == true || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_2) == true)
	{
		g_Player2.fLength = g_Player2.fLengthDef;
	}

	if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
	{
		if (GetKeyboradTrigger(DIK_RCONTROL) == true || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_2) == true)
		{
			switch (g_Player2.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//弾の設定
				SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
				break;
			case TYPE_3WEY:
				//弾の設定
				Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//弾の設定
				SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//弾の設定
				SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
		else if (GetKeyboradRepeat(DIK_RCONTROL) == true || GetJoykeyRepeat(JOYKEY_RB, CONTROLLER_2) == true)
		{
			switch (g_Player2.BulletType)
			{
			case TYPE_NORMAL:
				g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//弾の設定
				SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
				break;
			case TYPE_3WEY:
				//弾の設定
				Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_ALL:
				//弾の設定
				SetAllBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SEARCH:
				//弾の設定
				SetSearchBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_BOM:
				//弾の設定
				SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			case TYPE_SP:
				//弾の設定
				SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

				if (g_Player2.BulletCount <= 0)
				{
					g_Player2.BulletType = TYPE_NORMAL;
				}
				break;
			}
		}
	}

	if (IsDirectInputControllerConnected(CONTROLLER_2))
	{
		if (GetdJoyPov(POV_UP, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.y += -PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHTUP, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_RIGHTDOWN, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.y += PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFTDOWN, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += -PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_LEFTUP, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
	}

	if (!strcmp(ControllerName(CONTROLLER_2), ELE_CON))
	{
		if (GetdJoykeyPress(ELEKEY_B, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(ELEKEY_Y, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(ELEKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(ELEKEY_A, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(ELEKEY_X, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(ELEKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(ELEKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(ELEKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_2), PS_CON))
	{
		if (GetdJoykeyPress(PSKEY_CR, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(PSKEY_SQ, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(PSKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(PSKEY_CI, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(PSKEY_TRA, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(PSKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(PSKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(PSKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_2), NIN_CON))
	{
		if (GetdJoykeyPress(NINKEY_A, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(NINKEY_X, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(NINKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(NINKEY_B, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(NINKEY_Y, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(NINKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(NINKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(NINKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}
	else if (!IsXInputControllerConnected(CONTROLLER_2) && IsDirectInputControllerConnected(CONTROLLER_2))
	{
		if (GetdJoykeyPress(DKEY_B, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(DKEY_Y, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(DKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(DKEY_A, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(DKEY_X, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(DKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(DKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}
			else if (GetdJoykeyRepeat(DKEY_RB, CONTROLLER_2) == true)
			{
				switch (g_Player2.BulletType)
				{
				case TYPE_NORMAL:
					g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
					g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

					//弾の設定
					SetBullet(g_Player2.pos[SENTER], g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
					break;
				case TYPE_3WEY:
					//弾の設定
					Set3Bullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_ALL:
					//弾の設定
					SetAllBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SEARCH:
					//弾の設定
					SetSearchBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_BOM:
					//弾の設定
					SetBomBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				case TYPE_SP:
					//弾の設定
					SetSPBullet(g_Player2.pos[SENTER], g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

					if (g_Player2.BulletCount <= 0)
					{
						g_Player2.BulletType = TYPE_NORMAL;
					}
					break;
				}
			}

		}
	}

	if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
	{
		Enemy* pEnemy;
		Boss* pBoss;
		Player* pPlayer, * pPlayer3, * pPlayer4;
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;

		pEnemy = GetEnemy();
		pBoss = GetBoss();
		pPlayer = GetPlayer();
		pPlayer3 = GetPlayer3();
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
				if (pEnemy->pos.x - EnemyWidth / 2 <= g_Player2.pos[SENTER].x + g_Player2.fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_Player2.pos[SENTER].x - g_Player2.fLength && pEnemy->pos.y - EnemyHeight / 2 <= g_Player2.pos[SENTER].y + g_Player2.fLength && pEnemy->pos.y + EnemyHeight / 2 >= g_Player2.pos[SENTER].y - g_Player2.fLength)
				{
					if (g_Player2.pos[SENTER].x <= pEnemy->pos.x - EnemyWidth / 2)
					{
						g_Player2.pos[SENTER].x = (pEnemy->pos.x - EnemyWidth / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos[SENTER].x >= pEnemy->pos.x + EnemyWidth / 2)
					{
						g_Player2.pos[SENTER].x = (pEnemy->pos.x + EnemyWidth / 2) + g_Player2.fLength;
					}
					else if (g_Player2.pos[SENTER].y <= pEnemy->pos.y - EnemyHeight / 2)
					{
						g_Player2.pos[SENTER].y = (pEnemy->pos.y - EnemyHeight / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos[SENTER].y >= pEnemy->pos.y + EnemyHeight / 2)
					{
						g_Player2.pos[SENTER].y = (pEnemy->pos.y + EnemyHeight / 2) + g_Player2.fLength;
					}
				}
			}
		}

		for (int i = 0; i < MAX_BOSS; i++,pBoss++)
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
				if (pBoss->pos.x - EnemyWidth / 2 <= g_Player2.pos[SENTER].x + g_Player2.fLength && pBoss->pos.x + EnemyWidth / 2 >= g_Player2.pos[SENTER].x - g_Player2.fLength && pBoss->pos.y - EnemyHeight / 2 <= g_Player2.pos[SENTER].y + g_Player2.fLength && pBoss->pos.y + EnemyHeight / 2 >= g_Player2.pos[SENTER].y - g_Player2.fLength)
				{
					if (g_Player2.pos[SENTER].x <= pBoss->pos.x - EnemyWidth / 2)
					{
						g_Player2.pos[SENTER].x = (pBoss->pos.x - EnemyWidth / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos[SENTER].x >= pBoss->pos.x + EnemyWidth / 2)
					{
						g_Player2.pos[SENTER].x = (pBoss->pos.x + EnemyWidth / 2) + g_Player2.fLength;
					}
					else if (g_Player2.pos[SENTER].y <= pBoss->pos.y - EnemyHeight / 2)
					{
						g_Player2.pos[SENTER].y = (pBoss->pos.y - EnemyHeight / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos[SENTER].y >= pBoss->pos.y + EnemyHeight / 2)
					{
						g_Player2.pos[SENTER].y = (pBoss->pos.y + EnemyHeight / 2) + g_Player2.fLength;
					}
				}
			}
		}


		if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
		{
			if (pPlayer->pos[SENTER].x - pPlayer->fLength <= g_Player2.pos[SENTER].x + g_Player2.fLength && pPlayer->pos[SENTER].x + pPlayer->fLength >= g_Player2.pos[SENTER].x - g_Player2.fLength && pPlayer->pos[SENTER].y - pPlayer->fLength <= g_Player2.pos[SENTER].y + g_Player2.fLength && pPlayer->pos[SENTER].y + pPlayer->fLength >= g_Player2.pos[SENTER].y - g_Player2.fLength)
			{
				if (g_Player2.pos[SENTER].x <= pPlayer->pos[SENTER].x - pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_2) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer->pos[SENTER].x = (g_Player2.pos[SENTER].x + g_Player2.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].x = (pPlayer->pos[SENTER].x - pPlayer->fLength) - g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer->pos[SENTER].x = (g_Player2.pos[SENTER].x + g_Player2.fLength) + pPlayer->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].x = (pPlayer->pos[SENTER].x - pPlayer->fLength) - g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].x >= pPlayer->pos[SENTER].x + pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_2) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer->pos[SENTER].x = (g_Player2.pos[SENTER].x - g_Player2.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].x = (pPlayer->pos[SENTER].x + pPlayer->fLength) + g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer->pos[SENTER].x = (g_Player2.pos[SENTER].x - g_Player2.fLength) - pPlayer->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].x = (pPlayer->pos[SENTER].x + pPlayer->fLength) + g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].y <= pPlayer->pos[SENTER].y - pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer->pos[SENTER].y = (g_Player2.pos[SENTER].y + g_Player2.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].y = (pPlayer->pos[SENTER].y - pPlayer->fLength) - g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer->pos[SENTER].y = (g_Player2.pos[SENTER].y + g_Player2.fLength) + pPlayer->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].y = (pPlayer->pos[SENTER].y - pPlayer->fLength) - g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].y >= pPlayer->pos[SENTER].y + pPlayer->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer->pos[SENTER].y = (g_Player2.pos[SENTER].y - g_Player2.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].y = (pPlayer->pos[SENTER].y + pPlayer->fLength) + g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer->pos[SENTER].y = (g_Player2.pos[SENTER].y - g_Player2.fLength) - pPlayer->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].y = (pPlayer->pos[SENTER].y + pPlayer->fLength) + g_Player2.fLength;
						}
					}
				}

				//pPlayer->move = g_Player2.move;
				//g_Player2.move = pPlayer->move;
			}
		}
		if (pPlayer3->state != PLAYERSTATE_APPEAR && pPlayer3->state != PLAYERSTATE_DIE)
		{
			if (pPlayer3->pos[SENTER].x - pPlayer3->fLength <= g_Player2.pos[SENTER].x + g_Player2.fLength && pPlayer3->pos[SENTER].x + pPlayer3->fLength >= g_Player2.pos[SENTER].x - g_Player2.fLength && pPlayer3->pos[SENTER].y - pPlayer3->fLength <= g_Player2.pos[SENTER].y + g_Player2.fLength && pPlayer3->pos[SENTER].y + pPlayer3->fLength >= g_Player2.pos[SENTER].y - g_Player2.fLength)
			{
				if (g_Player2.pos[SENTER].x <= pPlayer3->pos[SENTER].x - pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_2) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer3->pos[SENTER].x = (g_Player2.pos[SENTER].x + g_Player2.fLength) + pPlayer3->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].x = (pPlayer3->pos[SENTER].x - pPlayer3->fLength) - g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer3->pos[SENTER].x = (g_Player2.pos[SENTER].x + g_Player2.fLength) + pPlayer3->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].x = (pPlayer3->pos[SENTER].x - pPlayer3->fLength) - g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].x >= pPlayer3->pos[SENTER].x + pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_2) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer3->pos[SENTER].x = (g_Player2.pos[SENTER].x - g_Player2.fLength) - pPlayer3->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].x = (pPlayer3->pos[SENTER].x + pPlayer3->fLength) + g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer3->pos[SENTER].x = (g_Player2.pos[SENTER].x - g_Player2.fLength) - pPlayer3->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].x = (pPlayer3->pos[SENTER].x + pPlayer3->fLength) + g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].y <= pPlayer3->pos[SENTER].y - pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer3->pos[SENTER].y = (g_Player2.pos[SENTER].y + g_Player2.fLength) + pPlayer3->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].y = (pPlayer3->pos[SENTER].y - pPlayer3->fLength) - g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer3->pos[SENTER].y = (g_Player2.pos[SENTER].y + g_Player2.fLength) + pPlayer3->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].y = (pPlayer3->pos[SENTER].y - pPlayer3->fLength) - g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].y >= pPlayer3->pos[SENTER].y + pPlayer3->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer3->pos[SENTER].y = (g_Player2.pos[SENTER].y - g_Player2.fLength) - pPlayer3->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].y = (pPlayer3->pos[SENTER].y + pPlayer3->fLength) + g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer3->pos[SENTER].y = (g_Player2.pos[SENTER].y - g_Player2.fLength) - pPlayer3->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].y = (pPlayer3->pos[SENTER].y + pPlayer3->fLength) + g_Player2.fLength;
						}
					}
				}

				//pPlayer3->move = g_Player2.move;
				//g_Player2.move = pPlayer3->move;
			}
		}
		if (pPlayer4->state != PLAYERSTATE_APPEAR && pPlayer4->state != PLAYERSTATE_DIE)
		{
			if (pPlayer4->pos[SENTER].x - pPlayer4->fLength <= g_Player2.pos[SENTER].x + g_Player2.fLength && pPlayer4->pos[SENTER].x + pPlayer4->fLength >= g_Player2.pos[SENTER].x - g_Player2.fLength && pPlayer4->pos[SENTER].y - pPlayer4->fLength <= g_Player2.pos[SENTER].y + g_Player2.fLength && pPlayer4->pos[SENTER].y + pPlayer4->fLength >= g_Player2.pos[SENTER].y - g_Player2.fLength)
			{
				if (g_Player2.pos[SENTER].x <= pPlayer4->pos[SENTER].x - pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_2) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer4->pos[SENTER].x = (g_Player2.pos[SENTER].x + g_Player2.fLength) + pPlayer4->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].x = (pPlayer4->pos[SENTER].x - pPlayer4->fLength) - g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer4->pos[SENTER].x = (g_Player2.pos[SENTER].x + g_Player2.fLength) + pPlayer4->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].x = (pPlayer4->pos[SENTER].x - pPlayer4->fLength) - g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].x >= pPlayer4->pos[SENTER].x + pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_2) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer4->pos[SENTER].x = (g_Player2.pos[SENTER].x - g_Player2.fLength) - pPlayer4->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].x = (pPlayer4->pos[SENTER].x + pPlayer4->fLength) + g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer4->pos[SENTER].x = (g_Player2.pos[SENTER].x - g_Player2.fLength) - pPlayer4->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].x = (pPlayer4->pos[SENTER].x + pPlayer4->fLength) + g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].y <= pPlayer4->pos[SENTER].y - pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer4->pos[SENTER].y = (g_Player2.pos[SENTER].y + g_Player2.fLength) + pPlayer4->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].y = (pPlayer4->pos[SENTER].y - pPlayer4->fLength) - g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer4->pos[SENTER].y = (g_Player2.pos[SENTER].y + g_Player2.fLength) + pPlayer4->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].y = (pPlayer4->pos[SENTER].y - pPlayer4->fLength) - g_Player2.fLength;
						}
					}
				}
				else if (g_Player2.pos[SENTER].y >= pPlayer4->pos[SENTER].y + pPlayer4->fLength)
				{
					if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_2))
					{
						pPlayer4->pos[SENTER].y = (g_Player2.pos[SENTER].y - g_Player2.fLength) - pPlayer4->fLength;
					}
					else
					{
						g_Player2.pos[SENTER].y = (pPlayer4->pos[SENTER].y + pPlayer4->fLength) + g_Player2.fLength;
					}

					if (!IsXInputControllerConnected(CONTROLLER_2), IsDirectInputControllerConnected(CONTROLLER_2))
					{
						if (GetdJoyPov(POV_UP, 0, CONTROLLER_2) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_2))
						{
							pPlayer4->pos[SENTER].y = (g_Player2.pos[SENTER].y - g_Player2.fLength) - pPlayer4->fLength;
						}
						else
						{
							g_Player2.pos[SENTER].y = (pPlayer4->pos[SENTER].y + pPlayer4->fLength) + g_Player2.fLength;
						}
					}
				}

				//pPlayer4->move = g_Player2.move;
				//g_Player2.move = pPlayer4->move;
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
			g_Player2.pos[SENTER].x += (g_Player2.move.x - pBoss->move.x * PLAYER_FIELD_INA);
			g_Player2.pos[SENTER].y += (g_Player2.move.y - pBoss->move.y * PLAYER_FIELD_INA);
			break;
		}
		else if (i == MAX_BOSS - 1)
		{
			g_Player2.pos[SENTER].x += g_Player2.move.x;
			g_Player2.pos[SENTER].y += g_Player2.move.y;
		}
	}

	if (GetMode()==MODE_VS)
	{
		//離れすぎ防止
		Player* pPlayer;
		pPlayer = GetPlayer();
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player2.pos[SENTER].x - pPlayer->pos[SENTER].x;
		Ylong = g_Player2.pos[SENTER].y - pPlayer->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player2.pos[SENTER].x = pPlayer->pos[SENTER].x + Xlong * LongIna;
			g_Player2.pos[SENTER].y = pPlayer->pos[SENTER].y + Ylong * LongIna;
		}

		//離れすぎ防止
		Player* pPlayer3;
		pPlayer3 = GetPlayer3();
		Xlong = g_Player2.pos[SENTER].x - pPlayer3->pos[SENTER].x;
		Ylong = g_Player2.pos[SENTER].y - pPlayer3->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player2.pos[SENTER].x = pPlayer3->pos[SENTER].x + Xlong * LongIna;
			g_Player2.pos[SENTER].y = pPlayer3->pos[SENTER].y + Ylong * LongIna;
		}

		//離れすぎ防止
		Player* pPlayer4;
		pPlayer4 = GetPlayer4();
		Xlong = g_Player2.pos[SENTER].x - pPlayer4->pos[SENTER].x;
		Ylong = g_Player2.pos[SENTER].y - pPlayer4->pos[SENTER].y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player2.pos[SENTER].x = pPlayer4->pos[SENTER].x + Xlong * LongIna;
			g_Player2.pos[SENTER].y = pPlayer4->pos[SENTER].y + Ylong * LongIna;
		}
	}
	else
	{
		//離れすぎ防止
		float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f;
		Xlong = g_Player2.pos[SENTER].x - pBoss->pos.x;
		Ylong = g_Player2.pos[SENTER].y - pBoss->pos.y;
		Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
		if (Long >= SCREEN_WIDTH * PLAYER_SPARSE_INA)
		{
			// プレイヤーの位置を制限
			float LongIna = (SCREEN_WIDTH * PLAYER_SPARSE_INA) / Long;
			g_Player2.pos[SENTER].x = pBoss->pos.x + Xlong * LongIna;
			g_Player2.pos[SENTER].y = pBoss->pos.y + Ylong * LongIna;
		}
	}	

	if (bOberP1())
	{
		Player* pPlayer;
		pPlayer = GetPlayer();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer->pos[SENTER].x - g_Player2.pos[SENTER].x;
		Ylong = pPlayer->pos[SENTER].y - g_Player2.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player2.bDisp[LEFTUP] = true;
			if (GetMode()==MODE_VS)
			{
				g_Player2.pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA_VS, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player2.pos[LEFTUP] = D3DXVECTOR3(pPlayer->pos[LEFTUP].x - Xlong * SPLIT_INA, pPlayer->pos[LEFTUP].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player2.bDisp[LEFTUP] = false;
		}
	}
	if (bOberP3())
	{
		Player* pPlayer3;
		pPlayer3 = GetPlayer3();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer3->pos[SENTER].x - g_Player2.pos[SENTER].x;
		Ylong = pPlayer3->pos[SENTER].y - g_Player2.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player2.bDisp[LEFTDOWN] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player2.pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player2.pos[LEFTDOWN] = D3DXVECTOR3(pPlayer3->pos[LEFTDOWN].x - Xlong * SPLIT_INA, pPlayer3->pos[LEFTDOWN].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player2.bDisp[LEFTDOWN] = false;
		}
	}
	if (bOberP4())
	{
		Player* pPlayer4;
		pPlayer4 = GetPlayer4();
		float Xlong = 0.0f, Ylong = 0.0f;

		Xlong = pPlayer4->pos[SENTER].x - g_Player2.pos[SENTER].x;
		Ylong = pPlayer4->pos[SENTER].y - g_Player2.pos[SENTER].y;
		if (Xlong > -SCREEN_WIDTH / 2 && Xlong < SCREEN_WIDTH / 2 && Ylong>-SCREEN_HEIGHT / 2 && Ylong < SCREEN_HEIGHT / 2)
		{
			g_Player2.bDisp[RIGHTDOWN] = true;
			if (GetMode() == MODE_VS)
			{
				g_Player2.pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA_VS, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA_VS, 0.0f);
			}
			else
			{
				g_Player2.pos[RIGHTDOWN] = D3DXVECTOR3(pPlayer4->pos[RIGHTDOWN].x - Xlong * SPLIT_INA, pPlayer4->pos[RIGHTDOWN].y - Ylong * SPLIT_INA, 0.0f);
			}
		}
		else
		{
			g_Player2.bDisp[RIGHTDOWN] = false;
		}
	}

	//移動量の更新(減衰)
	g_Player2.move.x += (PLAYER_SPEED_DEF - g_Player2.move.x) * PLAYER_INA;
	g_Player2.move.y += (PLAYER_SPEED_DEF - g_Player2.move.y) * PLAYER_INA;

	g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		//座標設定
	pVtx[0].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[0].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[1].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
	pVtx[2].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player2.pos[SENTER].x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
	pVtx[3].pos.y = g_Player2.pos[SENTER].y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
	pVtx[3].pos.z = 0.0f;


	pVtx += VT_MAX;

	for (int i = 1; i < POS_MAX; i++)
	{
		if (GetMode()==MODE_VS)
		{
			//座標設定
			pVtx[0].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[0].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[1].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[2].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[3].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA_VS;
			pVtx[3].pos.z = 0.0f;
		}
		else
		{
			//座標設定
			pVtx[0].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[0].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[1].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength * SPLIT_INA;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
			pVtx[2].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_Player2.pos[i].x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
			pVtx[3].pos.y = g_Player2.pos[i].y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength * SPLIT_INA;
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += VT_MAX;
	}

	g_pVtxBuffPlayer2->Unlock();//プレイヤーバッファのアンロック

	g_Player2.nCounterAnim++;
	if ((g_Player2.nCounterAnim % UP_TIME) == 0)//更新期間
	{
		g_Player2.nPatternAnim = (g_Player2.nPatternAnim + 1) % (U_MAX * V_MAX);

		g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		for (int i = 0; i < POS_MAX; i++)
		{
			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX) + UV_DEF / V_MAX);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX) + UV_DEF / V_MAX);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer2->Unlock();//プレイヤーバッファのアンロック
	}

	if (g_Player2.state == PLAYERSTATE_DIE)
	{
		g_bOber2 = false;
	}
	else if (g_Player2.pos[SENTER].x + g_Player2.fLength <= 0 || g_Player2.pos[SENTER].x - g_Player2.fLength >= SCREEN_WIDTH || g_Player2.pos[SENTER].y + g_Player2.fLength <= 0 || g_Player2.pos[SENTER].y - g_Player2.fLength >= SCREEN_HEIGHT)//画面右端
	{//画面外
		g_bOber2 = true;
	}
	else
	{//画面内
		g_bOber2 = false;
	}

	switch (g_Player2.state)
	{
	case PLAYERSTATE_NORMAL:
		SetVibrate(0.0f, CONTROLLER_1);

		g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		for (int i = 0; i < POS_MAX; i++)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += VT_MAX;
		}

		g_pVtxBuffPlayer2->Unlock();//プレイヤーバッファのアンロック
		break;
	case PLAYERSTATE_APPEAR:
		g_Player2.nCounterState++;
		if (g_Player2.nCounterState >= 60)
		{
			g_Player2.state = PLAYERSTATE_NORMAL;
			g_Player2.bDisp[SENTER] = true;
		}
		else if (g_Player2.nCounterState % 5 == 0)
		{
			g_Player2.bDisp[SENTER] = !g_Player2.bDisp[SENTER];
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player2.nCounterState--;
		if (g_Player2.nCounterState <= 0)
		{
			g_Player2.state = PLAYERSTATE_NORMAL;

			g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer2->Unlock();//プレイヤーバッファのアンロック
		}
		break;
	case PLAYERSTATE_DIE:
		g_Player2.bDisp[SENTER] = false;
		break;
	}

	if (GetMode() == MODE_VS)
	{
		g_bOber2 = true;
	}
}

//-------------------
//プレイヤー描画処理
//-------------------
void DrawPlayer2(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_Player2.bDisp[SENTER])
	{
		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer2, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer2);

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

		if (bOberP1() && g_Player2.bDisp[LEFTUP])
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
		if (g_bOber2)
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX*2,//始まりの番号
				2//ポリゴンの個数
			);
		}
		if (bOberP3() && g_Player2.bDisp[LEFTDOWN])
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX * 3,//始まりの番号
				2//ポリゴンの個数
			);
		}
		if (bOberP4() && g_Player2.bDisp[RIGHTDOWN])
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
void HitPlayer2(int nDamage)
{
	if (g_Player2.state != PLAYERSTATE_DIE && g_Player2.state != PLAYERSTATE_APPEAR)
	{
		VERTEX_2D* pVtx;

		g_Player2.nLife -= nDamage;

		if (g_Player2.nLife <= 0)
		{
			if (g_Player2.nRema <= 0)
			{
				//SetExplosion(g_Player2.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player2.fLength);
				SetExef(g_Player2.pos[SENTER], g_Player2.fLength, true);
				if (GetMode()==MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player2.pos[LEFTUP], g_Player2.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player2.pos[RIGHTUP], g_Player2.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player2.pos[LEFTDOWN], g_Player2.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player2.pos[RIGHTDOWN], g_Player2.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player2.pos[LEFTUP], g_Player2.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player2.pos[RIGHTUP], g_Player2.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player2.pos[LEFTDOWN], g_Player2.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player2.pos[RIGHTDOWN], g_Player2.fLength * SPLIT_INA, false);
					}
				}
				g_Player2.state = PLAYERSTATE_DIE;
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
			}
			else
			{
				//SetExplosion(g_Player2.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player2.fLength);
				SetExef(g_Player2.pos[SENTER], g_Player2.fLength, true);
				if (GetMode() == MODE_VS)
				{
					if (bOberP1())
					{
						SetExef(g_Player2.pos[LEFTUP], g_Player2.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player2.pos[RIGHTUP], g_Player2.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player2.pos[LEFTDOWN], g_Player2.fLength * SPLIT_INA_VS, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player2.pos[RIGHTDOWN], g_Player2.fLength * SPLIT_INA_VS, false);
					}
				}
				else
				{
					if (bOberP1())
					{
						SetExef(g_Player2.pos[LEFTUP], g_Player2.fLength * SPLIT_INA, false);
					}
					if (bOberP2())
					{
						SetExef(g_Player2.pos[RIGHTUP], g_Player2.fLength * SPLIT_INA, false);
					}
					if (bOberP3())
					{
						SetExef(g_Player2.pos[LEFTDOWN], g_Player2.fLength * SPLIT_INA, false);
					}
					if (bOberP4())
					{
						SetExef(g_Player2.pos[RIGHTDOWN], g_Player2.fLength * SPLIT_INA, false);
					}
				}
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
				g_Player2.state = PLAYERSTATE_APPEAR;
				g_Player2.nCounterState = 0;
				g_Player2.nRema--;
				g_Player2.nLife = 100;
			}
			SetVibrate(1.0f, CONTROLLER_2);
		}
		else
		{
			g_Player2.state = PLAYERSTATE_DAMAGE;
			g_Player2.nCounterState = 5;
			if (GetMode() == MODE_GAME)
			{
				AddScore(-nDamage * SCORE_MISS);
			}

			g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);

			for (int i = 0; i < POS_MAX; i++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pVtx += VT_MAX;
			}

			g_pVtxBuffPlayer2->Unlock();//バッファのアンロック
		}
	}
}

//-------------------------
//プレイヤーバッファの取得
//-------------------------
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer2(void)
{
	return g_pVtxBuffPlayer2;
}

//-------------------------
//プレイヤー取得
//-------------------------
Player* GetPlayer2(void)
{
	return &g_Player2;
}

//------------------
//画面外
//------------------
bool bOberP2(void)
{
	return g_bOber2;
}