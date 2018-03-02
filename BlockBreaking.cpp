//DX���C�u�����̈��p�w��
#include <DxLib.h>
///�����܂ł̖`�����L�^���܂����H�l��
///�y�Q�[���������zC/C++�Ńu���b�N���������yDX���C�u�����z���
///http://nut-softwaredevelopper.hatenablog.com/entry/2015/04/13/214627

//�E�ł̒ǉ�

//�u���b�N�ɂ��ẴN���X
class Block {
//�O������Q�Ƃł���悤�ɂ��邽��public
public:

	//�u���b�N�̍��W
	int blockX, blockY;
	//�u���b�N�̃T�C�Y
	int width, height;
	//�u���b�N�̐������
	bool live;

	//�u���b�N���̂������Ă����ꍇ�`����s���֐�
	void View() {
		if (live) {
			DrawBox(blockX,blockY,blockX+width,blockY+height,GetColor(255,255,255),TRUE);
		}
	}


	//�{�[���Ȃǂ��u���b�N�ɓ�����������鏈��
	void liveControl(int targetX, int targetY, int targetVec) {
		if (targetX > blockX && targetX<blockX + width && targetY > blockY && targetY < blockY && blockY < blockY + height) {
			live = false;
		}
		if (targetVec = -0 && CheckHitKey(KEY_INPUT_SPACE)) {
			live = true;
		}
	}

	//�u���b�N��z�u����֐�
	Block(int setX, int setY) {
		blockX = setX;
		blockY = setY;
		width = 90;
		height = 20;
		live = true;
	}
};
//�ǉ������܂�

