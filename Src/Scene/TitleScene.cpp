#include <memory>
#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgPushHdl_ = -1;
	imgTitleHdl_ = -1;
	skyDome_ = nullptr;
	animationController_ = nullptr;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	//	�摜�ǂݍ���
	imgTitleHdl_ = resMng_.Load(ResourceManager::SRC::IMG_TITLE).handleId_;
	imgPushHdl_ = resMng_.Load(ResourceManager::SRC::IMG_PUSHSPACE).handleId_;

	imgSoloHdl_ = resMng_.Load(ResourceManager::SRC::IMG_TITLE_HITORI).handleId_;
	imgEveryoneHdl_ = resMng_.Load(ResourceManager::SRC::IMG_TITLE_MINNNA).handleId_;

	//	�w�i
	spaceDomeTran_.pos = Utility::VECTOR_ZERO;
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	float size;

	//	���C���f��
	planet_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MDL_STAGE));
	planet_.pos = Utility::VECTOR_ZERO;
	planet_.scl = Utility::VECTOR_ONE;
	planet_.Update();

	//	��]����f��
	movePlanet_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MDL_STAGE));
	movePlanet_.pos = { -250.0f, -100.0f, -100.0f };
	size = 0.7f;
	movePlanet_.scl = { size, size, size };
	movePlanet_.quaRotLocal = Quaternion::Euler(
		Utility::Deg2RadF(90.0f), 0.0f, 0.0f);
	movePlanet_.Update();

	//	�L����
	charactor_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MDL_SURVIVOR_1));
	charactor_.pos = { -250.0f, -32.0f, -105.0f };
	size = 0.4f;
	charactor_.scl = { size, size, size };
	charactor_.quaRot = Quaternion::Euler(
		0.0f, Utility::Deg2RadF(90.0f), 0.0f);
	charactor_.Update();

	//	�A�j���[�V�����̐ݒ�
	std::string path = Application::PATH_MODEL + "Player/Anim/";
	animationController_ = std::make_unique<AnimationController>(charactor_.modelId);
	animationController_->Add(0, path + "Run.mv1", 20.0f);
	animationController_->Play(0);

	//	��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	ShaderVertHdl_ = LoadVertexShader("./Data/Shader/LightVertexShader.cso");
	ShaderPixHdl_ = LoadPixelShader("./Data/Shader/LighPixelShader.cso");
	cnt_ = 0;
	lightBuf = CreateShaderConstantBuffer(sizeof(float) * 4);


	inScene_ = InScene::PUSHKEY;

	oldPadStickY_ = 0.0f;
}

void TitleScene::Update(void)
{

	//	�V�[���J��
	InputManager& ins = InputManager::GetInstance();

	// �v���C���[�̓��͂����m����
	auto playerCtl = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	float stick = playerCtl.AKeyLY * -1 / 10;

	switch (inScene_)
	{
	case TitleScene::InScene::PUSHKEY:
		if (ins.IsTrgDown(KEY_INPUT_SPACE) || 
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::DOWN))
		{
			inScene_ = InScene::PLNUMSELECT;
		}
		break;
	case TitleScene::InScene::PLNUMSELECT:
		if (ins.IsTrgDown(KEY_INPUT_SPACE) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			SceneManager::GetInstance().SetPlayerNum(plnum_);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
		if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_UP) || (fabsf(stick) >= 0.01 && fabsf(oldPadStickY_) < 0.01))
		{
			switch (plnum_)
			{
			case SceneManager::PLNUM::SOLO:
				plnum_ = SceneManager::PLNUM::EVERYONE;
				break;
			case SceneManager::PLNUM::EVERYONE:
				plnum_ = SceneManager::PLNUM::SOLO;
				break;
			}
		}
		break;
	}

	//	�f���̉�]
	movePlanet_.quaRot = movePlanet_.quaRot.Mult(
		Quaternion::Euler(0.0f, 0.0f, Utility::Deg2RadF(-1.0f)));
	movePlanet_.Update();

	//	�L�����A�j���[�V����
	animationController_->Update();

	skyDome_->Update();

	cnt_++;

	oldPadStickY_ = stick;
}

void TitleScene::Draw(bool isDrawRaiderview)
{
	skyDome_->Draw();

	MV1DrawModel(planet_.modelId);
	//MV1DrawModel(movePlanet_.modelId);

	MV1DrawModel(charactor_.modelId);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 250, 1.0, 0.0, imgTitleHdl_, true);


	switch (inScene_)
	{
	case TitleScene::InScene::PUSHKEY:

		if (cnt_ % 30 < 20)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, 1.0, 0.0, imgPushHdl_, true);
		}
		break;
	case TitleScene::InScene::PLNUMSELECT:
		switch (plnum_)
		{
		case SceneManager::PLNUM::SOLO:
			DrawGraph(0, 0, imgSoloHdl_, true);
			break;
		case SceneManager::PLNUM::EVERYONE:
			DrawGraph(0, 0, imgEveryoneHdl_, true);
			break;
		default:
			break;
		}
		break;
	}

}
