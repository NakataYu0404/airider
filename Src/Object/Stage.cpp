#include <vector>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "Common/CollisionManager.h"
#include "Common/EffectController.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "../Render/PSMaterial.h"
#include "../Render/VSMaterial.h"
#include "../Render/VSModelRenderer.h"
#include "Stage.h"
#include "StageTorch.h"

Stage::Stage(void)
{
	
}

Stage::~Stage(void)
{
	
}

void Stage::Init(void)
{

	//stageTran_ = std::make_shared<Transform>();

	//stageTran_->SetModel(
	//	resMng_.LoadModelDuplicate(ResourceManager::SRC::MDL_STAGE));
	//stageTran_->scl = { 1.0f,1.0f,1.0f };
	//stageTran_->quaRot = Quaternion();

	//stageTran_->pos = DEFAULT_POS;

	//transform_ = std::make_shared<Transform>();

	//transform_->SetModel(
	//	resMng_.LoadModelDuplicate(ResourceManager::SRC::));
	//transform_->scl = stageTran_->scl;
	//transform_->quaRot = stageTran_->quaRot;
	//transform_->pos = stageTran_->pos;

	////	“–‚½‚è”»’è(ƒRƒ‰ƒCƒ_)ì¬
	//transform_->MakeCollider(Collider::Category::STAGE,Collider::TYPE::MODEL);

	//BornTorch();

	//for (auto t : torch_)
	//{
	//	t->Init();
	//}



	//stageTran_->Update();
	//transform_->Update();

	//EffectInit();
	//InitShader();
}

void Stage::InitShader(void)
{
	//	
	psPLightMaterial_ = std::make_shared<PSMaterial>("LightPixelShader.cso", torch_.size());
	vsPLightMaterial_ = std::make_shared<VSMaterial>( "ORGVS.cso", torch_.size(),stageTran_->modelId);
	vsPLightRenderer_ = std::make_shared<VSModelRenderer>(vsPLightMaterial_, psPLightMaterial_);
	
	vsPLightMaterial_->AddTexture(LoadGraph("Data/Model/Planet/stage/difuses.png"));
	vsPLightMaterial_->AddTexture(LoadGraph("Data/Model/Planet/stage/normals.png"));

	cameraDir_ = SceneManager::GetInstance().GetCamera()->GetForward();

	for (auto& t : torchPos_)
	{
		vsPLightMaterial_->AddConstBuffer(t.x, t.y + 150.0f, t.z, 1.0f);
	}
	//vsPLightMaterial_->AddTexture();

}

void Stage::Update(void)
{
	for (auto t : torch_)
	{
		t->Update();
	}

	effectController_->Update();
}

void Stage::Draw(bool isDrawRaiderWindow, int screenH)
{
	vsPLightRenderer_->Draw(screenH);

	for (auto t : torch_)
	{
		t->Draw();
	}
}

void Stage::SetTimer(float time)
{
	totalTime_ = time;
}

void Stage::BornTorch(void)
{
	torchPos_.push_back({ 8.0f,-205.0f,-150.0f });

	torchPos_.push_back({ 380.0f,-205.0f,-410.0f });

	torchPos_.push_back({ 88.0f,-205.0f,788.0f });
	torchPos_.push_back({ 0.0f,-205.0f,-788.0f });
	torchPos_.push_back({ 700.0f,-205.0f,-788.0f });
	torchPos_.push_back({ 1000.0f,-205.0f,788.0f });
	torchPos_.push_back({ 1350.0f,-205.0f,270.0f });
	torchPos_.push_back({ 1470.0f,-205.0f,-788.0f });

	torchPos_.push_back({ 1360.0f,-205.0f,-200.0f });

	torchPos_.push_back({ 750.0f,-205.0f,60.0f });

	torchPos_.push_back({ 460.0f,-205.0f,500.0f });

	torchPos_.push_back({ -165.0f,-205.0f,350.0f });


	for (int t = 0; t < torchPos_.size(); t++)
	{
		torch_.push_back(std::make_shared<StageTorch>(torchPos_[t], t));
	}

}

void Stage::StartEffect(EFFECT_TYPE type)
{
	switch (type)
	{
	case Stage::EFFECT_TYPE::DUST:
		effectController_->Play((int)EFFECT_TYPE::DUST, true);
		effectController_->TransUpdate((int)EFFECT_TYPE::DUST, EFFECT_POS_DUST, EFFECT_SCL_DUST, { 0,0,0 });
		break;
	default:
		break;
	}
}

void Stage::EffectInit(void)
{
 //	effectController_ = std::make_shared<EffectController>();

	//EffDustId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::).handleId_;

	//effectController_->Add((int)EFFECT_TYPE::DUST, EffDustId_);

	//StartEffect(EFFECT_TYPE::DUST);
}

void Stage::SetParam(void)
{
}
