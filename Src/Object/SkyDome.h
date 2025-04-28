#pragma once
#include <map>
#include <memory>
#include "Common/Transform.h"
#include "ActorBase.h"

class Raider;

class SkyDome : public ActorBase
{

public:

	static constexpr float SCALE = 100.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	//	状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	//	コンストラクタ
	SkyDome(void);

	//	デストラクタ
	~SkyDome(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(bool isDrawRaiderWindow = false, int screenH = -1) override;

	void SetPlayer(std::weak_ptr<Raider> rai);

private:

	void SetParam(void) override;

	//	状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateStay(void);
	void ChangeStateFollow(void);

	//	更新ステップ
	void UpdateNone(void);
	void UpdateStay(void);
	void UpdateFollow(void);

	//	状態
	STATE state_;

	std::weak_ptr<Raider> raider_;
};
