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

	//	�R���W���������߂��̎��s��
	static constexpr int MAX_COLLISION_TRY = 100;
	static constexpr float PUSHBACK_POW_SCALE = 2.0f;

	//	�C���X�^���X�̐���
	static void CreateInstance(void);

	//	�C���X�^���X�̎擾
	static CollisionManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

	//	�R���C�_�ǉ�
	void AddCollider(std::weak_ptr<Collider> collider);

	//	�R���C�_�폜
	void ClearCollider(void);

	//	�A�N�^�[�ǉ�
	void Add(std::shared_ptr<ActorBase> collider);

	//	�J�v�Z���ƃ��f���̏Փ˔���
	Collider::Collision_Date Capsule2Model_Collider_PushBack(const std::weak_ptr<ActorBase> actor, const std::weak_ptr<Transform> transform);

	//	���ƃR���C�_�̏Փ˔���(�΃R���C�_�A�^�[�Q�b�g�̕��͓��������������K�v�Ȃ��Ƃ�)
	DxLib::MV1_COLL_RESULT_POLY Line2Collider_ColInfo(const Collider::Category colCategory,const VECTOR LineTopPos, const VECTOR LineBotPos);

	//	���ƃR���C�_�̏Փ˔���i�΃A�N�^�[�A�^�[�Q�b�g�̕��ɓ��������������K�v�ȂƂ��j
	std::vector<std::weak_ptr<ActorBase>> Line2Collider_Actor(const Collider::Category colCategory, const VECTOR LineTopPos, const VECTOR LineBotPos);

	//	�����炠��J�v�Z���R���C�_���g�����J�v�Z���ƃJ�v�Z���̏Փ˔���
	bool Capsule2_Collider(const std::weak_ptr<Capsule> a, const std::weak_ptr<Capsule> b);

	//	���͂����X�t�B�A���ƁA�����炠��J�v�Z���R���C�_�Ƃ̓����蔻��
	bool Sphere2Capsule_Collider(const std::weak_ptr<Sphere> a, const std::weak_ptr<Capsule> b);

private:

	//	�ÓI�C���X�^���X
	static CollisionManager* instance_;

	//	�����������̔���
	bool isAttack = false;

	//	�Փ˂����|���S���̖@��
	DxLib::VECTOR Normal_ = {};

	//	�Փ˔���ɗp������R���C�_
	std::vector<std::weak_ptr<ActorBase>> actors_;
	std::vector<std::weak_ptr<Collider>> colliders_;

	//	�Փ˂����ꍇ�̊֐�
	std::function<void(void)> onCollision_;

	//	�Փ˔���̂��߂̊Ǘ�
	std::map<Collider::Category, std::vector<Collider::Category>> categoryMap_;

};
