#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Common/CollisionManager.h"
#include "Camera.h"

Camera::Camera(void) : colMng_(CollisionManager::GetInstance())
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = DEFAULT_CAMERA_POS;
	targetPos_ = Utility::VECTOR_ZERO;
	followTransform_ = nullptr;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	ChangeMode(MODE::FIXED_POINT);
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	//	�N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW_ZOOMIN:
		SetBeforeDrawZoomIn();
		break;
	case Camera::MODE::FOLLOW_ZOOMOUT:
		SetBeforeDrawZoomOut();
		break;
	}

	//	�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		DEFAULT_TARGET_POS,
		cameraUp_
	);

	//	DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

}

void Camera::Draw(void)
{
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

Camera::MODE Camera::GetMode(void)
{
	return mode_;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	if (mode_ == mode)
	{
		return;
	}

	//	�J�����̏����ݒ�
	if (!(mode_ == MODE::FOLLOW_ZOOMIN && mode == MODE::FOLLOW_ZOOMOUT))
	{
		SetDefault();
		if (followTransform_ != nullptr)
		{
			pos2followVec_ = Utility::DistanceV(pos_, followTransform_->pos);
			initPos2FollowDis_ = Utility::Distance(pos_, followTransform_->pos);
		}
	}

	//	�J�������[�h�̕ύX
	mode_ = mode;

	//	�ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;

	}


}

void Camera::SetDefault(void)
{

	//	�J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	//	�����_
	targetPos_ = Utility::VECTOR_ZERO;

	//	�J�����̏����
	cameraUp_ = Utility::DIR_U;

	angles_.x = Utility::Deg2RadF(DEFAULT_CAMERA_ANGLE.x);
	angles_.y = DEFAULT_CAMERA_ANGLE.y;
	angles_.z = DEFAULT_CAMERA_ANGLE.z;

	rot_ = Quaternion();

}

void Camera::ProcessRot(void)
{
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	pos_ = DEFAULT_CAMERA_POS;
}

void Camera::SetBeforeDrawZoomIn(void)
{
	targetPos_ = followTransform_->pos;
	if (Utility::Distance(pos_, targetPos_) >= ZOOMIN_DISTANCE_MAX)
	{
		pos_ = VAdd(pos_, Utility::VDiv(pos2followVec_, ZOOM_SPEED));
	}
}

void Camera::SetBeforeDrawZoomOut(void)
{
	targetPos_ = followTransform_->pos;

	if (Utility::Distance(pos_, targetPos_) <= initPos2FollowDis_)
	{
		pos_ = VSub(pos_, Utility::VDiv(pos2followVec_, ZOOM_SPEED));
	}
	else
	{
		ChangeMode(MODE::FIXED_POINT);
	}
}

