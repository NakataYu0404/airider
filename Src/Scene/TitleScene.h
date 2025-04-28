#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;


class TitleScene : public SceneBase
{

public:

	//	コンストラクタ
	TitleScene(void);

	//	デストラクタ
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

	//	スカイドーム用
	Transform spaceDomeTran_;

	//	惑星
	Transform planet_;
	Transform movePlanet_;

	//	キャラクター
	Transform charactor_;

	//	画像
	int imgTitleHdl_;
	int imgPushHdl_;

	int imgSoloHdl_;
	int imgEveryoneHdl_;

	//	スカイドーム(背景)
	std::unique_ptr<SkyDome> skyDome_;

	//	アニメーション
	std::unique_ptr<AnimationController> animationController_;

	//	シェーダーのハンドル
	int ShaderPixHdl_;
	int ShaderVertHdl_;

	FLOAT4 lightDir;
	int lightBuf;

	int cnt_;

	float oldPadStickY_;
};
