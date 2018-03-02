//DXライブラリの引用指示
#include <DxLib.h>
///ここまでの冒険を記録しますか？様の
///【ゲーム製作入門】C/C++でブロック崩しを作る【DXライブラリ】より
///http://nut-softwaredevelopper.hatenablog.com/entry/2015/04/13/214627

//⑥での追加

//ブロックについてのクラス
class Block {
//外部から参照できるようにするためpublic
public:

	//ブロックの座標
	int blockX, blockY;
	//ブロックのサイズ
	int width, height;
	//ブロックの生死状態
	bool live;

	//ブロック自体が生きていた場合描画を行う関数
	void View() {
		if (live) {
			DrawBox(blockX,blockY,blockX+width,blockY+height,GetColor(255,255,255),TRUE);
		}
	}


	//ボールなどがブロックに入ったら消える処理
	void liveControl(int targetX, int targetY, int targetVec) {
		if (targetX > blockX && targetX<blockX + width && targetY > blockY && targetY < blockY && blockY < blockY + height) {
			live = false;
		}
		if (targetVec = -0 && CheckHitKey(KEY_INPUT_SPACE)) {
			live = true;
		}
	}

	//ブロックを配置する関数
	Block(int setX, int setY) {
		blockX = setX;
		blockY = setY;
		width = 90;
		height = 20;
		live = true;
	}
};
//追加ここまで

