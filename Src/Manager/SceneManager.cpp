#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	//	�J����
	camera_ = new Camera();
	camera_->Init();

	isSceneChanging_ = false;

	//	�f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//	3D�p�̐ݒ�
	Init3D();

	//	�����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{

	//	�w�i�F�ݒ�
	SetBackgroundColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z);

	//	Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	//	Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	//	�o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	//	���C�g�̐ݒ�
	SetUseLighting(true);
	
	//	���C�g�̐ݒ�
	ChangeLightTypeDir(INIT_LIGHT_DIR);

	//	�t�H�O�ݒ�
	SetFogEnable(true);
	SetFogColor(FOG_COLOR.x, FOG_COLOR.y, FOG_COLOR.z);
	SetFogStartEnd(FOG_START, FOG_END);
	
}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	//	�f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / NANOSECOND_TO_SECONDS);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	//	�J�����X�V
	camera_->Update();

}

void SceneManager::Draw(void)
{

	// ���C���E�B���h�E�ƒǉ��E�B���h�E�̕`��^�[�Q�b�g���Z�b�g
	HWND windows[] = { NULL, subWindowH_ };

	for (HWND hwnd : windows)
	{
		if (hwnd == subWindowH_ && !subWindowH_)
		{
			continue;
		}

		SetDrawScreen(mainScreen_); // �o�b�N�o�b�t�@��`��^�[�Q�b�g�ɐݒ�
		if (hwnd)
		{
			SetScreenFlipTargetWindow(hwnd); // �ǉ��E�B���h�E���^�[�Q�b�g�ɐݒ�
		}
		else
		{
			SetScreenFlipTargetWindow(NULL); // ���C���E�B���h�E���^�[�Q�b�g�ɐݒ�
		}

		ClearDrawScreen();
		camera_->SetBeforeDraw();
		UpdateEffekseer3D();

		if (hwnd == subWindowH_)
		{
			// �ǉ��E�B���h�E�p�̕`��i�Ⴆ�΁A�G�l�~�[��`�悵�Ȃ��j
			scene_->Draw(false); // �����ŃG�l�~�[�`��𐧌�
			fader_->Draw(false);
		}
		else
		{
			// ���C���E�B���h�E�p�̕`��i�Ⴆ�΁A�G�l�~�[��`�悷��j
			scene_->Draw(true); // �����ŃG�l�~�[�`��𐧌�
			fader_->Draw(true);
		}

		camera_->Draw();

		if (sceneId_ != SCENE_ID::GAME)
		{
			//	�Q�[���V�[���̓X�N���[�������������āA���ꂼ���Z�o�b�t�@�ɏ������ޕK�v������̂Ńn���h���𖾎����ăN���X���Ƃɍs��
			DrawEffekseer3D();
		}


		ScreenFlip();
	}

	//	�w�ʃX�N���[���Ƀ��C���X�N���[����`��
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen_, true);
	ScreenFlip();
}

void SceneManager::Destroy(void)
{

	if (scene_ != nullptr)
	{
		delete scene_;
	}

	delete fader_;
	delete camera_;

	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	//	�t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	//	�J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	//	�t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

void SceneManager::SetSubWindowH(HWND hdl)
{
	subWindowH_ = hdl;
}

int SceneManager::GetMainScreen(void)
{
	return mainScreen_;
}

void SceneManager::SetPlayerNum(PLNUM plNum)
{
	plNum_ = plNum;
}

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	//	�f���^�^�C��
	deltaTime_ = INIT_DELTATIME;

	camera_ = nullptr;

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = INIT_DELTATIME;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	//	���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	//	�V�[����ύX����
	sceneId_ = sceneId;

	//	���݂̃V�[�������
	if (scene_ != nullptr)
	{
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	}

	scene_->SetPlayerNum(plNum_);
	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		//	���]��
		if (fader_->IsEnd())
		{
			//	���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		//	�Ó]��
		if (fader_->IsEnd())
		{
			//	���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
			//	�Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}
