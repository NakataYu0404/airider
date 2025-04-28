#pragma once
#include <memory>
#include "Common/Transform.h"
class ResourceManager;
class SceneManager;
class Capsule;
class Sphere;

class ActorBase
{

public:

	//	�R���X�g���N�^
	ActorBase(void);

	//	�f�X�g���N�^
	virtual ~ActorBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(bool isDrawRaiderWindow = false,int screenH = -1) = 0;


	//	�I�u�W�F�N�g���Փˎ��ɕԂ�
	virtual void OnCollision(std::weak_ptr<Collider> collider);

	virtual void OnCollision(Collider::Category category);

	const std::weak_ptr<Transform> GetTransform(void) const;

	//	�Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const;
	const std::weak_ptr<Sphere> GetSphere(void) const;

	virtual void SetPos(VECTOR pos);


protected:
	virtual void SetParam(void) = 0;

	//	�V���O���g���Q��
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	//	���f������̊�{���
	std::shared_ptr<Transform> transform_;

	//	�Փ˔���p�̃J�v�Z��
	std::shared_ptr<Capsule> capsule_;
	std::shared_ptr<Sphere> sphere_;
};
