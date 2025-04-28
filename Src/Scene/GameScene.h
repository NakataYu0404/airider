#pragma once
#include <array>
#include "SceneBase.h"
class Stage;
class SkyDome;
class Victim;
class CollisionManager;

class PSMaterial;
class PSMeshRenderer;

class PlayerBase;

class GameScene : public SceneBase
{
public:
	
	//	ゲーム内タイプ
	enum class InGameSceneType
	{
		START,
		GAME,
		OVER,
	};

	//	勝者タイプ
	enum class WinType
	{
		RAIDER,
		SURVIVOR,
		DRAW
	};

	//	コンストラクタ
	GameScene(void);

	//	デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(bool isDrawRaiderview = false) override;

private:

	std::shared_ptr<PlayerBase> player_;

	//	シングルトン参照
	CollisionManager& colMng_;

	// PSのマテリアルとレンダラー(走査線)
	std::shared_ptr<PSMaterial> psScanMaterial_;
	std::shared_ptr<PSMeshRenderer> psScanRenderer_;
	// PSのマテリアルとレンダラー(モノトーン)
	std::shared_ptr<PSMaterial> psMonoMaterial_;
	std::shared_ptr<PSMeshRenderer> psMonoRenderer_;

};
