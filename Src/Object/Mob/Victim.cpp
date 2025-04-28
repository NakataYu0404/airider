#include <Dxlib.h>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/EffectController.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "Victim.h"

Victim::Victim()
{
	animationController_ = nullptr;
}

Victim::~Victim()
{
}

void Victim::Init()
{
	//transform_ = std::make_shared<Transform>();

	////	モデルの基本設定
	//transform_->SetModel(resMng_.LoadModelDuplicate(
	//	ResourceManager::SRC::));
	//transform_->scl = SCALE;
	//transform_->pos = Utility::POS_FARAWAY;
	//transform_->headPos = transform_->pos;
	//transform_->quaRot = Quaternion();
	//transform_->quaRotLocal =
	//	Quaternion::Euler({ 0.0f, Utility::Deg2RadF(Utility::ROT_HALF_DEG), 0.0f });
	//transform_->Update();

	//transform_->MakeCollider(Collider::Category::BATTERY, Collider::TYPE::CAPSULE);

	////	カプセルコライダ
	//capsule_ = std::make_shared<Capsule>(transform_);
	//capsule_->SetLocalPosTop(CAPSULE_TOP);
	//capsule_->SetLocalPosDown(CAPSULE_BOTTOM);
	//capsule_->SetRadius(CAPSULE_RADIUS);

	////	丸影画像
	//imgShadowHdl_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYERSHADOW).handleId_;


	////	初期状態
	//ChangeState(STATE::PLAY);

	//SetParam();
}

void Victim::Update()
{
	if (!isAlive_)
	{
		return;
	}

	//	更新ステップ
	switch (state_)
	{
	case Victim::STATE::NONE:
		UpdateNone();
		break;
	case Victim::STATE::PLAY:
		UpdatePlay();
		break;
	}
	//	モデル制御更新
	transform_->Update();

}

void Victim::Draw(bool isDrawRaiderWindow, int screenH)
{
	if (!isAlive_)
	{
		return;
	}
	
	//	モデルの描画
	MV1DrawModel(transform_->modelId);
	
	effectController_->Draw();
}

void Victim::SetParam()
{
	isAlive_ = true;
	cnt_ = 0;

}

void Victim::OnCollision(std::weak_ptr<Collider> collider)
{
	switch (collider.lock()->category_)
	{
	case Collider::Category::SURVIVOR:
		SetAlive(false);
		break;
	case Collider::Category::RAIDER:
		break;
	case Collider::Category::BATTERY:
		break;
	case Collider::Category::STAGE:
		transform_->pos = collider.lock()->hitInfo_.movedPos;
		transform_->Update();
		break;
	default:
		break;
	}
}

void Victim::OnCollision(Collider::Category category)
{
	switch (category)
	{
	case Collider::Category::RAIDER:
		break;
	case Collider::Category::SURVIVOR:
		break;
	case Collider::Category::STAGE:
		break;
	case Collider::Category::LIGHT:
		break;
	default:
		break;
	}
}

bool Victim::IsAlive(void)
{
	return isAlive_;
}

void Victim::SetAlive(bool alive)
{
	isAlive_ = alive;
}

void Victim::SetPos(VECTOR pos)
{
	transform_->pos = pos;
	transform_->Update();
	EffectInit();
	StartEffect(EFFECT_TYPE::POWERUP);

}

void Victim::EffectInit(void)
{
	//effectController_ = std::make_shared<EffectController>();

	//EffpowerupId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::).handleId_;

	//effectController_->Add((int)EFFECT_TYPE::POWERUP, EffpowerupId_);
}

void Victim::StartEffect(EFFECT_TYPE type)
{
	switch (type)
	{
	case Victim::EFFECT_TYPE::POWERUP:
		effectController_->Play((int)EFFECT_TYPE::POWERUP, false);
		effectController_->TransUpdate((int)EFFECT_TYPE::POWERUP, transform_->pos, EFFECT_SIZE, { 0,0,0 });
		break;
	}
}

void Victim::ChangeState(STATE state)
{
	state_ = state;
}

void Victim::ChangeStateNone(void)
{
}

void Victim::ChangeStatePlay(void)
{
}

void Victim::UpdateNone(void)
{
}

void Victim::UpdatePlay(void)
{
	effectController_->Update();

	transform_->quaRot = Quaternion::AngleAxis(Utility::ROT_QUARTER_DEG, Utility::AXIS_X);
	transform_->quaRot = Quaternion::AngleAxis(cnt_ / ROT_TIME_DIV, transform_->GetForward());

	transform_->Update();
	cnt_++;
}

void Victim::DrawShadow(void)
{

}
