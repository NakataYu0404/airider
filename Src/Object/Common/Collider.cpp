#include "Collider.h"

Collider::Collider(Category cate, TYPE type, int modelId)
{
	category_ = cate;
	type_ = type;
	modelId_ = modelId;
	pos_ = { 0.0f,0.0f,0.0f };
	hitInfo_ = {};
}

Collider::~Collider(void)
{
}
