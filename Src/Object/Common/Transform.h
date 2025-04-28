#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "Collider.h"

//	/	<summary>
//	/	モデル制御の基本情報
//	/	大きさ：VECTOR基準
//	/	回転　：Quaternion基準
//	/	位置　：VECTOR基準
//	/	</summary>
class Transform
{

public:

	//	コンストラクタ
	Transform(void);
	Transform(int model);

	//	デストラクタ
	~Transform(void);

	void Update(void);
	
	void Release(void);

	//	モデルハンドルをセット
	void SetModel(int modelHId);

	//	コライダ作成
	void MakeCollider(Collider::Category category, Collider::TYPE type,int modelIdH = -1);

	//	前方方向を取得
	VECTOR GetForward(void) const;

	//	後方方向を取得
	VECTOR GetBack(void) const;

	//	右方向を取得
	VECTOR GetRight(void) const;

	//	左方向を取得
	VECTOR GetLeft(void) const;

	//	上方向を取得
	VECTOR GetUp(void) const;

	//	下方向を取得
	VECTOR GetDown(void) const;

	//	対象方向を取得
	VECTOR GetDir(const VECTOR& vec) const;

	//	モデルのハンドルID
	int modelId;

	//	大きさ
	VECTOR scl;

	//	回転
	VECTOR rot;

	//	位置
	VECTOR pos;

	//	頭位置
	VECTOR headPos;

	//	中心位置
	VECTOR midPos;

	//	ローカル座標
	VECTOR localPos;

	//	行列
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	//	回転
	Quaternion quaRot;

	//	ローカル回転
	Quaternion quaRotLocal;

	//	コライダ
	std::shared_ptr<Collider> collider_;

};
