#pragma once
#include <vector>
#include <map>
#include <memory>
//	#include <iostream>
#include <functional>
#include "Transform.h"
#include "../ActorBase.h"
#include "./Collider.h"

using namespace std;

class Transform;
class Collider;
class Capsule;
class Sphere;
class ActorBase;
class Player;

class Shape
{

};

class CollisionManager
{
public:

	//	コリジョン押し戻しの試行回数
	static constexpr int MAX_COLLISION_TRY = 100;
	static constexpr float PUSHBACK_POW_SCALE = 2.0f;

	//	インスタンスの生成
	static void CreateInstance(void);

	//	インスタンスの取得
	static CollisionManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

	//	コライダ追加
	void AddCollider(std::weak_ptr<Collider> collider);

	//	コライダ削除
	void ClearCollider(void);

	//	アクター追加
	void Add(std::shared_ptr<ActorBase> collider);

	//	カプセルとモデルの衝突判定
	Collider::Collision_Date Capsule2Model_Collider_PushBack(const std::weak_ptr<ActorBase> actor, const std::weak_ptr<Transform> transform);

	//	線とコライダの衝突判定(対コライダ、ターゲットの方は当たった処理が必要ないとき)
	DxLib::MV1_COLL_RESULT_POLY Line2Collider_ColInfo(const Collider::Category colCategory,const VECTOR LineTopPos, const VECTOR LineBotPos);

	//	線とコライダの衝突判定（対アクター、ターゲットの方に当たった処理が必要なとき）
	std::vector<std::weak_ptr<ActorBase>> Line2Collider_Actor(const Collider::Category colCategory, const VECTOR LineTopPos, const VECTOR LineBotPos);

	//	元からあるカプセルコライダを使ったカプセルとカプセルの衝突判定
	bool Capsule2_Collider(const std::weak_ptr<Capsule> a, const std::weak_ptr<Capsule> b);

	//	入力したスフィア情報と、元からあるカプセルコライダとの当たり判定
	bool Sphere2Capsule_Collider(const std::weak_ptr<Sphere> a, const std::weak_ptr<Capsule> b);

private:

	//	静的インスタンス
	static CollisionManager* instance_;

	//	当たったかの判定
	bool isAttack = false;

	//	衝突したポリゴンの法線
	DxLib::VECTOR Normal_ = {};

	//	衝突判定に用いられるコライダ
	std::vector<std::weak_ptr<ActorBase>> actors_;
	std::vector<std::weak_ptr<Collider>> colliders_;

	//	衝突した場合の関数
	std::function<void(void)> onCollision_;

	//	衝突判定のための管理
	std::map<Collider::Category, std::vector<Collider::Category>> categoryMap_;

};
