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
	int x = 40;
	int y = 40;

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
		DrawCircle(x, y, 10, GetColor(255, 255, 255), TRUE);

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