#pragma once
#include <map>
#include <array>
#include <memory>
class ResourceManager;
class EffectController;

class StageTorch : public ActorBase
{

public:


	enum class EFFECT_TYPE
	{
		TORCH,
		TORCH_DARK,
	};

	//	コンストラクタ
	StageTorch(VECTOR pos,int torchNo = 0);

	//	デストラクタ
	~StageTorch(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(bool isDrawRaiderWindow = false, int screenH = -1) override;

private:

	static constexpr float LOCAL_ADD_HEIGHT = 300.0f;
	static constexpr VECTOR EFFECT_SIZE = { 20.0f,20.0f,20.0f };

	static constexpr int EFFECT_STARTFLAME = 60;
	static constexpr int EFFECT_ENDFLAME = 90;
	static constexpr float EFFECT_SPEED = 0.5f;

	void StartEffect(EFFECT_TYPE type);

	void EffectInit(void);

	void SetParam(void) override;


	int EffTorchId_;
	int EffPlayTorchId_;

	int EffTorchDarkId_;
	int EffPlayTorchDarkId_;

	int torchNo_;

	std::shared_ptr<EffectController> effectController_;

};

