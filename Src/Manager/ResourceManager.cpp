#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_SND = Application::PATH_SOUND;

	Resource* res;
	
	//	タイトル画像
	res = new RES(RES_T::IMG, PATH_IMG + "-1");
	resourcesMap_.emplace(SRC::IMG_NONE, res);
	res = new RES(RES_T::IMG, PATH_MDL + "-1");
	resourcesMap_.emplace(SRC::MDL_NONE, res);
	res = new RES(RES_T::IMG, PATH_EFF + "-1");
	resourcesMap_.emplace(SRC::EFF_NONE, res);

	//	タイトル画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Title.png");
	resourcesMap_.emplace(SRC::IMG_TITLE, res);

	res = new RES(RES_T::IMG, PATH_IMG + "Title/hitoride.png");
	resourcesMap_.emplace(SRC::IMG_TITLE_HITORI, res);

	res = new RES(RES_T::IMG, PATH_IMG + "Title/minnnade.png");
	resourcesMap_.emplace(SRC::IMG_TITLE_MINNNA, res);

	//	PushSpace
	res = new RES(RES_T::IMG, PATH_IMG + "Title/PushSpace.png");
	resourcesMap_.emplace(SRC::IMG_PUSHSPACE, res);

	res = new RES(RES_T::IMG, PATH_IMG + "raiderSousa.png");
	resourcesMap_.emplace(SRC::IMG_RAIDER_OPERATE, res);
	res = new RES(RES_T::IMG, PATH_IMG + "survivorSousa.png");
	resourcesMap_.emplace(SRC::IMG_SURVIVOR_OPERATE, res);

	
	//	プレイヤー1
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Survivor/1.mv1");
	resourcesMap_.emplace(SRC::MDL_SURVIVOR_1, res);
	//	プレイヤー2
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Survivor/2.mv1");
	resourcesMap_.emplace(SRC::MDL_SURVIVOR_2, res);
	//	プレイヤー3
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Survivor/3.mv1");
	resourcesMap_.emplace(SRC::MDL_SURVIVOR_3, res);


	//	プレイヤー影
	res = new RES(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::IMG_PLAYERSHADOW, res);

	//	スカイドーム
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::MDL_SKYDOME, res);

	//	stage
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/stage/stagenocandle5.mv1");
	resourcesMap_.emplace(SRC::MDL_STAGE, res);

	//	stage
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/stage/candle.mv1");
	resourcesMap_.emplace(SRC::MDL_STAGECANDLE, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	//	ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	//	リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		//	登録されていない
		return dummy_;
	}

	//	ロード処理
	rPair->second->Load();

	//	念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
