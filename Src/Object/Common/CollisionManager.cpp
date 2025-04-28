
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

	//	�g�p����^�O��ǉ�����
	tags.clear();
	tags.emplace_back(Collider::Category::SURVIVOR);
	tags.emplace_back(Collider::Category::RAIDER);
	tags.emplace_back(Collider::Category::STAGE);

	//	�����蔻��̂��߂̃^�O�Ǘ�
	categoryMap_.emplace(Collider::Category::RAIDER, tags);
	categoryMap_.emplace(Collider::Category::SURVIVOR, tags);


	//	tags.clear();f
	//	tags.emplace_back(Collider::TAG::RAIDER);
	//	categoryMap_.emplace(Collider::TAG::SURVIVOR, tags);

}

void CollisionManager::Update(void)
{

	//	���[�v�𔲂����Ƃ��ɍ폜����R���C�_�̗v�f�ԍ���ۑ�����
	std::vector<int> deleteAct;
	deleteAct.clear();

	//	�͈�for�����g�p���Ă������A�u����̔ԍ��ŗv�f��G�肽���v���߁A���ʂ�for����
	for (int actNum = 0; actNum < actors_.size(); actNum++)
	{
		auto actor = actors_[actNum];

		//	�擾�����R���C�_�[���
		auto actorCollider = actor.lock()->GetTransform().lock()->collider_;
		actorCollider->pos_ = actor.lock()->GetTransform().lock()->pos;

		auto actorType = actorCollider->type_;
		auto actorCategory = actorCollider->category_;

		//	���݂̃R���C�_���������̂łȂ���΂��Ȃ���
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

			//	�������̓��m�ł͔��肵�Ȃ�
			if (actor.lock() == target.lock())
			{
				continue;
			}

			//	����ȊO�̃I�u�W�F�N�g
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
				case Collider::TYPE::MODEL:		//	�J�v�Z���΃��f��
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
				case Collider::TYPE::CAPSULE:	//	�J�v�Z���΃J�v�Z��
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
				case Collider::TYPE::SPHERE:	//	�J�v�Z���΃X�t�B�A
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
	//	capsule���L�����N�^�[�ŁAtransform���X�e�[�W�̂��Ƃ��낤
	//	answer�������炭stage���̂Ȃ�

	Collider::Collision_Date answer;
	answer.isHit = false;

	answer.movedPos = actor.lock()->GetTransform().lock()->pos;


	//	�J�v�Z�����ړ�������
	std::shared_ptr<Transform> trans = std::make_shared<Transform>();
	trans->pos = actor.lock()->GetTransform().lock()->pos;
	trans->Update();
	std::shared_ptr<Capsule> cap = std::make_shared<Capsule>(actor.lock()->GetCapsule(), trans);

	//	//	//	�J�v�Z���Ƃ̏Փ˔���
	//	for (const auto c : colliders_)
	//	{

		auto hits = MV1CollCheck_Capsule(
			transform.lock()->modelId, -1,
			cap->GetPosTop(), cap->GetPosDown(), cap->GetRadius());

		//	DxLib::MV1_COLL_RESULT_POLY_DIM


		//	�Փ˂��������̃|���S���ƏՓˉ������܂ŁA
		//	�v���C���[�̈ʒu���ړ�������
		for (int i = 0; i < hits.HitNum; i++)
		{

			auto hit = hits.Dim[i];
			//	DxLib::MV1_COLL_RESULT_POLY

			//	�n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
			//	���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
			//	�Փ˂����|���S���̖@�������������肷��
			for (int tryCnt = 0; tryCnt < MAX_COLLISION_TRY; tryCnt++)
			{

				//	�ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
				//	�ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
				int pHit = HitCheck_Capsule_Triangle(
					cap->GetPosTop(), cap->GetPosDown(), cap->GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					//	�@���̕����ɂ�����Ƃ����ړ�������
					answer.movedPos = VAdd(answer.movedPos, VScale(hit.Normal, PUSHBACK_POW_SCALE));
					//	�J�v�Z�����ꏏ�Ɉړ�������
					trans->pos = answer.movedPos;
					trans->Update();

					answer.isHit = true;
					answer.normal = hit.Normal;


					continue;
				}

				break;
			}
		}

		//	���o�����n�ʃ|���S�����̌�n��
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
		//	�擾�����R���C�_�[���
		//	actor:typ,CAPSULE, tag, player
		auto actorCollider = actor.lock()->GetTransform().lock()->collider_;
		auto actorCategory = actorCollider->category_;

		//	�X�e�[�W�ȊO�ł���΃��[�v����蒼��
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

	//���̃R�[�h�ɖ�肪�����āA�ЂƂ̃J�e�S���[�ɕ����̃��f�����������ꍇ�ɁA�ЂƂ߂̃��f���̓����蔻�肾������Ė�������

	std::shared_ptr<ActorBase> a = nullptr;

	if (this == nullptr)
	{
		ret.push_back(a);
		return ret;
	}

	for (auto actor : actors_)
	{
		//	�擾�����R���C�_�[���
		//	actor:typ,CAPSULE, tag, player
		auto actorCollider = actor.lock()->GetTransform().lock()->collider_;
		auto actorCategory = actorCollider->category_;

		//	�ΏۈȊO�ł���΃��[�v����蒼��
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
	//	�v���C���[�ƓG�̏Փ˔���
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
