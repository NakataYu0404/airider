#pragma once
#include <map>
#include <array>
#include <memory>
#include "Common/Transform.h"
class ResourceManager;
class CollisionManager;
class WarpStar;
class Raider;
class Survivor;
class EffectController;
class StageTorch;

class PSMaterial;
class VSMaterial;
class VSModelRenderer;

class Stage : public ActorBase
{

public:

	//	ステージの切り替え間隔
	static constexpr float TIME_STAGE_CHANGE = 1.0f;
	static constexpr float DEFAULT_GRAVITY_POW = 1.0f;

	enum class EFFECT_TYPE
	{
		DUST
	};

	//	ステージ名
	enum class NAME
	{
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_BASE,
		FLAT_PLANET_ROT01,
	};

	//	コンストラクタ
	Stage(void);

	//	デストラクタ
	~Stage(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(bool isDrawRaiderWindow = false, int screenH = -1) override;

	void SetObject(std::weak_ptr<Raider> raider, std::array<std::weak_ptr<Survivor>, 3> survivor);

	void SetTimer(float time);

private:

	static constexpr VECTOR DEFAULT_POS = { 0.0f,-200.0f,0.0f };

	static constexpr VECTOR EFFECT_POS_DUST = { 300,0,-200 };
	static constexpr VECTOR EFFECT_SCL_DUST = { 50,100,50 };

	void InitShader(void);

	//	シェーダーのハンドル
	int ShaderPixHdl_;
	int ShaderVertHdl_;

	void BornTorch(void);

	void EffectInit(void);

	void StartEffect(EFFECT_TYPE type);

	void SetParam(void) override;

	std::weak_ptr<Raider> raider_;
	std::array<std::weak_ptr<Survivor>,3> survivors_;

	std::shared_ptr<Transform> stageTran_;
	std::vector<std::shared_ptr<StageTorch>> torch_;

	int EffDustId_;
	int EffPlayDustId_;

	std::shared_ptr<EffectController> effectController_;

	std::vector<VECTOR> torchPos_;

	//	ポイントライト
	std::shared_ptr<PSMaterial> psPLightMaterial_;
	std::shared_ptr<VSMaterial> vsPLightMaterial_;
	std::shared_ptr<VSModelRenderer> vsPLightRenderer_;

	float totalTime_;

	VECTOR cameraDir_;
};
