#pragma once
#include "../Manager/SceneManager.h"

class ResourceManager;

class SceneBase
{

public:

	//	�R���X�g���N�^
	SceneBase(void);

	//	�f�X�g���N�^
	virtual ~SceneBase(void) = 0;

	//	����������
	virtual void Init(void) = 0;

	//	�X�V�X�e�b�v
	virtual void Update(void) = 0;

	//	�`�揈��(�Q�[���V�[�����H�A�Q�[���V�[���Ȃ�A���C�_�[���̕`���ʂ��H)
	virtual void Draw(bool isDrawRaiderview = false) = 0;

	void SetPlayerNum(SceneManager::PLNUM num);

protected:

	//	���\�[�X�Ǘ�
	ResourceManager& resMng_;

	SceneManager::PLNUM plNum_;
};
