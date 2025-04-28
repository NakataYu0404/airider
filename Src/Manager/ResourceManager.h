#pragma once
#include <map>
#include <string>
#include "../Common/Vector2.h"
#include "Resource.h"

class ResourceManager
{

public:

	//	���\�[�X��
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

	//	�����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	//	�ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	//	������
	void Init(void);

	//	���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	//	���\�[�X�̊��S�j��
	void Destroy(void);

	//	���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	//	���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	static constexpr int IMG_NUMBER_NUMX = 11;
	static constexpr int IMG_NUMBER_NUMY = 1;
	static constexpr int IMG_NUMBER_SIZEX = 65;
	static constexpr int IMG_NUMBER_SIZEY = 63;

	//	�ÓI�C���X�^���X
	static ResourceManager* instance_;

	//	���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource*> resourcesMap_;

	//	�ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	//	�f�t�H���g�R���X�g���N�^��private�ɂ��āA
	//	�O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	//	�������[�h
	Resource& _Load(SRC src);

};
