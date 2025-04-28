#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;
class EffectController;

class Victim : public ActorBase
{
public:

	//	レイダーが殺した時に得るポイント 100で進化
	static constexpr int POINT_EVOLUTION = 25;

	//	状態
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		END
	};

	//	アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		DEAD,
	};

	enum class EFFECT_TYPE
	{
		POWERUP,
	};

	Victim();
	~Victim();

	void Init() override;
	void Update() override;
	void Draw(bool isDrawRaiderWindow = false, int screenH = -1) override;
	void SetParam() override;

	void OnCollision(std::weak_ptr<Collider> collider) override;
	void OnCollision(Collider::Category category) override;

	bool IsAlive(void);
	void SetAlive(bool);

	void SetPos(VECTOR pos);

private:

	static constexpr VECTOR SCALE = { 2.0f,1.5f,2.0f };
	static constexpr VECTOR EFFECT_SIZE = { 50.0f,10.0f,50.0f };

	static constexpr VECTOR CAPSULE_TOP = { 0.0f, 110.0f, 0.0f };
	static constexpr VECTOR CAPSULE_BOTTOM = { 0.0f, 30.0f, 0.0f };
	static constexpr float CAPSULE_RADIUS = 60.0f;

	static constexpr float ROT_TIME_DIV = 10.0f;


	void EffectInit(void);

	void StartEffect(EFFECT_TYPE type);

	//	状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//	更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);

	//	描画系
	void DrawShadow(void);

	std::shared_ptr<EffectController> effectController_;

	int EffpowerupId_;

	//	生きてるか
	bool isAlive_;

	//	アニメーション
	std::shared_ptr<AnimationController> animationController_;

	//	状態管理
	STATE state_;

	//	丸影
	int imgShadowHdl_;

	int cnt_;
};

