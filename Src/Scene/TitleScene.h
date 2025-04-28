#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;


class TitleScene : public SceneBase
{

public:

	//	�R���X�g���N�^
	TitleScene(void);

	//	�f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(bool isDrawRaiderview = false) override;

private:

	enum class InScene
	{
		PUSHKEY,
		PLNUMSELECT,
	};

	SceneManager::PLNUM plnum_;
	InScene inScene_;

	//	�X�J�C�h�[���p
	Transform spaceDomeTran_;

	//	�f��
	Transform planet_;
	Transform movePlanet_;

	//	�L�����N�^�[
	Transform charactor_;

	//	�摜
	int imgTitleHdl_;
	int imgPushHdl_;

	int imgSoloHdl_;
	int imgEveryoneHdl_;

	//	�X�J�C�h�[��(�w�i)
	std::unique_ptr<SkyDome> skyDome_;

	//	�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//	�V�F�[�_�[�̃n���h��
	int ShaderPixHdl_;
	int ShaderVertHdl_;

	FLOAT4 lightDir;
	int lightBuf;

	int cnt_;

	float oldPadStickY_;
};
