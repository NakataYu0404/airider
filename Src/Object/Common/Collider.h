#pragma once
#include <DxLib.h>
#include <vector>

class Collider
{

public:

	//	�Ώێ��
	enum class Category
	{
		RAIDER,
		SURVIVOR,
		STAGE,
		LIGHT,
		BATTERY,
		MAGICSHOT,
	};

	//	�Փˎ��
	enum class TYPE
	{
		MODEL,
		CAPSULE,
		SPHERE,
	};

	//	�Փˏ��
	struct Collision_Date
	{
		bool isHit;
		Category tag;
		TYPE type;
		int modelId;
		VECTOR movedPos;
		DxLib::VECTOR normal;
	};

	//	�R���X�g���N�^
	Collider(Category tag, TYPE type, int modelId);

	//	�f�X�g���N�^
	~Collider(void);

	//	�Ώێ��
	Category category_;

	//	�Փˎ��
	TYPE type_;

	//	���f���̃n���h��ID
	int modelId_;

	//	�Փˏ��
	Collision_Date hitInfo_;

	//	���W
	VECTOR pos_;
};
