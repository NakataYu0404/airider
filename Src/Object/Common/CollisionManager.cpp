
#include"../../Utility/Utility.h"
#include"Capsule.h"
#include"Sphere.h"
#include"Collider.h"
#include "CollisionManager.h"

CollisionManager* CollisionManager::instance_ = nullptr;


void CollisionManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new CollisionManager();
	}
	instance_->Init();
}

CollisionManager& CollisionManager::GetInstance(void)
{
	return *instance_;
}

void CollisionManager::Init(void)
{

	std::vector<Collider::Category> tags;

	//	使用するタグを追加する
	tags.clear();
	tags.emplace_back(Collider::Category::SURVIVOR);
	tags.emplace_back(Collider::Category::RAIDER);
	tags.emplace_back(Collider::Category::STAGE);

	//	当たり判定のためのタグ管理
	categoryMap_.emplace(Collider::Category::RAIDER, tags);
	categoryMap_.emplace(Collider::Category::SURVIVOR, tags);


	//	tags.clear();f
	//	tags.emplace_back(Collider::TAG::RAIDER);
	//	categoryMap_.emplace(Collider::TAG::SURVIVOR, tags);

}

void CollisionManager::Update(void)
{

	//	ループを抜けたときに削除するコライダの要素番号を保存する
	std::vector<int> deleteAct;
	deleteAct.clear();

	//	範囲for文を使用していたが、「特定の番号で要素を触りたい」ため、普通のfor文に
	for (int actNum = 0; actNum < actors_.size(); actNum++)
	{
		auto actor = actors_[actNum];

		//	取得したコライダー情報
		auto actorCollider = actor.lock()->GetTransform().lock()->collider_;
		actorCollider->pos_ = actor.lock()->GetTransform().lock()->pos;

		auto actorType = actorCollider->type_;
		auto actorCategory = actorCollider->category_;

		//	現在のコライダが動くものでなければやりなおし
		if ((actorCategory != Collider::Category::RAIDER &&
			actorCategory != Collider::Category::SURVIVOR) ||
			categoryMap_.count(actorCategory) == 0)
		{
			continue;
		}

		std::vector<int> deleteTar;
		deleteTar.clear();

		for (int tarNum = 0; tarNum < actors_.size(); tarNum++)
		{
			auto target = actors_[tarNum];

			//	同じもの同士では判定しない
			if (actor.lock() == target.lock())
			{
				continue;
			}

			//	それ以外のオブジェクト
			//	target:typ,CAPSULE, tag, enemy stage
			auto targetCollider = target.lock()->GetTransform().lock()->collider_;
			auto targetType = targetCollider->type_;
			auto targetCategory = targetCollider->category_;
			
			switch (actorType)
			{
			case Collider::TYPE::MODEL:
				break;
			case Collider::TYPE::CAPSULE:
				switch (targetType)
				{
				case Collider::TYPE::MODEL:		//	カプセル対モデル
				{
					if (targetCategory == Collider::Category::STAGE)
					{
						auto info = Capsule2Model_Collider_PushBack(actor, target.lock()->GetTransform());

						if (info.isHit)
						{
							targetCollider->hitInfo_.isHit = true;
							targetCollider->hitInfo_.normal = info.normal;
							targetCollider->hitInfo_.movedPos = info.movedPos;
							actor.lock()->OnCollision(targetCollider);
						}
						else
						{
							targetCollider->hitInfo_.isHit = false;
							targetCollider->hitInfo_.normal = { 0.0f, 0.0f, 0.0f };
						}
					}

				}
				break;
				case Collider::TYPE::CAPSULE:	//	カプセル対カプセル
					if (Capsule2_Collider(
						actor.lock()->GetCapsule(),
						target.lock()->GetCapsule()))
					{
						actor.lock()->OnCollision(targetCollider);
						target.lock()->OnCollision(actorCollider);

						if (actorCollider->category_ == Collider::Category::BATTERY && targetCollider->category_ == Collider::Category::SURVIVOR)
						{
							deleteAct.push_back(actNum);
						}
						if (actorCollider->category_ == Collider::Category::SURVIVOR && targetCollider->category_ == Collider::Category::BATTERY)
						{
							deleteTar.push_back(tarNum);
						}
					}
					break;
				case Collider::TYPE::SPHERE:	//	カプセル対スフィア
					if (Sphere2Capsule_Collider(
						target.lock()->GetSphere(),
						actor.lock()->GetCapsule()))
					{
						actor.lock()->OnCollision(targetCollider);
						target.lock()->OnCollision(actorCollider);
						deleteTar.push_back(tarNum);
					}
					break;
				}
				break;
			case Collider::TYPE::SPHERE:
				switch (targetType)
				{
				case Collider::TYPE::MODEL:
					break;
				case Collider::TYPE::CAPSULE:
					if (Sphere2Capsule_Collider(
						actor.lock()->GetSphere(),
						target.lock()->GetCapsule()))
					{
						actor.lock()->OnCollision(targetCollider);
						target.lock()->OnCollision(actorCollider);
						deleteAct.push_back(actNum);
					}
					break;
				case Collider::TYPE::SPHERE:
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

		}
		for (auto del : deleteTar)
		{
			actors_.erase(actors_.begin() + del);
		}
	}
	for (auto del : deleteAct)
	{
		actors_.erase(actors_.begin() + del);
	}

}

void CollisionManager::Draw(void)
{
}

void CollisionManager::Destroy(void)
{
	delete instance_;
}

void CollisionManager::Add(std::shared_ptr<ActorBase> collider)
{
	actors_.push_back(collider);
}

void CollisionManager::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void CollisionManager::ClearCollider(void)
{
	colliders_.clear();
	actors_.clear();
	categoryMap_.clear();
}

Collider::Collision_Date CollisionManager::Capsule2Model_Collider_PushBack(const std::weak_ptr<ActorBase> actor, const std::weak_ptr<Transform> transform)
{
	//	capsuleがキャラクターで、transformがステージのことだろう
	//	answerもおそらくstageよりのなんか

	Collider::Collision_Date answer;
	answer.isHit = false;

	answer.movedPos = actor.lock()->GetTransform().lock()->pos;


	//	カプセルを移動させる
	std::shared_ptr<Transform> trans = std::make_shared<Transform>();
	trans->pos = actor.lock()->GetTransform().lock()->pos;
	trans->Update();
	std::shared_ptr<Capsule> cap = std::make_shared<Capsule>(actor.lock()->GetCapsule(), trans);

	//	//	//	カプセルとの衝突判定
	//	for (const auto c : colliders_)
	//	{

		auto hits = MV1CollCheck_Capsule(
			transform.lock()->modelId, -1,
			cap->GetPosTop(), cap->GetPosDown(), cap->GetRadius());

		//	DxLib::MV1_COLL_RESULT_POLY_DIM


		//	衝突した複数のポリゴンと衝突回避するまで、
		//	プレイヤーの位置を移動させる
		for (int i = 0; i < hits.HitNum; i++)
		{

			auto hit = hits.Dim[i];
			//	DxLib::MV1_COLL_RESULT_POLY

			//	地面と異なり、衝突回避位置が不明なため、何度か移動させる
			//	この時、移動させる方向は、移動前座標に向いた方向であったり、
			//	衝突したポリゴンの法線方向だったりする
			for (int tryCnt = 0; tryCnt < MAX_COLLISION_TRY; tryCnt++)
			{

				//	再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
				//	最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
				int pHit = HitCheck_Capsule_Triangle(
					cap->GetPosTop(), cap->GetPosDown(), cap->GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					//	法線の方向にちょっとだけ移動させる
					answer.movedPos = VAdd(answer.movedPos, VScale(hit.Normal, PUSHBACK_POW_SCALE));
					//	カプセルも一緒に移動させる
					trans->pos = answer.movedPos;
					trans->Update();

					answer.isHit = true;
					answer.normal = hit.Normal;


					continue;
				}

				break;
			}
		}

		//	検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
	//	}

	return answer;
}

DxLib::MV1_COLL_RESULT_POLY CollisionManager::Line2Collider_ColInfo(const Collider::Category colCategory, const VECTOR LineTopPos, const VECTOR LineBotPos)
{
	MV1_COLL_RESULT_POLY a;

	if (this == nullptr)
	{
		a.HitFlag = false;
		return a;
	}

	for (auto actor : actors_)
	{
		//	取得したコライダー情報
		//	actor:typ,CAPSULE, tag, player
		auto actorCollider = actor.lock()->GetTransform().lock()->collider_;
		auto actorCategory = actorCollider->category_;

		//	ステージ以外であればループをやり直す
		if (actorCategory != colCategory)
		{
			continue;
		}

		auto hit = MV1CollCheck_Line(actor.lock()->GetTransform().lock()->modelId, -1, LineTopPos, LineBotPos);

		return hit;
	}
}

std::vector<std::weak_ptr<ActorBase>> CollisionManager::Line2Collider_Actor(const Collider::Category colCategory, const VECTOR LineTopPos, const VECTOR LineBotPos)
{

	std::vector<std::weak_ptr<ActorBase>> ret;

	//このコードに問題があって、ひとつのカテゴリーに複数のモデルがあった場合に、ひとつめのモデルの当たり判定だけ取って満足する

	std::shared_ptr<ActorBase> a = nullptr;

	if (this == nullptr)
	{
		ret.push_back(a);
		return ret;
	}

	for (auto actor : actors_)
	{
		//	取得したコライダー情報
		//	actor:typ,CAPSULE, tag, player
		auto actorCollider = actor.lock()->GetTransform().lock()->collider_;
		auto actorCategory = actorCollider->category_;

		//	対象以外であればループをやり直す
		if (actorCategory != colCategory)
		{
			continue;
		}

		auto hit = HitCheck_Line_Sphere(LineTopPos, LineBotPos, actor.lock()->GetCapsule().lock()->GetCenter(), actor.lock()->GetCapsule().lock()->GetRadius());

		if (hit)
		{
			ret.push_back(actor);
		}
	}

	return ret;
}

bool CollisionManager::Capsule2_Collider(const std::weak_ptr<Capsule> a, const std::weak_ptr<Capsule> b)
{
	//	プレイヤーと敵の衝突判定
	if (HitCheck_Capsule_Capsule(a.lock()->GetPosTop(), a.lock()->GetPosDown(), a.lock()->GetRadius(),
									b.lock()->GetPosTop(), b.lock()->GetPosDown(), b.lock()->GetRadius()))
	{
		//	isAttack = true;
		return true;
	}
	else
	{
		isAttack = false;
		return false;
	}

}

bool CollisionManager::Sphere2Capsule_Collider(const std::weak_ptr<Sphere> a, const std::weak_ptr<Capsule> b)
{
	if (HitCheck_Sphere_Capsule(a.lock()->GetPos(), a.lock()->GetRadius(), b.lock()->GetPosTop(), b.lock()->GetPosDown(), b.lock()->GetRadius()))
	{
		return true;
	}

	return false;
}
