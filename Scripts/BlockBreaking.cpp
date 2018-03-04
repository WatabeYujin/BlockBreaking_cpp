//DX���C�u�����̈��p�w��
#include <DxLib.h>
#include <mmsystem.h>
#include <string>       // �w�b�_�t�@�C���C���N���[�h

using namespace std;         //  ���O��Ԏw��
//�u���b�N�̐���\���}�N��
#define BLOCK_NUM 24
#define WINDOW_X 640
#define WINDOW_Y 600

///�����܂ł̖`�����L�^���܂����H�l��
///�y�Q�[���������zC/C++�Ńu���b�N���������yDX���C�u�����z���
///http://nut-softwaredevelopper.hatenablog.com/entry/2015/04/13/214627

//�Q�[���ɓo�ꂷ�镨�̂͑S�ăN���X�Ƃ��Ĉ����ƕ֗�
//����̓u���b�N�E�v���C���[�E�{�[�������ꂼ��N���X�ɂ���

//�u���b�N�̃N���X
class Block {
//�O������Q�Ƃł���悤�ɂ��邽��public
public:

	//�u���b�N�̍��W
	int x, y;
	//�u���b�N�̃T�C�Y
	int width, height;
	int gh;
	//�u���b�N�̐������
	bool live;

	//�u���b�N���̂������Ă����ꍇ�`����s���֐�
	void View() {
		if (live) {
			DrawGraph(x, y, gh, TRUE);
		}
	}

	//�u���b�N��z�u����֐�
	Block(int setX, int setY) {
		x = setX;
		y = setY;
		//�u���b�N�̉摜��ǂݍ���
		gh = LoadGraph("Resource/Sprite/Block.png");
		//�ǂݍ��񂾉摜���畝�ƍ��������߂�int�^�ɑ������֐�
		GetGraphSize(gh, &width, &height);
		live = true;
	}

	//���t���[�����ƂɎ��s�������������͑S��All�֐��̂悤�ɂ܂Ƃ߂�Ɗy
	void All() {
		View();
	}
};

