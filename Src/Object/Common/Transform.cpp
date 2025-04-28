#include <DxLib.h>
#include "../../Utility/Utility.h"
#include "Transform.h"

Transform::Transform(void)
{
	modelId = -1;

	scl = Utility::VECTOR_ONE;
	rot = Utility::VECTOR_ZERO;
	pos = Utility::VECTOR_ZERO;
	headPos = Utility::VECTOR_ZERO;
	midPos = Utility::VECTOR_ZERO;
	localPos = Utility::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();
	
	collider_ = nullptr;
}

Transform::Transform(int model)
{
	modelId = model;

	scl = Utility::VECTOR_ONE;
	rot = Utility::VECTOR_ZERO;
	pos = Utility::VECTOR_ZERO;
	headPos = Utility::VECTOR_ZERO;
	midPos = Utility::VECTOR_ZERO;
	localPos = Utility::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();

	collider_ = nullptr;
}

Transform::~Transform(void)
{

}

void Transform::Update(void)
{
	localPos = VAdd(pos, localPos);

	//	中心位置を、足元座標と頭座標の1/2とする
	midPos = Utility::VDiv(VAdd(pos, headPos), 2.0f);

	//	大きさ
	matScl = MGetScale(scl);

	//	回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	//	位置
	matPos = MGetTranslate(pos);

	//	行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	//	行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

	//	衝突判定の更新
	if (collider_ != nullptr)
	{
		MV1RefreshCollInfo(modelId);
	}


	
}

void Transform::Release(void)
{
}

void Transform::SetModel(int model)
{
	modelId = model;
}

void Transform::MakeCollider(Collider::Category category,Collider::TYPE type, int modelIdH)
{
	if (modelId == -1 && type == Collider::TYPE::MODEL)
	{
		return;
	}

	if (modelIdH != -1)
	{
		collider_ = std::make_shared<Collider>(category, type, modelIdH);
		int ret = MV1SetupCollInfo(modelIdH, -1, 1, 1, 1);
	}
	else
	{
		collider_ = std::make_shared<Collider>(category, type, modelId);
		int ret = MV1SetupCollInfo(modelId, -1, 1, 1, 1);
	}

}

VECTOR Transform::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(Utility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(Utility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(Utility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& vec) const
{
	return quaRot.PosAxis(vec);
}
