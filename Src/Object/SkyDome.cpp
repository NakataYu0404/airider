#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include "Common/Transform.h"
#include "SkyDome.h"

SkyDome::SkyDome(void)
{

	state_ = STATE::NONE;
	
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{
	transform_ = std::make_shared<Transform>();

	//	���f������̊�{���
	transform_->SetModel(
		resMng_.LoadModelDuplicate(
			ResourceManager::SRC::MDL_SKYDOME));
	transform_->scl = SCALES;
	transform_->pos = Utility::VECTOR_ZERO;
	transform_->quaRot = Quaternion::Euler(
		0.0f, 
		Utility::Deg2RadF(Utility::ROT_HALF_DEG),
		0.0f
	);
	transform_->quaRotLocal = Quaternion();
	transform_->Update();

	//	Z�o�b�t�@����(�˂������΍�)
	MV1SetUseZBuffer(transform_->modelId, false);
	MV1SetWriteZBuffer(transform_->modelId, false);

	//	��ԑJ��
	auto sceneId = scnMng_.GetSceneID();
	ChangeState(STATE::STAY);

}

void SkyDome::Update(void)
{

	//	�X�V�X�e�b�v
	switch (state_)
	{
	case SkyDome::STATE::NONE:
		UpdateNone();
		break;
	case SkyDome::STATE::STAY:
		UpdateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		UpdateFollow();
		break;
	}

}

void SkyDome::Draw(bool isDrawRaiderWindow, int screenH)
{
	MV1DrawModel(transform_->modelId);
}

void SkyDome::SetPlayer(std::weak_ptr<Raider> rai)
{
	raider_ = rai;
}

void SkyDome::SetParam(void)
{
}

void SkyDome::ChangeState(STATE state)
{

	//	��ԕύX
	state_ = state;

	//	�e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case SkyDome::STATE::NONE:
		ChangeStateNone();
		break;
	case SkyDome::STATE::STAY:
		ChangeStateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		ChangeStateFollow();
		break;
	}

}

void SkyDome::ChangeStateNone(void)
{
}

void SkyDome::ChangeStateStay(void)
{
}

void SkyDome::ChangeStateFollow(void)
{
	transform_->Update();
}

void SkyDome::UpdateNone(void)
{
}

void SkyDome::UpdateStay(void)
{
}

void SkyDome::UpdateFollow(void)
{
	transform_->Update();
}
