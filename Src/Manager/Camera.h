#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Common/Vector2.h"
class Transform;
class CollisionManager;

class Camera
{

public:

	//	�J�����X�s�[�h(�x)
	static constexpr float SPEED = 1.0f;

	//	�J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	//	�J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	//	�J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f,1700.0f,-600.0f };
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f,100.0f,0.0f };
	static constexpr VECTOR DEFAULT_CAMERA_ANGLE = { 30.0f,0.0f,0.0f };

	//	�J�������[�h
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

	//	�J�����ʒu
	VECTOR GetPos(void) const;
	//	�J�����̑���p�x
	VECTOR GetAngles(void) const;
	//	�J�����̒����_
	VECTOR GetTargetPos(void) const;

	//	�J�����p�x
	Quaternion GetQuaRot(void) const;
	//	X��]�𔲂����J�����p�x
	Quaternion GetQuaRotOutX(void) const;
	//	�J�����̑O������
	VECTOR GetForward(void) const;

	//	�J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	//	�Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	MODE GetMode(void);

private:

	static constexpr float ZOOMIN_DISTANCE_MAX = 1000.0f;
	static constexpr float ZOOM_SPEED = 30.0f;


	//	�J�����������ʒu�ɖ߂�
	void SetDefault(void);

	//	�J��������
	void ProcessRot(void);

	//	���[�h�ʍX�V�X�e�b�v
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawZoomIn(void);
	void SetBeforeDrawZoomOut(void);

	//	�J�������Ǐ]�ΏۂƂ���Transform
	const Transform* followTransform_;

	//	�J�������[�h
	MODE mode_;

	//	�J�����̈ʒu
	VECTOR pos_;

	//	�J�����p�x(rad)
	VECTOR angles_;

	//	X����]�������p�x
	Quaternion rotOutX_;

	//	�J�����p�x
	Quaternion rot_;

	//	�����_
	VECTOR targetPos_;

	//	�J�����̏����
	VECTOR cameraUp_;

	Vector2 defaultMousePos_;

	//	�V���O���g���Q��
	CollisionManager& colMng_;

	//	�J�������W���Ǐ]�Ώۂ̃x�N�g��
	VECTOR pos2followVec_;

	//	�������_�ł̃J�������W���Ǐ]�Ώۂ̋���
	float initPos2FollowDis_;
};

