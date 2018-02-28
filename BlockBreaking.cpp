//DXライブラリの引用指示
#include <DxLib.h>

//main()と大体同じ
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	///ここまでの冒険を記録しますか？様の
	///【ゲーム製作入門】C/C++でブロック崩しを作る【DXライブラリ】より
	///http://nut-softwaredevelopper.hatenablog.com/entry/2015/04/13/214627

	//ウィンドウモード
	ChangeWindowMode(TRUE);
	//DXライブラリを使うのに必要。何かエラーがあればWinMain()を抜けてくれる
	if (DxLib_Init() == -1)return -1;
	//動くゲームを表示するのに必要な裏画面を描画するための関数
	SetDrawScreen(DX_SCREEN_BACK);

	//円(ボール)の中心
	int x = 40;
	int y = 40;

	//今回②での追加

	//ボールの動く方向を変数で表す。vecXが+1、右-1で左。VecYが+1で下、-1で上。
	int vecX = 1, vecY = 1;

	//追加ここまで

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
		DrawCircle(x, y, 10, GetColor(255, 255, 255), TRUE);

		//今回②での追加

		//弾が跳ね返る条件文
		//640はデフォルトでの画面サイズの横、480は画面サイズの縦
		if (x > 640)vecX = -1;
		if (x < 0)vecX = 1;
		if (y < 0)vecY = 1;
		if (y > 480)vecY = -1;

		//弾の座標を動かす（5はスピード）
		x += 5 * vecX;
		y += 5 * vecY;

		//追加ここまで

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