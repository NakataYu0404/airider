#include "PlayerBase.h"
#include <string>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Common/CollisionManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void) : colMng_(CollisionManager::GetInstance())
{
	SetParam();
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::Init(void)
{
	transform_ = std::make_shared<Transform>();
	transform_->modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::MDL_SURVIVOR_1);
	speed_ = 1;
}

void PlayerBase::Update(void)
{
	ProcessMove(0);
	Rotate();
	transform_->quaRot = playerRotY_;
	transform_->Update();
}

void PlayerBase::Draw(bool isDrawRaiderWindow, int screenH)
{
	MV1DrawModel(transform_->modelId);
}

void PlayerBase::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void PlayerBase::ClearCollider(void)
{
	colliders_.clear();
}

void PlayerBase::AddCapsule(std::shared_ptr<Capsule> capsule)
{
}

void PlayerBase::Damage(float damage)
{
}

PlayerBase::STATE PlayerBase::GetState(void)
{
	return state_;
}

bool PlayerBase::IsMove(void)
{
	if (movePow_.x == 0.0f && movePow_.z == 0.0f)
	{
		return false;
	}
	return true;
}

void PlayerBase::SetPlayAnimation(int type, bool isLoop,
	float startStep, float endStep, bool isStop, bool isForce)
{
	animationController_->Play(type, isLoop, startStep, endStep, isStop, isForce);
}

void PlayerBase::AnimUpdate(void)
{
	animationController_->Update();
}

void PlayerBase::SetParam(void)
{
	ForwardKey_ = KEY_INPUT_W;
	BackKey_ = KEY_INPUT_S;
	RightKey_ = KEY_INPUT_D;
	LeftKey_ = KEY_INPUT_A;

	gravityPow_ = 10.0f;
}

void PlayerBase::ChangeState(STATE state)
{
	//	��ԕύX
	state_ = state;

	//	�e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case PlayerBase::STATE::NONE:
		ChangeStateNone();
		break;
	case PlayerBase::STATE::PLAY:
		ChangeStatePlay();
		break;
	}
}

void PlayerBase::ChangeStateNone(void)
{
}

void PlayerBase::ChangeStatePlay(void)
{
}


void PlayerBase::ProcessMove(int plNum)
{
	auto& ins = InputManager::GetInstance();

	// �v���C���[�̓��͂����m����
	auto playerCtl = ins.GetJPadInputState(static_cast<InputManager::JOYPAD_NO>(plNum+1));

	//	�ړ��ʂ��[��
	movePow_ = Utility::VECTOR_ZERO;

	VECTOR dir = Utility::VECTOR_ZERO;

	dir.x = playerCtl.AKeyLX;
	dir.z = playerCtl.AKeyLY * -1;


	// dir�͕����x�N�g���̂��Ƃ����A�R���g���[���[�̓��͂�10�i�K(N��Ԃ�����)�ɂ����̂�0�`10�ɂȂ��Ă���B�����ł���ȏ�ő�l��1.0f�̕����s�����ǂ��̂ŏ��Z����B
	dir.x /= DIV_MOVE_CONTROLL;
	dir.z /= DIV_MOVE_CONTROLL;

	if (dir.x > 0.1f || ins.IsNew(RightKey_))
	{
		dir.x = 1.0f;
	}
	if (dir.z > 0.1f || ins.IsNew(ForwardKey_))
	{
		dir.z = 1.0f;
	}

	if (dir.x < -0.1f || ins.IsNew(LeftKey_))
	{
		dir.x = -1.0f;
	}
	if (dir.z < -0.1f || ins.IsNew(BackKey_))
	{
		dir.z = -1.0f;
	}

	float stickAngle = atan2f(dir.x, dir.z);

	if (!Utility::EqualsVZero(dir)) {
		dir = Utility::VNormalize(dir);
	}

	//	�v���C���[�����������p�x
	rotRad_ = stickAngle;

	if (!Utility::EqualsVZero(dir)) {

		moveDir_ = dir;
		movePow_ = VScale(dir, speed_);

		transform_->pos.x += movePow_.x;
		transform_->pos.y += movePow_.y;
		transform_->pos.z += movePow_.z;

		//	��]����
		SetGoalRotate(rotRad_ + Utility::Deg2RadF(180.0f));

	}
	else
	{
	}
}

void PlayerBase::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot;
	Quaternion axis;

	cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	axis = Quaternion::AngleAxis(Utility::RadIn2PI((double)cameraRot.y + rotRad), Utility::AXIS_Y);

	//	���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//	�������l
	if (angleDiff > 0.01)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;

}

void PlayerBase::CollisionGravity(void)
{

	//	�W�����v�ʂ����Z
	movedPos_.y -= gravityPow_;

	//	�d�͕���
	VECTOR dirGravity = Utility::DIR_D;

	//	�d�͕����̔���
	VECTOR dirUpGravity = Utility::DIR_U;


	float checkPow = CHECK_COL_POW;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow_));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));

	auto hit = colMng_.GetInstance().Line2Collider_ColInfo(Collider::Category::STAGE, gravHitPosUp_, gravHitPosDown_);

	if (hit.HitFlag > 0)
	{
		//	�Փ˒n�_����A������Ɉړ�
		movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, gravityPow_ * 2.0f));

		gravityPow_ = DEFAULT_GRAVITY_POW;

	}
}

void PlayerBase::Rotate(void)
{

	stepRotTime_ -= scnMng_.GetDeltaTime();

	//	��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void PlayerBase::Collision(void)
{
	//	���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_->pos, movePow_);

	//	�Փ�(�d��)
	CollisionGravity();

	//	�ړ�
	transform_->pos = movedPos_;

}

float PlayerBase::Myself2OtherDistance3D(std::weak_ptr<Transform> toTran)
{
	VECTOR Dif = VSub(transform_->pos, toTran.lock()->pos);
	float DistanceXZ = sqrtf(powf(fabsf(Dif.x), 2) + powf(fabsf(Dif.z), 2));
	float Distance3D = sqrtf(powf(fabsf(DistanceXZ), 2) + powf(fabsf(Dif.y), 2));
	return Distance3D;
}

float PlayerBase::Myself2OtherDistance2D(std::weak_ptr<Transform> toTran)
{
	VECTOR Dif = VSub(transform_->pos, toTran.lock()->pos);
	float DistanceXZ = sqrtf(powf(fabsf(Dif.x), 2) + powf(fabsf(Dif.z), 2));
	return DistanceXZ;
}

VECTOR PlayerBase::Myself2OtherDir(std::weak_ptr<Transform> toTran)
{
	VECTOR ret;

	VECTOR myPos = transform_->pos;
	VECTOR otherPos = toTran.lock()->pos;

	ret = Utility::VNormalize(VSub(otherPos, myPos));
	return ret;
}

