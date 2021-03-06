//DXライブラリの引用指示
#include <DxLib.h>
#include <windows.h>
#include <string>       // ヘッダファイルインクルード
#pragma comment(lib, "winmm.lib")

using namespace std;         //  名前空間指定
//ブロックの数を表すマクロ
#define BLOCK_NUM 24
#define WINDOW_X 640
#define WINDOW_Y 600

///ここまでの冒険を記録しますか？様の
///【ゲーム製作入門】C/C++でブロック崩しを作る【DXライブラリ】より
///http://nut-softwaredevelopper.hatenablog.com/entry/2015/04/13/214627

//ゲームに登場する物体は全てクラスとして扱うと便利
//今回はブロック・プレイヤー・ボールをそれぞれクラスにした

//ブロックのクラス
class Block {
//外部から参照できるようにするためpublic
public:

	//ブロックの座標
	int x, y;
	//ブロックのサイズ
	int width, height;
	int gh;
	//ブロックの生死状態
	bool live;

	//ブロック自体が生きていた場合描画を行う関数
	void View() {
		if (live) {
			DrawGraph(x, y, gh, TRUE);
		}
	}

	//ブロックを配置する関数
	Block(int setX, int setY) {
		x = setX;
		y = setY;
		//ブロックの画像を読み込む
		gh = LoadGraph("Resource/Sprite/Block.png");
		//読み込んだ画像から幅と高さを求めてint型に代入する関数
		GetGraphSize(gh, &width, &height);
		live = true;
	}

	//枚フレームごとに実行させたい処理は全てAll関数のようにまとめると楽
	void All() {
		View();
	}
};

//プレイヤーのクラス
class Player{
public:
	int x, y, width, height;
	int tempospeed;
	int gh;

	Player() {
		x = 320;
		y = 550;
		tempospeed = 938;
		gh = LoadGraph("Resource/Sprite/Player.png");
		width = 80;
		height = 20;
	}

	int Move() {
		if (CheckHitKey(KEY_INPUT_RIGHT) && x < 640- width) {
			x += 10 + (1000 - tempospeed)/25;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) && x > 0) {
			x -= 10 + (1000 - tempospeed) / 25;
		}	
		else {
			return 0;
		}
	}

	void View() {
		DrawExtendGraph(x, y, x + width, y + height, gh, TRUE);
	}

	void All() {
		Move();
		View();
	}
};

//ボールのクラス
class Ball {
public:
	int nextMovePosX, nextMovePosY;
	int x, y, r;
	int vecX, vecY;
	int speed;

	Ball() {
		x = 320;
		y = 300;
		r = 5;
		vecX = 1;
		vecY = 1;
		speed = 5;
	}

	void Move() {
		x = nextMovePosX;
		y = nextMovePosY;
	}

	void View() {
		DrawCircle(x, y, r, GetColor(255, 255, 255));
	}

	void All() {
		View();
	}
};

class Audio {
public:
	int audioMem;

	//ブロックを配置する関数
	Audio(int audiomem) {
		audioMem = audiomem;
	}

	void PlayAudio(int playType) {
		switch (playType)
		{
		case 0:
			PlaySoundMem(audioMem, DX_PLAYTYPE_NORMAL, TRUE);
			break;
		case 1:
			PlaySoundMem(audioMem, DX_PLAYTYPE_BACK, TRUE);
			break;
		case 2:
			PlaySoundMem(audioMem, DX_PLAYTYPE_LOOP, TRUE);
			break;
		default:
			break;
		}

	}

	void StopAudio() {
		StopSoundMem(audioMem);
	}

};

//ボールの弾道を表示する
class Line {
public:
	int pos1[2];
	int pos2[2];

	void drawline(int sx, int sy, int dx, int dy) {
		DrawLine(sx, sy, dx, dy, GetColor(255, 128, 128));
	}

	void All(){
		drawline(pos1[0], pos1[1], pos2[0], pos2[1]);
	}
};