//main()と大体同じ
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	

	//ウィンドウモード
	ChangeWindowMode(TRUE);
	//DXライブラリを使うのに必要。何かエラーがあればWinMain()を抜けてくれる
	if (DxLib_Init() == -1)return -1;
	//動くゲームを表示するのに必要な裏画面を描画するための関数
	SetDrawScreen(DX_SCREEN_BACK);

	//プレイヤーバーの色{red,green,blue}
	const int playerColor[3]{
		100100,100
	};

	//ボールの動く方向を変数で表す。vecXが+1、右-1で左。VecYが+1で下、-1で上。
	//今回からスペースキーを押すとこの値が変わり、ボールが動き出す仕組みに変更する。
	int vecX = 0, vecY = 0;
	

	//ボールの移動スピード
	const int ballSpeed = 5;

	//画面サイズ
	const int windowSizeX = 640, windowSizeY = 480;

	//プレイヤーバーのサイズ
	int playerSizeX = 120, playerSizeY = 20;

	//プレイヤーバーの初期位置
	const int playerBarInitialX = windowSizeX /2 - playerSizeX/2, playerBarInitialY = 400;

	//プレイヤーバーの位置
	int playerBarX = playerBarInitialX, playerBarY = playerBarInitialY;

	//ボールのスポーン位置
	const int ballSpawnPosX = windowSizeX / 2, ballSpawnPosY = playerBarInitialY-50;

	//ボールの座標
	int ballPosX = ballSpawnPosX, ballPosY = ballSpawnPosY;

	//ボールの半径
	const int ballRadius = 10;

	//ボールの色{red,green,blue}
	const int ballColor[3]{
		255,255,255
	};

	//ブロックを変数（配列）として管理
	Block* bl[20];
	for (int i = 0; i < 20; i++) {
		//配置したい場所のブロックの座標を入力する
		bl[i] = new Block(100 + 110 * (i % 4), 40 + 40 * (i / 4));
	}

	//windowsのアプリを実行するときにはProcessMassage()という関数を定期的に呼び出さなければならない為
	while (ProcessMessage() != -1) {
		//呼び出された時点での時間をミリ秒で返す
		int startTime = GetNowCount();
		
		//裏画面を表示する関数
		ScreenFlip();
		//裏画面を消去
		ClearDrawScreen();
		
		//円を書く関数（円の中心のX座標,Y座標,半径,色,塗りつぶすか否か）
		//Getカラーは色番号を変換
		DrawCircle(ballPosX, ballPosY, ballRadius, GetColor(ballColor[0], ballColor[1], ballColor[2]), TRUE);

		//プレイヤーの描画を行う。DrawBox(左上頂点のX座標, Y座標, 右下頂点X座標, Y座標, 色, 塗りつぶすか否か)
		DrawBox(playerBarX, playerBarY, playerBarX + playerSizeX, playerBarY + playerSizeY, GetColor(playerColor[0], playerColor[1], playerColor[2]), TRUE);

		for (int i = 0; i < 20; i++) {
			bl[i]->View();
		}

		//ボールのスピードが0のときは動かないので条件分岐
		if(vecX != 0&& vecY != 0){
			//弾が跳ね返る条件文
			if (ballPosX > windowSizeX)vecX = -1;
			if (ballPosX < 0)vecX = 1;
			if (ballPosY < 0)vecY = 1;
			if (ballPosY > windowSizeY)vecY = -1;

			//プレイヤーと弾が接触したら弾のY方向の向きを-1にして跳ね返す
			if (ballPosX > playerBarX && ballPosX < playerBarX + playerSizeX &&
				ballPosY > playerBarY &&ballPosY < playerBarY + playerSizeY) vecY = -1;
			//画面より下にボールが出たらミスとして扱う
			if (ballPosY > windowSizeY) {
				ballPosX = ballSpawnPosX;
				ballPosY = ballSpawnPosY;
				vecX = 0;
				vecY = 0;
			}
			for (int i = 0;i < 20; i++) {
				//ブロックにボールが当たったときのボールの跳ね方
				if (bl[i]->live) {
					if (ballPosX > bl[i]->blockX && ballPosX < bl[i]->blockX + bl[i]->width &&
						ballPosY + ballRadius > bl[i]->blockY && ballPosY + ballRadius < bl[i]->blockY + bl[i]->height) {//上
						bl[i]->live = false;
						vecY *= -1;
					}
					if (ballPosX > bl[i]->blockX && ballPosX < bl[i]->blockX + bl[i]->width &&
						ballPosY - ballRadius > bl[i]->blockY && ballPosY - ballRadius < bl[i]->blockY + bl[i]->height) {//下
						bl[i]->live = false;
						vecY *= -1;
					}
					if (ballPosX + ballRadius > bl[i]->blockX && ballPosX + ballRadius < bl[i]->blockX + bl[i]->width &&
						ballPosY > bl[i]->blockY && ballPosY < bl[i]->blockY + bl[i]->height) {//左
						bl[i]->live = false;
						vecX *= -1;
					}
					if (ballPosX - ballRadius > bl[i]->blockX && ballPosX - ballRadius < bl[i]->blockX + bl[i]->width &&
						ballPosY > bl[i]->blockY && ballPosY < bl[i]->blockY + bl[i]->height) {//右
						bl[i]->live = false;
						vecX *= -1;
					}
				}
			}
		}
		else {
			//スペースキーを押すと開始させる。
			DrawFormatString(260, 160, GetColor(255, 100, 255), "SPACE押さんかい");
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				vecX = 1;
				vecY = -1;
				for (int i = 0; i<20; i++)bl[i]->live = true;
			}
		}
		//ゲームクリア時の処理
		for (int i = 0; i < 20; i++) {
			if (bl[i]->live)break;
			if (i == 19) {
				DrawFormatString(260, 120, GetColor(255,100, 255), "やるやん");
				ballPosX = 320;
				ballPosY = 240;
				vecX = 0;
				vecY = 0;
			}
		}

		//弾の座標を動かす
		ballPosX += ballSpeed * vecX;
		ballPosY += ballSpeed * vecY;

		//CheckHitKey()で右キーが”押されているか”を判定し、押されている間はプレイヤーのX座標を+10する。
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && playerBarX < windowSizeX - playerSizeX) {
			playerBarX += 10;
		}
		//逆に左キーが押されていた場合、プレイヤーのX座標を-10する。
		if (CheckHitKey(KEY_INPUT_LEFT) == 1 && playerBarX > 0) {
			playerBarX -= 10;
		}

		//引数で指定するキーが押されているかどうか。押されていれば1を返す
		//今回の場合escが押されると1を返し、ループを抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;

		//startTimeと同じ。ループを一周するごとに発生する時間差を修正するのに使用
		int endTime = GetNowCount();

		//ミリ秒単位で指定した時間、処理を停止する関数
		//60FPSで1秒＝1000ミリ秒なので1000/60ミリ秒が50回連続したら1000ミリ秒（1秒）
		//この値にループ中に発生した時間差を引くと正確に1/60秒でループが一周終わる
		WaitTimer(1000 / 60 - (endTime - startTime));
	}
	//Dxライブラリを終了させる関数
	DxLib_End();
	//winMainが無事終了したことをあらわす
	return 0;
}