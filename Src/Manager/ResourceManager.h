#pragma once
#include <map>
#include <string>
#include "../Common/Vector2.h"
#include "Resource.h"

class ResourceManager
{

public:

	//	リソース名
	enum class SRC
	{
		IMG_NONE,
		MDL_NONE,
		EFF_NONE,

		IMG_TITLE,

		IMG_TITLE_MINNNA,
		IMG_TITLE_HITORI,

		IMG_PUSHSPACE,
		IMG_PLAYERSHADOW,

		IMG_RAIDER_OPERATE,
		IMG_SURVIVOR_OPERATE,

		MDL_SURVIVOR_1,
		MDL_SURVIVOR_2,
		MDL_SURVIVOR_3,

		MDL_SKYDOME,
		MDL_STAGE,
		MDL_STAGECANDLE,

	};

	//	明示的にインステンスを生成する
	static void CreateInstance(void);

	//	静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	//	初期化
	void Init(void);

	//	解放(シーン切替時に一旦解放)
	void Release(void);

	//	リソースの完全破棄
	void Destroy(void);

	//	リソースのロード
	const Resource& Load(SRC src);

	//	リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	static constexpr int IMG_NUMBER_NUMX = 11;
	static constexpr int IMG_NUMBER_NUMY = 1;
	static constexpr int IMG_NUMBER_SIZEX = 65;
	static constexpr int IMG_NUMBER_SIZEY = 63;

	//	静的インスタンス
	static ResourceManager* instance_;

	//	リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	//	読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	//	デフォルトコンストラクタをprivateにして、
	//	外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	//	内部ロード
	Resource& _Load(SRC src);

};
