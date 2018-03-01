//DX���C�u�����̈��p�w��
#include <DxLib.h>

//main()�Ƒ�̓���
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	///�����܂ł̖`�����L�^���܂����H�l��
	///�y�Q�[���������zC/C++�Ńu���b�N���������yDX���C�u�����z���
	///http://nut-softwaredevelopper.hatenablog.com/entry/2015/04/13/214627

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

	//�D�ł̒ǉ�

	//�u���b�N�̏�Ԃ̊Ǘ�
	bool block[5]{
	true,true,
	true,true,
	true
	};

	//�ǉ������܂�

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

		//�D�ł̒ǉ�

		//�u���b�N�������Ă��邩�E�u���b�N�ɓ����������̏���
		for (int i = 0; i < 5; i++) {
			if (block[i]) {
				//�����Ă���΃u���b�N��\��
				DrawBox(80 + i * 100, 100, 80 + (i + 1) * 100 - 10, 140, GetColor(255, 255, 255), TRUE);
				if (ballPosX > 80 + i * 100 && ballPosX < 80 + (i + 1) * 100 - 10 && ballPosY>100 && ballPosY < 140) {
					//�{�[���̒��˕����ςȂ̂͌�̃p�[�g�ɂďC���\��
					block[i] = false;
					vecX *= -1;
					vecY *= -1;
				}
			}
		}

		//�ǉ������܂�

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
		}
		else {
			//�X�y�[�X�L�[�������ƊJ�n������B
			DrawFormatString(260, 160, GetColor(255, 255, 255), "SPACE�����񂩂�");
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				vecX = 1;
				vecY = -1;
				for (int i = 0; i < 5; i++)block[i] = true;
			}
		}
		//�D�ł̒ǉ�

		//�u���b�N���S�Ė����Ȃ����Ƃ��̏���
		if (!block[0] && !block[1] && !block[2] && !block[3] && !block[4]) {
			DrawFormatString(260, 120, GetColor(255, 255, 255), "�����");
			ballPosX = ballSpawnPosX;
			ballPosY = ballSpawnPosY;
			vecX = 0;
			vecY = 0;
		}

		//�ǉ������܂�

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