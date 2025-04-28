#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Resource.h"

Resource::Resource(void)
{
	type_ = TYPE::NONE;
	path_ = "";

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(TYPE type, const std::string& path)
{
	type_ = type;
	path_ = path;

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY)
{
	type_ = type;
	path_ = path;
	numX_ = numX;
	numY_ = numY;
	sizeX_ = sizeX;
	sizeY_ = sizeY;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::~Resource(void)
{
}

void Resource::Load(void)
{

	switch (type_)
	{
	case Resource::TYPE::IMG:
		//	画像
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::TYPE::IMGS:
		//	複数画像
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case Resource::TYPE::MODEL:
		//	モデル
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:

		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;
	case Resource::TYPE::SOUND:
		handleId_ = LoadSoundMem(path_.c_str());
		break;
	}

}

void Resource::Release(void)
{
	switch (type_)
	{
	case Resource::TYPE::IMG:
		DeleteGraph(handleId_);
		handleId_ = -1;  // 解放後にハンドルを無効化
		break;

	case Resource::TYPE::IMGS:
	{
		if (handleIds_ == nullptr)
		{
			return;
		}

		int num = numX_ * numY_;
		bool hasValidHandle = false;
		for (int i = 0; i < num; i++)
		{
			if (handleIds_[i] > 0)
			{
				hasValidHandle = true;
				break;
			}
		}

		if (!hasValidHandle)
		{
			return;
		}

		for (int i = 0; i < num; i++)
		{
			if (handleIds_[i] > 0)
			{
				DeleteGraph(handleIds_[i]);
				handleIds_[i] = -1;  // 解放後にハンドルを無効化
			}
		}
		delete[] handleIds_;
		handleIds_ = nullptr;  // 解放後にポインタをnullptrに設定
	}
	break;

	case Resource::TYPE::MODEL:
	{
		MV1DeleteModel(handleId_);
		handleId_ = -1;  // 解放後にハンドルを無効化
		auto ids = duplicateModelIds_;
		for (auto id : ids)
		{
			MV1DeleteModel(id);
		}
		duplicateModelIds_.clear();  // 解放後にリストをクリア
	}
	break;

	case Resource::TYPE::EFFEKSEER:
		DeleteEffekseerEffect(handleId_);
		handleId_ = -1;  // 解放後にハンドルを無効化
		break;

	case Resource::TYPE::SOUND:
		DeleteSoundMem(handleId_);
		handleId_ = -1;  // 解放後にハンドルを無効化
		break;
	}
}
void Resource::CopyHandle(int* imgs)
{

	if (handleIds_ == nullptr)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}

}
