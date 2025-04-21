//------------------------------------------
//
//シューティングゲームプログラムのメインの処理の定義・宣言[main.h]
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
#include"dinput.h" // 入力に必要なファイル
#include"Xinput.h" // Xinput(パッド処理)
#include"xaudio2.h"

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")

// マクロ定義
#define FPS (1000/60) // 更新スピード
#define UP_TIME (3) // テクスチャスピード

// 名前
#define CLASS_NAME "WindowClass" // クラス名
#define WINDOW_NAME "勇者は結局兵器が強いことに気付いたようです" // ウインドウ名
#define DATA_FILE "data\\DATA\\data.bin"//スコア保存用ファイル
#define TEXTURE_CURSOR "data\\TEXTURE\\MouseCursor.png"//カーソル
#define TEXTURE_TITLE "data\\TEXTURE\\field003.jpg"//タイトル背景
#define TEXTURE_TITLEROGO "data\\TEXTURE\\TitleRogo.png"//タイトルロゴ
#define TEXTURE_TITLEROGOTXT "data\\TEXTURE\\Rogo.png"//タイトル文字
#define TEXTURE_SELECT1 "data\\TEXTURE\\Enter or Start.png"//セレクト表示
#define TEXTURE_SELECT2 "data\\TEXTURE\\Select.png"//スクロール型セレクト
#define TEXTURE_RESULT "data\\TEXTURE\\field002.jpg"//リザルト背景
#define TEXTURE_SELECTRESTART "data\\TEXTURE\\Restart.png"//リスタート
#define TEXTURE_PLAYER1 "data\\TEXTURE\\yuusiya.png" // プレイヤー
#define TEXTURE_PLAYER2 "data\\TEXTURE\\mahoutukai.png" // プレイヤー
#define TEXTURE_PLAYER3 "data\\TEXTURE\\riyoukisi.png" // プレイヤー
#define TEXTURE_PLAYER4 "data\\TEXTURE\\souryo.png" // プレイヤー
#define TEXTURE_NAME_BULLET "data\\TEXTURE\\bullet000.png" // 弾
#define TEXTURE_NAME_FIELD "data\\TEXTURE\\field002.jpg"//ゲームフィールド
#define TEXTURE_NAME_EXP "data\\TEXTURE\\explosion001.png"//爆発
#define TEXTURE_NAME_ENE "data\\TEXTURE\\gi.jpg"//敵
#define TEXTURE_NAME_ENE2 "data\\TEXTURE\\anko.png"//敵
#define TEXTURE_NAME_ENE3 "data\\TEXTURE\\anko2.png"//敵
#define TEXTURE_NAME_ENE4 "data\\TEXTURE\\cat.png"//敵
#define TEXTURE_SCORE "data\\TEXTURE\\number000.png"//スコア
#define TEXTURE_SELECTBACK "data\\TEXTURE\\Back to Title.png"//タイトルに戻る
#define TEXTURE_SELECTRETRY "data\\TEXTURE\\Retry.png"//リトライ
#define TEXTURE_SELECTEXIT "data\\TEXTURE\\Exit.png"//終了
#define TEXTURE_SCORENUMBER "data\\TEXTURE\\Number.png"//順位表示
#define TEXTURE_EFFECT "data\\TEXTURE\\effect002.png"//エフェクト
#define TEXTURE_ITEM "data\\TEXTURE\\item.png"//アイテム
#define TEXTURE_NAME_MARK "data\\TEXTURE\\Mark001.png"//マーク
#define TEXTURE_NAME_BOSS "data\\TEXTURE\\kerberos.png"//敵
#define TEXTURE_NAME_BOSS2 "data\\TEXTURE\\dragon.png"//敵
#define TEXTURE_NAME_BOSS3 "data\\TEXTURE\\maou.png"//敵
#define TEXTURE_NAME_BOSS4 "data\\TEXTURE\\zeus.png"//敵

// 大きさ
#define SCREEN_WIDTH (1280) // 幅
#define SCREEN_HEIGHT (720) // 高さ

// 頂点
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 頂点フォーマット
#define VT_MAX (4)//頂点数
#define UV_DEF (1.0f) // テクスチャのデフォルトのサイズ

// コントローラー
#define	ELE_CON "GP30X-Dinput"//学校支給コントローラ
#define PS_CON "Wireless Controller"//プレステ4
#define NIN_CON "Pro Controller"//プロコン

//デバッグ用
#define FPS_TIME (500)
#define FONT_SIZE (20)

// 頂点情報の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos; // 座標
	float rhw; // 座標変換係数(1.0f)
	D3DCOLOR col; // カラー
	D3DXVECTOR2 tex; // テクスチャ座標
} VERTEX_2D;

// 画面の種類
typedef enum
{
	MODE_TITLE = 0,//タイトル
	MODE_DEMO,//デモ
	MODE_TUTO,//チュートリアル
	MODE_PLAYSELECT,//プレイセレクト
	MODE_GAME,//ゲーム
	MODE_VS,//バーサス
	MODE_RESULT,//リザルト
	MODE_RANK,//ランキング
	MODE_OPTION,//オプション
	MODE_MAX//モード数
} MODE;

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウインドウプロシージャ
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // 初期化処理
void Uninit(void); // 終了処理
void Update(void); // 更新処理
void Draw(void); // 描画処理
LPDIRECT3DDEVICE9 GetDevice(void); // デバイスの取得
void SetMode(MODE mode);//モード設定
MODE GetMode(void);//モード取得
HWND GethWnd(void);//ハンドル取得
HINSTANCE GethInstanse(void);//インスタンス取得

void ConfineCursorToWindow(HWND hwnd);//カーソル制御
void ReleaseCursor(void);//カーソル制御解除

void ToggleFullScreen(HWND hWnd);//フルスクリーン
void ResetDevice(void);//デバイス再構築

//デバッグ用
void DrawDebug(int nData, float *fData, char *cData);

#endif // _MAIN_H_