//ゲームの動作処理をクラス内での関数で実行する
//その為メンバー変数として
class GameControl {
public:
	Block * bl[BLOCK_NUM];
	Player* pl;
	Ball* ba;
	Audio * bgm[2];
	Audio * se[6];
	Line * li;

	int catchScale;

	int life;
	int state;
	int titlegh;
	int gameOvergh;
	int cleargh;
	bool spacePushFlag;
	bool shiftPushFlag;
	bool ballCatchMode;
	int tempo;
	int startTime;
	bool wallSE;
	int bgmID;
	bool gamestart;

	//スペースキーが押されたかどうかを判断する
	//長押しによる連続入力を防ぐための関数
	bool PushSpace() {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			if (!spacePushFlag) {
				spacePushFlag = true;
				return true;
			}
		}
		else {
			spacePushFlag = false;
		}
		return false;
	}

	//シフトキーが押されたかどうかを判断する
	//長押しによる連続入力を防ぐための関数
	bool PushShift() {
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			if (!shiftPushFlag) {
				shiftPushFlag = true;
				return true;
			}
		}
		else {
			shiftPushFlag = false;
		}
		return false;
	}

	//コンストラクタでブロックプレイヤー、ボールを実体化させている
	GameControl() {
		gamestart = false;
		life = 2;
		catchScale = 7;
		state = 0;
		tempo = 938;
		wallSE = false;
		titlegh = LoadGraph("Resource/Sprite/Title.png");
		gameOvergh = LoadGraph("Resource/Sprite/GameOver.png");
		cleargh = LoadGraph("Resource/Sprite/Clear.png");
		spacePushFlag = false;
		shiftPushFlag = false;
		ballCatchMode = false;
		for (int i = 0; i < BLOCK_NUM; i++) {
			bl[i] = new Block(140 + (i % 4) * 100, 10 + (i / 4) * 50);
		}
		bgm[0] = new Audio(LoadSoundMem("Resource/Audio/Future_Monday.mp3"));
		bgm[1] = new Audio(LoadSoundMem("Resource/Audio/barsulbeat.mp3"));
		se[0] = new Audio(LoadSoundMem("Resource/Audio/bound.ogg"));
		se[1] = new Audio(LoadSoundMem("Resource/Audio/player.ogg"));
		se[2] = new Audio(LoadSoundMem("Resource/Audio/wall.ogg"));
		se[3] = new Audio(LoadSoundMem("Resource/Audio/miss.ogg"));
		
		pl = new Player();
		ba = new Ball();
		li = new Line();
		ba->vecX = 1;
		ba->vecY = 1;
		tempo = 938;
		bgm[bgmID]->StopAudio();
		bgmID = 0;
		bgm[bgmID]->PlayAudio(2);
	}
	//オブジェクトがdelete演算で消去されるときの処理

	//コンストラクタで実体化させたものを全てdeleteしているので
	//GameControlのポインタをdeleteするだけで全てdeleteできる
	~GameControl() {
		for (int i = 0; BLOCK_NUM; i++) {
			delete bl[i];
		}
		for (int i = 0; i>sizeof(se); i++) {
			delete se[i];
		}
		for (int i = 0; i>sizeof(bgm); i++) {
			delete bgm[i];
		}
		delete pl;
		delete ba;
		delete li;
	}

	//タイトル画面での処理(state=0)
	void Title() {
		DrawGraph(0, 0, titlegh, TRUE);
		if (CheckHitKey(KEY_INPUT_1)) {
			tempo = 938;
			bgm[bgmID]->StopAudio();
			bgmID = 0;
			bgm[bgmID]->PlayAudio(2);
		}
		if (CheckHitKey(KEY_INPUT_2)) {
			tempo = 324;
			bgm[bgmID]->StopAudio();
			bgmID = 1;
			bgm[bgmID]->PlayAudio(2);
		}
			
		if (PushSpace()) {
			pl->tempospeed = tempo;
			bgm[bgmID]->StopAudio();
			state = 1;
			ba->x = 320;
			ba->y = 300;
			bgm[bgmID]->PlayAudio(2);
			startTime = GetSoundCurrentTime(bgm[bgmID]->audioMem);
		}
	}

	void BallLine() {

		int m_targetPosX = ba->x+ ba->speed * ba->vecX, m_targetPosY = ba->y + ba->speed * ba->vecY;

		while (m_targetPosX < WINDOW_X && m_targetPosX > 0 &&
				m_targetPosY < WINDOW_Y && m_targetPosY > 0)
		{
			m_targetPosX += ba->speed * ba->vecX;
			m_targetPosY += ba->speed * ba->vecY;
			for (int i = 0; i < BLOCK_NUM; i++) {
				if (bl[i]->live) {
					if (m_targetPosX + ba->r > bl[i]->x && m_targetPosX - ba->r < bl[i]->x + bl[i]->width&&
						m_targetPosY + ba->r>bl[i]->y &&m_targetPosY - ba->r < bl[i]->y + bl[i]->height) {
						goto BallLine_exit;
					}
				}
			}
			if (m_targetPosX > pl->x && m_targetPosX < pl->x + pl->width &&m_targetPosY + ba->r >pl->y) {
				goto BallLine_exit;
			}

		}
	BallLine_exit:
		li->drawline(ba->x, ba->y, m_targetPosX, m_targetPosY);
		ba->nextMovePosX = m_targetPosX;
		ba->nextMovePosY = m_targetPosY;
	}

	//ゲーム本編での処理(state=1)
	//これまで�@〜�Fで書いてきたゲームを動かす処理を入れている
	void Game() {
		//startTime = GetNowCount();
		for (int i = 0; i < BLOCK_NUM; i++) {
			bl[i]->All();
		}
		pl-> All();
		ba-> View();
		BallLine();
		for (int i = 0; i < life; i++) {
			DrawCircle(20 + i * (life + ba->r + 10), 20, ba->r, GetColor(255, 255, 255));
		}
		for (int i = 0; BLOCK_NUM; i++) {
			if (bl[i]->live)break;
			if (i == BLOCK_NUM - 1) {
				gamestart = false;
				ba->vecX = 1;
				ba->vecY = 1;
				state = 3;
			}
		}
		if (gamestart && life > 0) {
			//if ((startTime - endTime)*1000 == tempo) {
			int m_time = GetSoundCurrentTime(bgm[bgmID]->audioMem) - startTime;
			if(m_time >= tempo){
			//if (GetSoundCurrentTime(au[0]->audioMem)/10 % tempo == 0) {
				startTime += tempo;
				//startTime = GetSoundCurrentTime(au[0]->audioMem);
				ballCatchMode = false;
				ba->Move();
				wallSE = false;
			}
			if (ballCatchMode) {
				ba->y = pl->y;
				ba->x = pl->x+pl->width/2;
			}
			else {
				
				if (!wallSE && ba->x + ba->r >= WINDOW_X) {
					ba->vecX = -1;
					se[2]->PlayAudio(1);
					wallSE = true;
				}
				if (!wallSE && ba->x - ba->r <= 0) {
					ba->vecX = 1;
					se[2]->PlayAudio(1);
					wallSE = true;
				}
				if (!wallSE && ba->y - ba->r <= 0) {
					ba->vecY = 1;
					se[2]->PlayAudio(1);
					wallSE = true;
				}
				/*
				//ボールを捕まえるときの処理
				if (PushShift() &&ba->x > pl->x && ba->x <pl->x + pl->width && ba->y + ba->r+catchScale > pl->y) {
					ballCatchMode = true;
				}
				*/
				//プレイヤーにボールが当たったときの処理
				if (!ballCatchMode && ba->x > pl->x && ba->x <pl->x + pl->width &&
					ba->y + ba->r >pl->y &&ba->y < pl->y + pl->height) {
					se[1]->PlayAudio(1);
					ballCatchMode=true;
					ba->vecY = -1;
				}
				if (ba->y > WINDOW_Y) {
					gamestart = false;
					se[3]->PlayAudio(1);
					ba->x = 320;
					ba->y = 300;
					life--;
				}
				for (int i = 0; i < BLOCK_NUM; i++) {
					if (bl[i]->live) {
						if (ba->x > bl[i]->x && ba->x < bl[i]->x + bl[i]->width &&
							ba->y + ba->r>bl[i]->y && ba->y + ba->r < bl[i]->y + bl[i]->height) {
							se[0]->PlayAudio(1);
							bl[i]->live = false;
							ba->vecY *= -1;
						}
						if (ba->x > bl[i]->x && ba->x < bl[i]->x + bl[i]->width &&
							ba->y - ba->r>bl[i]->y && ba->y - ba->r < bl[i]->y + bl[i]->height) {
							se[0]->PlayAudio(1);
							bl[i]->live = false;
							ba->vecY *= -1;
						}
						if (ba->x + ba->r > bl[i]->x && ba->x + ba->r < bl[i]->x + bl[i]->width &&
							ba->y >= bl[i]->y && ba->y <= bl[i]->y + bl[i]->height) {
							se[0]->PlayAudio(1);
							bl[i]->live = false;
							ba->vecX *= -1;
						}
						if (ba->x - ba->r > bl[i]->x && ba->x - ba->r < bl[i]->x + bl[i]->width &&
							ba->y >= bl[i]->y && ba->y <= bl[i]->y + bl[i]->height) {
							se[0]->PlayAudio(1);
							bl[i]->live = false;
							ba->vecX *= -1;
						}
					}
				}
			}
		}
		else if (life > 0) {
			DrawFormatString(260, 360, GetColor(255, 255, 255), "Space押さんかい");
			//if (CheckHitKey(KEY_INPUT_SPACE)) {
			if (PushSpace()) {
				gamestart = true;
			}
		}
		else {
			state = 2;
		}
		if (GetSoundCurrentTime(bgm[bgmID]->audioMem) - startTime >= tempo) {
			//startTime = GetSoundCurrentTime(au[0]->audioMem);
			startTime += tempo;
		}
	}
	
	//ゲームオーバー時の処理(state=2)
	void GameOver() {
		bgm[bgmID]->StopAudio();
		DrawGraph(0, 0, gameOvergh, TRUE);
		//if (CheckHitKey(KEY_INPUT_SPACE)) {
		if (PushSpace()) {
			state = 0;
			life = 2;
			for (int i = 0; i < BLOCK_NUM; i++) {
				bl[i]->live = true;
			}
		}
	}

	//ゲームクリア時の処理(state=3)
	void GameClear() {
		bgm[bgmID]->StopAudio();
		DrawGraph(0, 0, cleargh, TRUE);
		if (PushSpace()) {
			state = 0;
			life = 2;
			for (int i = 0; i < BLOCK_NUM; i++) {
				bl[i]->live = true;
			}
		}
	}

	//stateの値に応じてゲームの場面を切り替える
	void All() {
		if (state == 0) {
			Title();
		}
		if (state == 1) {
			Game();
		}
		if (state == 2)GameOver();
		if (state == 3)GameClear();
	}
};

//main()と大体同じ
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	

	//ウィンドウモード
	ChangeWindowMode(TRUE);

	//ウィンドウのサイズを冒頭でマクロ定義した値になる
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);
	//DXライブラリを使うのに必要。何かエラーがあればWinMain()を抜けてくれる
	if (DxLib_Init() == -1)return -1;
	//動くゲームを表示するのに必要な裏画面を描画するための関数
	SetDrawScreen(DX_SCREEN_BACK);

	GameControl* ga = new GameControl();
	timeBeginPeriod(1);
	while (ProcessMessage() != -1)
	{
		int startTime = GetNowCount();
		ScreenFlip();
		ClearDrawScreen();

		ga->All();

		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
		int endTime = GetNowCount();
		WaitTimer((1000 / 60) - (endTime - startTime));

	}
	timeEndPeriod(1);
	delete ga;

	//Dxライブラリを終了させる関数
	DxLib_End();
	//winMainが無事終了したことをあらわす
	return 0;
}