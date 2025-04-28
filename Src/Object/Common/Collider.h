#pragma once
#include <DxLib.h>
#include <vector>

class Collider
{

public:

	//	対象種別
	enum class Category
	{
		RAIDER,
		SURVIVOR,
		STAGE,
		LIGHT,
		BATTERY,
		MAGICSHOT,
	};

	//	衝突種別
	enum class TYPE
	{
		MODEL,
		CAPSULE,
		SPHERE,
	};

	//	衝突情報
	struct Collision_Date
	{
		bool isHit;
		Category tag;
		TYPE type;
		int modelId;
		VECTOR movedPos;
		DxLib::VECTOR normal;
	};

	//	コンストラクタ
	Collider(Category tag, TYPE type, int modelId);

	//	デストラクタ
	~Collider(void);

	//	対象種別
	Category category_;

	//	衝突種別
	TYPE type_;

	//	モデルのハンドルID
	int modelId_;

	//	衝突情報
	Collision_Date hitInfo_;

	//	座標
	VECTOR pos_;
};