//�v���C���[�̃N���X
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

	int Move() {
		if (CheckHitKey(KEY_INPUT_RIGHT) && x < 640- width) {
			x += speed;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) && x > 0) {
			x -= speed;
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

//�{�[���̃N���X
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
		vecX = 0;
		vecY = 0;
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

	//�u���b�N��z�u����֐�
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

//�{�[���̒e����\������
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

//�Q�[���̓��쏈�����N���X���ł̊֐��Ŏ��s����
//���̈׃����o�[�ϐ��Ƃ���
class GameControl {
public:
	Block * bl[BLOCK_NUM];
	Player* pl;
	Ball* ba;
	Audio * au[3];
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
	int endTime;

	//�X�y�[�X�L�[�������ꂽ���ǂ����𔻒f����
	//�������ɂ��A�����͂�h�����߂̊֐�
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

	//�V�t�g�L�[�������ꂽ���ǂ����𔻒f����
	//�������ɂ��A�����͂�h�����߂̊֐�
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

	//�R���X�g���N�^�Ńu���b�N�v���C���[�A�{�[�������̉������Ă���
	GameControl() {
		life = 2;
		catchScale = 7;
		state = 0;
		tempo = 59;
		titlegh = LoadGraph("Resource/Sprite/Title.png");
		gameOvergh = LoadGraph("Resource/Sprite/GameOver.png");
		cleargh = LoadGraph("Resource/Sprite/Clear.png");
		spacePushFlag = false;
		shiftPushFlag = false;
		ballCatchMode = false;
		for (int i = 0; i < BLOCK_NUM; i++) {
			bl[i] = new Block(140 + (i % 4) * 100, 10 + (i / 4) * 50);
		}
		au[0] = new Audio(LoadSoundMem("Resource/Audio/Future_Monday.mp3"));
		au[1] = new Audio(LoadSoundMem("Resource/Audio/bound.ogg"));
		au[2] = new Audio(LoadSoundMem("Resource/Audio/player.ogg"));
		pl = new Player();
		ba = new Ball();
		li = new Line();
	}

	//�I�u�W�F�N�g��delete���Z�ŏ��������Ƃ��̏���
	//�R���X�g���N�^�Ŏ��̉����������̂�S��delete���Ă���̂�
	//GameControl�̃|�C���^��delete���邾���őS��delete�ł���
	~GameControl() {
		for (int i = 0; BLOCK_NUM; i++) {
			delete bl[i];
		}
		for (int i = 0; 1; i++) {
			delete au[i];
		}
		delete pl;
		delete ba;
		delete li;
	}

	//�^�C�g����ʂł̏���(state=0)
	void Title() {
		DrawGraph(0, 0, titlegh, TRUE);
		if (PushSpace()) {
			state = 1;
			ba->x = 320;
			ba->y = 300;
			au[0]->PlayAudio(2);
		}
	}

	void BallLine() {
		int m_targetPosX = ba->x+ ba->speed * ba->vecX, m_targetPosY = ba->y + ba->speed * ba->vecY;

		while (ba->vecX != 0 && ba->vecY != 0 &&
				m_targetPosX < WINDOW_X && m_targetPosX > 0 &&
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

	//�Q�[���{�҂ł̏���(state=1)
	//����܂Ň@�`�F�ŏ����Ă����Q�[���𓮂������������Ă���
	void Game() {
		startTime = GetNowCount();
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
				ba->vecX = 0;
				ba->vecY = 0;
				state = 3;
			}
		}
		if (ba->vecX != 0 && ba->vecX != 0 && life > 0) {
			//endTime = GetNowCount();
			//if ((startTime - endTime)*1000 == tempo) {
			if(startTime + (tempo * 1000) <= GetNowCount()){
				
				ballCatchMode = false;
				ba->Move();
			}
			if (ballCatchMode) {
				ba->y = pl->y;
				ba->x = pl->x+pl->width/2;
				if (!CheckHitKey(KEY_INPUT_LSHIFT)) {
					ballCatchMode = false;
					ba->vecY = -1;
				}
			}
			else {
				
				if (ba->x + ba->r >= WINDOW_X)ba->vecX = -1;
				if (ba->x - ba->r <= 0)ba->vecX = 1;
				if (ba->y - ba->r <= 0)ba->vecY = 1;
				//�{�[����߂܂���Ƃ��̏���
				if (PushShift() &&ba->x > pl->x && ba->x <pl->x + pl->width && ba->y + ba->r+catchScale > pl->y) {
					ballCatchMode = true;
				}
				//�v���C���[�Ƀ{�[�������������Ƃ��̏���
				if (ba->x > pl->x && ba->x <pl->x + pl->width && ba->y + ba->r >pl->y) {
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
						if (ba->x + ba->r > bl[i]->x && ba->x + ba->r < bl[i]->x + bl[i]->width &&
							ba->y > bl[i]->y && ba->y < bl[i]->y + bl[i]->height) {
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
		}
		else if (life > 0) {
			DrawFormatString(260, 360, GetColor(255, 255, 255), "Space�����񂩂�");
			//if (CheckHitKey(KEY_INPUT_SPACE)) {
			if (PushSpace()) {
				ba->vecX = 1;
				ba->vecY = 1;
			}
		}
		else {
			state = 2;
		}
	}
	
	//�Q�[���I�[�o�[���̏���(state=2)
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

	//�Q�[���N���A���̏���(state=3)
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

	//state�̒l�ɉ����ăQ�[���̏�ʂ�؂�ւ���
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

//main()�Ƒ�̓���
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	

	//�E�B���h�E���[�h
	ChangeWindowMode(TRUE);

	//�E�B���h�E�̃T�C�Y��`���Ń}�N����`�����l�ɂȂ�
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);
	//DX���C�u�������g���̂ɕK�v�B�����G���[�������WinMain()�𔲂��Ă����
	if (DxLib_Init() == -1)return -1;
	//�����Q�[����\������̂ɕK�v�ȗ���ʂ�`�悷�邽�߂̊֐�
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

	//Dx���C�u�������I��������֐�
	DxLib_End();
	//winMain�������I���������Ƃ�����킷
	return 0;
}