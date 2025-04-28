#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Render/PSMaterial.h"
#include "../Render/PSMeshRenderer.h"
#include "../Object/Common/CollisionManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/Capsule.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player/PlayerBase.h"
#include "../Object/Mob/Victim.h"
#include "GameScene.h"

GameScene::GameScene(void) : colMng_(CollisionManager::GetInstance())
{

}

GameScene::~GameScene(void)
{
	CollisionManager::GetInstance().ClearCollider();
	
}

void GameScene::Init(void)
{
	player_ = std::make_shared<PlayerBase>();
	player_->Init();
}

void GameScene::Update(void)
{
	player_->Update();
}

void GameScene::Draw(bool isDrawRaiderview)
{
	player_->Draw();
}
