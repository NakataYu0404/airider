#pragma once
#include <memory>
#include <DxLib.h>
class Transform;

class Sphere
{

public:

	//	�f�o�b�O���̊ȈՃJ�v�Z���\���̐F
	static constexpr int COLOR = 0xffffff;
	static constexpr int DIVISION_NUM = 10;

	//	�R���X�g���N�^
	//	�J�v�Z��������I�u�W�F�N�g���w�肷��
	Sphere(std::weak_ptr<Transform> parent);
	Sphere(const std::weak_ptr<Sphere> base, const std::weak_ptr<Transform> parent);

	//	�f�X�g���N�^
	~Sphere(void);

	//	�`��
	void Draw(void);

	//	�eTransform����̑��Έʒu���擾
	VECTOR GetLocalPos(void) const;

	void SetLocalPos(const VECTOR& pos);

	//	���[���h���W���擾
	VECTOR GetPos(void) const;

	//	���a
	float GetRadius(void) const;
	void SetRadius(float radius);

private:

	//	�J�v�Z�����������鑊��
	std::weak_ptr<Transform> transformParent_;

	//	�eTransform����̑��Έʒu
	VECTOR localPos_;

	//	���a
	float radius_;

};
