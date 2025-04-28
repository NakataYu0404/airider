#pragma once
#include <map>
#include <vector>
#include <array>
#include <memory>
#include "../ActorBase.h"
class AnimationController;
class Collider;
class Capsule;
class CollisionManager;


class PlayerBase : public ActorBase
{

public:

	//	状態
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		VICTORY,
		END
	};

	static constexpr float DEFAULT_GRAVITY_POW = 10.0f;

	//	コンストラクタ
	PlayerBase(void);

	//	デストラクタ
	~PlayerBase(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(bool isDrawRaiderWindow = false, int screenH = -1)override;

	//	衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	void AddCapsule(std::shared_ptr<Capsule> capsule);

	//	ダメージ受ける
	void Damage(float damage);

	STATE GetState(void);

	void ChangeState(STATE state);

	bool IsMove(void);

	void SetPlayAnimation(int type, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void AnimUpdate(void);

protected:

	VECTOR DIR_F = { 0.0f,0.0f,1.0f };
	VECTOR DIR_B = { 0.0f,0.0f,-1.0f };
	VECTOR DIR_L = { -1.0f,0.0f,0.0f };
	VECTOR DIR_R = { 1.0f,0.0f,0.0f };

	static constexpr float ROT_F = 0.0f;
	static constexpr float ROT_B = 180.0f;
	static constexpr float ROT_L = 270.0f;
	static constexpr float ROT_R = 90.0f;

	static constexpr int MAX_HP = 100;

	static constexpr float DIV_MOVE_CONTROLL = 10.0f;

	static constexpr float CHECK_COL_POW = 20.0f;

	virtual void SetParam(void) override;


	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//	操作
	virtual void ProcessMove(int plNum);

	//	回転
	virtual void SetGoalRotate(double rotRad);
	void CollisionGravity(void);

	void Rotate(void);

	//	衝突判定
	void Collision(void);

	//	自分→何かTransform直線距離(内部で正規化、値を正の数にします)
	float Myself2OtherDistance3D(std::weak_ptr<Transform> toTran);
	float Myself2OtherDistance2D(std::weak_ptr<Transform> toTran);
	VECTOR Myself2OtherDir(std::weak_ptr<Transform> toTran);

	//	回転完了までの時間
	static constexpr float TIME_ROT = 0.6f;

	//	アニメーション
	std::shared_ptr<AnimationController> animationController_;

	//	衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;

	//	移動スピード
	float speed_;

	//	状態管理
	STATE state_;

	//	移動方向
	VECTOR moveDir_;

	//	移動量
	VECTOR movePow_;

	//	移動後の座標
	VECTOR movedPos_;

	float gravityPow_;

	//	回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	float rotRad_;


	//	衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	int ForwardKey_;
	int BackKey_;
	int RightKey_;
	int LeftKey_;

	//	シングルトン参照
	CollisionManager& colMng_;

};
