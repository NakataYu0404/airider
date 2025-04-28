#pragma once
#include "../Manager/SceneManager.h"

class ResourceManager;

class SceneBase
{

public:

	//	コンストラクタ
	SceneBase(void);

	//	デストラクタ
	virtual ~SceneBase(void) = 0;

	//	初期化処理
	virtual void Init(void) = 0;

	//	更新ステップ
	virtual void Update(void) = 0;

	//	描画処理(ゲームシーンか？、ゲームシーンなら、レイダー側の描画画面か？)
	virtual void Draw(bool isDrawRaiderview = false) = 0;

	void SetPlayerNum(SceneManager::PLNUM num);

protected:

	//	リソース管理
	ResourceManager& resMng_;

	SceneManager::PLNUM plNum_;
};
