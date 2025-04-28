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
	
	//	�Q�[�����^�C�v
	enum class InGameSceneType
	{
		START,
		GAME,
		OVER,
	};

	//	���҃^�C�v
	enum class WinType
	{
		RAIDER,
		SURVIVOR,
		DRAW
	};

	//	�R���X�g���N�^
	GameScene(void);

	//	�f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(bool isDrawRaiderview = false) override;

private:

	std::shared_ptr<PlayerBase> player_;

	//	�V���O���g���Q��
	CollisionManager& colMng_;

	// PS�̃}�e���A���ƃ����_���[(������)
	std::shared_ptr<PSMaterial> psScanMaterial_;
	std::shared_ptr<PSMeshRenderer> psScanRenderer_;
	// PS�̃}�e���A���ƃ����_���[(���m�g�[��)
	std::shared_ptr<PSMaterial> psMonoMaterial_;
	std::shared_ptr<PSMeshRenderer> psMonoRenderer_;

};
