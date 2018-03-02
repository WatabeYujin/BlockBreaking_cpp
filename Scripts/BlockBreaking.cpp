//DXライブラリの引用指示
#include <DxLib.h>

#include <string>       // ヘッダファイルインクルード

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
	int speed;
	int gh;

	Player() {
		x = 320;
		y = 550;
		speed = 10;
		gh = LoadGraph("Resource/Sprite/Player.png");
		width = 80;
		height = 20;
	}

	void Move() {
		if (CheckHitKey(KEY_INPUT_RIGHT) && x < 640- width) {
			x += speed;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) && x > 0) {
			x -= speed;
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
	int x, y, r;
	int vecX, vecY;
	int speed;

	Ball() {
		x = 320;
		y = 300;
		r = 5;
		vecX = 0;
		vecY = 0;
		speed = 5;
	}

	void Move() {
		x += speed * vecX;
		y += speed * vecY;
	}

	void View() {
		DrawCircle(x, y, r, GetColor(255, 255, 255));
	}

	void All() {
		Move();
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

//ゲームの動作処理をクラス内での関数で実行する
//その為メンバー変数として
class GameControl {
public:
	Block * bl[BLOCK_NUM];
	Player* pl;
	Ball* ba;
	Audio * au[3];

	int life;
	int state;
	int titlegh;
	int gameOvergh;
	int cleargh;
	bool pushFlag;

	//スペースキーが押されたかどうかを判断する
	//長押しによる連続入力を防ぐための関数
	bool PushSpace() {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			if (!pushFlag) {
				pushFlag = true;
				return true;
			}
		}
		else {
			pushFlag = false;
		}
		return false;
	}

	//コンストラクタでブロックプレイヤー、ボールを実体化させている
	GameControl() {
		life = 2;
		state = 0;
		titlegh = LoadGraph("Resource/Sprite/Title.png");
		gameOvergh = LoadGraph("Resource/Sprite/GameOver.png");
		cleargh = LoadGraph("Resource/Sprite/Clear.png");
		pushFlag = false;
		for (int i = 0; i < BLOCK_NUM; i++) {
			bl[i] = new Block(140 + (i % 4) * 100, 10 + (i / 4) * 50);
		}
		au[0] = new Audio(LoadSoundMem("Resource/Audio/Uprising.mp3"));
		au[1] = new Audio(LoadSoundMem("Resource/Audio/bound.ogg"));
		au[2] = new Audio(LoadSoundMem("Resource/Audio/player.ogg"));
		pl = new Player();
		ba = new Ball();
	}

	//オブジェクトがdelete演算で消去されるときの処理
	//コンストラクタで実体化させたものを全てdeleteしているので
	//GameControlのポインタをdeleteするだけで全てdeleteできる
	~GameControl() {
		for (int i = 0; BLOCK_NUM; i++) {
			delete bl[i];
		}
		for (int i = 0; 1; i++) {
			delete au[i];
		}
		delete pl;
		delete ba;
	}

	//タイトル画面での処理(state=0)
	void Title() {
		DrawGraph(0, 0, titlegh, TRUE);
		if (PushSpace()) {
			state = 1;
			ba->x = 320;
			ba->y = 300;
			au[0]->PlayAudio(2);
		}
	}

	//ゲーム本編での処理(state=1)
	//これまで①～⑦で書いてきたゲームを動かす処理を入れている
	void Game() {
		for (int i = 0; i < BLOCK_NUM; i++) {
			bl[i]->All();
		}
		pl->All();
		ba->All();

		for (int i = 0; i < life; i++) {
			DrawCircle(20 + i * (life + ba->r + 10), 20, ba->r, GetColor(255, 255, 255));
		}

		for (int i = 0; BLOCK_NUM; i++) {
			if (bl[i]->live)break;
			if (i == BLOCK_NUM - 1) {
				ba->vecX = 0;
				ba->vecY = 0;
				state = 3;
			}
		}

		if (ba->vecX != 0 && ba->vecX != 0 && life > 0) {
			if (ba->x > WINDOW_X)ba->vecX = -1;
			if (ba->x < 0)ba->vecX = 1;
			if (ba->y < 0)ba->vecY = 1;
			if (ba->x > pl->x && ba-> x <pl ->x + pl -> width && ba -> y + ba -> r >pl->y) {
				au[2]->PlayAudio(1);
				ba->vecY = -1;
			}
			if (ba->y > WINDOW_Y) {
				ba->x = 320;
				ba->y = 300;
				ba->vecX = 0;
				ba->vecY = 0;
				life--;
			}
			for (int i = 0; i < BLOCK_NUM; i++) {
				if (bl[i]->live) {
					if (ba->x > bl[i]->x && ba->x < bl[i]->x + bl[i]->width &&
						ba->y + ba->r>bl[i]->y && ba->y + ba->r < bl[i]->y + bl[i]->height) {
						au[1]->PlayAudio(1);
						bl[i]->live = false;
						ba->vecY *= -1;
					}
					if (ba->x > bl[i]->x && ba->x < bl[i]->x + bl[i]->width &&
						ba->y - ba->r>bl[i]->y && ba->y - ba->r < bl[i]->y + bl[i]->height) {
						au[1]->PlayAudio(1);
						bl[i]->live = false;
						ba->vecY *= -1;
					}
					if (ba->x+ba->r > bl[i]->x && ba->x +ba->r < bl[i]->x + bl[i]->width &&
						ba->y > bl[i]->y && ba->y < bl[i] ->y+bl[i]->height) {
						au[1]->PlayAudio(1);
						bl[i]->live = false;
						ba->vecX *= -1;
					}
					if (ba->x - ba->r > bl[i]->x && ba->x - ba->r < bl[i]->x + bl[i]->width &&
						ba->y > bl[i]->y && ba->y < bl[i]->y + bl[i]->height) {
						au[1]->PlayAudio(1);
						bl[i]->live = false;
						ba->vecX *= -1;
					}
				}
			}
		}
		else if (life > 0) {
			DrawFormatString(260, 360, GetColor(255, 255, 255), "Space押さんかい");
			//if (CheckHitKey(KEY_INPUT_SPACE)) {
			if(PushSpace()){
				ba->vecX = 1;
				ba->vecY = 1;
			}
		}
		else {
			state = 2;
		}
	}
	
	//ゲームオーバー時の処理(state=2)
	void GameOver() {
		au[0]->StopAudio();
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
		au[0]->StopAudio();
		DrawGraph(0, 0, cleargh, TRUE);
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			state = 0;
			life = 2;
			for (int i = 0; i < BLOCK_NUM; i++) {
				bl[i]->live = true;
			}
		}
	}

	//stateの値に応じてゲームの場面を切り替える
	void All() {
		if (state == 0)Title();
		if (state == 1)Game();
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

	delete ga;

	//Dxライブラリを終了させる関数
	DxLib_End();
	//winMainが無事終了したことをあらわす
	return 0;
}