//main()�Ƒ�̓���
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	

	//�E�B���h�E���[�h
	ChangeWindowMode(TRUE);
	//DX���C�u�������g���̂ɕK�v�B�����G���[�������WinMain()�𔲂��Ă����
	if (DxLib_Init() == -1)return -1;
	//�����Q�[����\������̂ɕK�v�ȗ���ʂ�`�悷�邽�߂̊֐�
	SetDrawScreen(DX_SCREEN_BACK);

	//�v���C���[�o�[�̐F{red,green,blue}
	const int playerColor[3]{
		100100,100
	};

	//�{�[���̓���������ϐ��ŕ\���BvecX��+1�A�E-1�ō��BVecY��+1�ŉ��A-1�ŏ�B
	//���񂩂�X�y�[�X�L�[�������Ƃ��̒l���ς��A�{�[���������o���d�g�݂ɕύX����B
	int vecX = 0, vecY = 0;
	

	//�{�[���̈ړ��X�s�[�h
	const int ballSpeed = 5;

	//��ʃT�C�Y
	const int windowSizeX = 640, windowSizeY = 480;

	//�v���C���[�o�[�̃T�C�Y
	int playerSizeX = 120, playerSizeY = 20;

	//�v���C���[�o�[�̏����ʒu
	const int playerBarInitialX = windowSizeX /2 - playerSizeX/2, playerBarInitialY = 400;

	//�v���C���[�o�[�̈ʒu
	int playerBarX = playerBarInitialX, playerBarY = playerBarInitialY;

	//�{�[���̃X�|�[���ʒu
	const int ballSpawnPosX = windowSizeX / 2, ballSpawnPosY = playerBarInitialY-50;

	//�{�[���̍��W
	int ballPosX = ballSpawnPosX, ballPosY = ballSpawnPosY;

	//�{�[���̔��a
	const int ballRadius = 10;

	//�{�[���̐F{red,green,blue}
	const int ballColor[3]{
		255,255,255
	};

	//�u���b�N��ϐ��i�z��j�Ƃ��ĊǗ�
	Block* bl[20];
	for (int i = 0; i < 20; i++) {
		//�z�u�������ꏊ�̃u���b�N�̍��W����͂���
		bl[i] = new Block(100 + 110 * (i % 4), 40 + 40 * (i / 4));
	}

	//windows�̃A�v�������s����Ƃ��ɂ�ProcessMassage()�Ƃ����֐������I�ɌĂяo���Ȃ���΂Ȃ�Ȃ���
	while (ProcessMessage() != -1) {
		//�Ăяo���ꂽ���_�ł̎��Ԃ��~���b�ŕԂ�
		int startTime = GetNowCount();
		
		//����ʂ�\������֐�
		ScreenFlip();
		//����ʂ�����
		ClearDrawScreen();
		
		//�~�������֐��i�~�̒��S��X���W,Y���W,���a,�F,�h��Ԃ����ۂ��j
		//Get�J���[�͐F�ԍ���ϊ�
		DrawCircle(ballPosX, ballPosY, ballRadius, GetColor(ballColor[0], ballColor[1], ballColor[2]), TRUE);

		//�v���C���[�̕`����s���BDrawBox(���㒸�_��X���W, Y���W, �E�����_X���W, Y���W, �F, �h��Ԃ����ۂ�)
		DrawBox(playerBarX, playerBarY, playerBarX + playerSizeX, playerBarY + playerSizeY, GetColor(playerColor[0], playerColor[1], playerColor[2]), TRUE);

		for (int i = 0; i < 20; i++) {
			bl[i]->View();
		}

		//�{�[���̃X�s�[�h��0�̂Ƃ��͓����Ȃ��̂ŏ�������
		if(vecX != 0&& vecY != 0){
			//�e�����˕Ԃ������
			if (ballPosX > windowSizeX)vecX = -1;
			if (ballPosX < 0)vecX = 1;
			if (ballPosY < 0)vecY = 1;
			if (ballPosY > windowSizeY)vecY = -1;

			//�v���C���[�ƒe���ڐG������e��Y�����̌�����-1�ɂ��Ē��˕Ԃ�
			if (ballPosX > playerBarX && ballPosX < playerBarX + playerSizeX &&
				ballPosY > playerBarY &&ballPosY < playerBarY + playerSizeY) vecY = -1;
			//��ʂ�艺�Ƀ{�[�����o����~�X�Ƃ��Ĉ���
			if (ballPosY > windowSizeY) {
				ballPosX = ballSpawnPosX;
				ballPosY = ballSpawnPosY;
				vecX = 0;
				vecY = 0;
			}
			for (int i = 0;i < 20; i++) {
				//�u���b�N�Ƀ{�[�������������Ƃ��̃{�[���̒��˕�
				if (bl[i]->live) {
					if (ballPosX > bl[i]->blockX && ballPosX < bl[i]->blockX + bl[i]->width &&
						ballPosY + ballRadius > bl[i]->blockY && ballPosY + ballRadius < bl[i]->blockY + bl[i]->height) {//��
						bl[i]->live = false;
						vecY *= -1;
					}
					if (ballPosX > bl[i]->blockX && ballPosX < bl[i]->blockX + bl[i]->width &&
						ballPosY - ballRadius > bl[i]->blockY && ballPosY - ballRadius < bl[i]->blockY + bl[i]->height) {//��
						bl[i]->live = false;
						vecY *= -1;
					}
					if (ballPosX + ballRadius > bl[i]->blockX && ballPosX + ballRadius < bl[i]->blockX + bl[i]->width &&
						ballPosY > bl[i]->blockY && ballPosY < bl[i]->blockY + bl[i]->height) {//��
						bl[i]->live = false;
						vecX *= -1;
					}
					if (ballPosX - ballRadius > bl[i]->blockX && ballPosX - ballRadius < bl[i]->blockX + bl[i]->width &&
						ballPosY > bl[i]->blockY && ballPosY < bl[i]->blockY + bl[i]->height) {//�E
						bl[i]->live = false;
						vecX *= -1;
					}
				}
			}
		}
		else {
			//�X�y�[�X�L�[�������ƊJ�n������B
			DrawFormatString(260, 160, GetColor(255, 100, 255), "SPACE�����񂩂�");
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				vecX = 1;
				vecY = -1;
				for (int i = 0; i<20; i++)bl[i]->live = true;
			}
		}
		//�Q�[���N���A���̏���
		for (int i = 0; i < 20; i++) {
			if (bl[i]->live)break;
			if (i == 19) {
				DrawFormatString(260, 120, GetColor(255,100, 255), "�����");
				ballPosX = 320;
				ballPosY = 240;
				vecX = 0;
				vecY = 0;
			}
		}

		//�e�̍��W�𓮂���
		ballPosX += ballSpeed * vecX;
		ballPosY += ballSpeed * vecY;

		//CheckHitKey()�ŉE�L�[���h������Ă��邩�h�𔻒肵�A������Ă���Ԃ̓v���C���[��X���W��+10����B
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && playerBarX < windowSizeX - playerSizeX) {
			playerBarX += 10;
		}
		//�t�ɍ��L�[��������Ă����ꍇ�A�v���C���[��X���W��-10����B
		if (CheckHitKey(KEY_INPUT_LEFT) == 1 && playerBarX > 0) {
			playerBarX -= 10;
		}

		//�����Ŏw�肷��L�[��������Ă��邩�ǂ����B������Ă����1��Ԃ�
		//����̏ꍇesc����������1��Ԃ��A���[�v�𔲂���
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;

		//startTime�Ɠ����B���[�v��������邲�Ƃɔ������鎞�ԍ����C������̂Ɏg�p
		int endTime = GetNowCount();

		//�~���b�P�ʂŎw�肵�����ԁA�������~����֐�
		//60FPS��1�b��1000�~���b�Ȃ̂�1000/60�~���b��50��A��������1000�~���b�i1�b�j
		//���̒l�Ƀ��[�v���ɔ����������ԍ��������Ɛ��m��1/60�b�Ń��[�v������I���
		WaitTimer(1000 / 60 - (endTime - startTime));
	}
	//Dx���C�u�������I��������֐�
	DxLib_End();
	//winMain�������I���������Ƃ�����킷
	return 0;
}