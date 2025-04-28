
#include <vector>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Common/CollisionManager.h"
#include "Common/EffectController.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "StageTorch.h"
#include "Stage.h"

StageTorch::StageTorch(VECTOR pos, int torchNo)
{
	transform_ = std::make_shared<Transform>();

	transform_->SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::MDL_STAGECANDLE));

	transform_->scl = {1.0f,1.0f,1.0f};
	transform_->quaRot = Quaternion();
	transform_->pos = pos;
	transform_->localPos.y += LOCAL_ADD_HEIGHT;
	transform_->Update();

	torchNo_ = torchNo;
}

StageTorch::~StageTorch(void)
{
	
}

void StageTorch::Init(void)
{
	EffectInit();
}

void StageTorch::Update(void)
{
	effectController_->Update();
}

void StageTorch::Draw(bool isDrawRaiderWindow, int screenH)
{
	MV1DrawModel(transform_->modelId);
	effectController_->Draw();
}

void StageTorch::StartEffect(EFFECT_TYPE type)
{
	switch (type)
	{
	case StageTorch::EFFECT_TYPE::TORCH_DARK:
		effectController_->Play((int)EFFECT_TYPE::TORCH_DARK, true, EFFECT_STARTFLAME + torchNo_, EFFECT_ENDFLAME, EFFECT_SPEED);
		effectController_->TransUpdate((int)EFFECT_TYPE::TORCH_DARK, transform_->localPos, EFFECT_SIZE, { 0,0,0 });
		break;
	default:
		break;
	}
}

void StageTorch::EffectInit(void)
{
	//effectController_ = std::make_shared<EffectController>();

	//EffTorchId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::).handleId_;

	//effectController_->Add((int)EFFECT_TYPE::TORCH_DARK, EffTorchId_);

	//StartEffect(EFFECT_TYPE::TORCH_DARK);
}

void StageTorch::SetParam(void)
{
}
