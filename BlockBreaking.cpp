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

	//�~(�{�[��)�̒��S
	int ballPosX = 40, ballPosY = 40;

	//�{�[���̔��a
	const int ballRadius = 10;

	//�{�[���̐F{red,green,blue}
	const int ballColor[3]{
		255,255,255
	};

	//�v���C���[�o�[�̐F{red,green,blue}
	const int playerColor[3]{
		100100,100
	};

	//�{�[���̓���������ϐ��ŕ\���BvecX��+1�A�E-1�ō��BVecY��+1�ŉ��A-1�ŏ�B
	int vecX = 1, vecY = 1;

	//�{�[���̈ړ��X�s�[�h
	const int ballSpeed = 5;

	//��ʃT�C�Y
	const int windowSizeX = 640, windowSizeY = 480;

	//����B�ł̒ǉ�

	//�v���C���[�o�[�̏����ʒu
	int playerBarX = 320, playerBarY = 400;

	//�v���C���[�o�[�̃T�C�Y
	int playerSizeX = 60, playerSizeY = 20;

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

		//����B�ł̒ǉ�

		//�v���C���[�̕`����s���BDrawBox(���㒸�_��X���W, Y���W, �E�����_X���W, Y���W, �F, �h��Ԃ����ۂ�)
		DrawBox(playerBarX, playerBarY, playerBarX + playerSizeX, playerBarY + playerSizeY, GetColor(playerColor[0], playerColor[1], playerColor[2]), TRUE);

		//�ǉ������܂�

		//����B�ł̒ǉ�

		//�e�����˕Ԃ������
		if (ballPosX > windowSizeX)vecX = -1;
		if (ballPosX < 0)vecX = 1;
		if (ballPosY < 0)vecY = 1;
		if (ballPosY > windowSizeY)vecY = -1;


		//����B�ł̒ǉ�

		//�v���C���[�ƒe���ڐG������e��Y�����̌�����-1�ɂ��Ē��˕Ԃ�
		if( ballPosX > playerBarX && ballPosX<playerBarX + playerSizeX && ballPosY > playerBarY) vecY = -1;

		//�ǉ������܂�

		//�e�̍��W�𓮂���
		ballPosX += ballSpeed * vecX;
		ballPosY += ballSpeed * vecY;

		//����B�̒ǉ�

		//CheckHitKey()�ŉE�L�[���h������Ă��邩�h�𔻒肵�A������Ă���Ԃ̓v���C���[��X���W��+10����B
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			playerBarX += 10;
		}
		//�t�ɍ��L�[��������Ă����ꍇ�A�v���C���[��X���W��-10����B
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			playerBarX -= 10;
		}
		
		//�ǉ������܂�

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