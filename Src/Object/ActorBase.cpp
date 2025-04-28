#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "ActorBase.h"

ActorBase::ActorBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
}

ActorBase::~ActorBase(void)
{
	transform_->Release();
}

void ActorBase::OnCollision(std::weak_ptr<Collider> collider)
{
}

void ActorBase::OnCollision(Collider::Category category)
{
}

const std::weak_ptr<Transform> ActorBase::GetTransform(void) const
{
	return transform_;
}

const std::weak_ptr<Capsule> ActorBase::GetCapsule(void) const
{
	return capsule_;
}

const std::weak_ptr<Sphere> ActorBase::GetSphere(void) const
{
	return sphere_;
}

void ActorBase::SetPos(VECTOR pos)
{
	transform_->pos = pos;
	transform_->Update();
}
