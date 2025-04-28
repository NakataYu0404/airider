#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Common/Vector2.h"
class Transform;
class CollisionManager;

class Camera
{

public:

	//	カメラスピード(度)
	static constexpr float SPEED = 1.0f;

	//	カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	//	カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	//	カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f,1700.0f,-600.0f };
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f,100.0f,0.0f };
	static constexpr VECTOR DEFAULT_CAMERA_ANGLE = { 30.0f,0.0f,0.0f };

	//	カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW_ZOOMIN,
		FOLLOW_ZOOMOUT,
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void Draw(void);

	//	カメラ位置
	VECTOR GetPos(void) const;
	//	カメラの操作角度
	VECTOR GetAngles(void) const;
	//	カメラの注視点
	VECTOR GetTargetPos(void) const;

	//	カメラ角度
	Quaternion GetQuaRot(void) const;
	//	X回転を抜いたカメラ角度
	Quaternion GetQuaRotOutX(void) const;
	//	カメラの前方方向
	VECTOR GetForward(void) const;

	//	カメラモードの変更
	void ChangeMode(MODE mode);

	//	追従対象の設定
	void SetFollow(const Transform* follow);

	MODE GetMode(void);

private:

	static constexpr float ZOOMIN_DISTANCE_MAX = 1000.0f;
	static constexpr float ZOOM_SPEED = 30.0f;


	//	カメラを初期位置に戻す
	void SetDefault(void);

	//	カメラ操作
	void ProcessRot(void);

	//	モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawZoomIn(void);
	void SetBeforeDrawZoomOut(void);

	//	カメラが追従対象とするTransform
	const Transform* followTransform_;

	//	カメラモード
	MODE mode_;

	//	カメラの位置
	VECTOR pos_;

	//	カメラ角度(rad)
	VECTOR angles_;

	//	X軸回転が無い角度
	Quaternion rotOutX_;

	//	カメラ角度
	Quaternion rot_;

	//	注視点
	VECTOR targetPos_;

	//	カメラの上方向
	VECTOR cameraUp_;

	Vector2 defaultMousePos_;

	//	シングルトン参照
	CollisionManager& colMng_;

	//	カメラ座標→追従対象のベクトル
	VECTOR pos2followVec_;

	//	初期時点でのカメラ座標→追従対象の距離
	float initPos2FollowDis_;
};

