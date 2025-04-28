#include <DxLib.h>
#include "../Application.h"
#include "Fader.h"

Fader::STATE Fader::GetState(void) const
{
	return state_;
}

bool Fader::IsEnd(void) const
{
	return isEnd_;
}

void Fader::SetFade(STATE state)
{
	state_ = state;
	if (state_ != STATE::NONE)
	{
		isPreEnd_ = false;
		isEnd_ = false;
	}
}

void Fader::Init(void)
{
	state_ = STATE::NONE;
	alpha_ = 0;
	isPreEnd_ = true;
	isEnd_ = true;

	imgRaiderHdl_ = LoadGraph("Data/Image/Title/obake.png");
	imgSurvivorHdl_ = LoadGraph("Data/Image/Title/ningen.png");
}

void Fader::Update(void)
{

	if (isEnd_)
	{
		return;
	}

	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > MAX_ALPHA)
		{
			//	フェード終了
			alpha_ = MAX_ALPHA;
			if (isPreEnd_)
			{
				//	1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}

		break;

	case STATE::FADE_IN:
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			//	フェード終了
			alpha_ = 0;
			if (isPreEnd_)
			{
				//	1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	default:
		return;
	}

}

void Fader::Draw(bool raiderView)
{

	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawGraph(0, 0, imgSurvivorHdl_, false);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}
