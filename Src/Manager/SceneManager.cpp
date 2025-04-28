#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	//	カメラ
	camera_ = new Camera();
	camera_->Init();

	isSceneChanging_ = false;

	//	デルタタイム
	preTime_ = std::chrono::system_clock::now();

	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//	3D用の設定
	Init3D();

	//	初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{

	//	背景色設定
	SetBackgroundColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z);

	//	Zバッファを有効にする
	SetUseZBuffer3D(true);

	//	Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	//	バックカリングを有効にする
	SetUseBackCulling(true);

	//	ライトの設定
	SetUseLighting(true);
	
	//	ライトの設定
	ChangeLightTypeDir(INIT_LIGHT_DIR);

	//	フォグ設定
	SetFogEnable(true);
	SetFogColor(FOG_COLOR.x, FOG_COLOR.y, FOG_COLOR.z);
	SetFogStartEnd(FOG_START, FOG_END);
	
}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	//	デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / NANOSECOND_TO_SECONDS);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	//	カメラ更新
	camera_->Update();

}

void SceneManager::Draw(void)
{

	// メインウィンドウと追加ウィンドウの描画ターゲットをセット
	HWND windows[] = { NULL, subWindowH_ };

	for (HWND hwnd : windows)
	{
		if (hwnd == subWindowH_ && !subWindowH_)
		{
			continue;
		}

		SetDrawScreen(mainScreen_); // バックバッファを描画ターゲットに設定
		if (hwnd)
		{
			SetScreenFlipTargetWindow(hwnd); // 追加ウィンドウをターゲットに設定
		}
		else
		{
			SetScreenFlipTargetWindow(NULL); // メインウィンドウをターゲットに設定
		}

		ClearDrawScreen();
		camera_->SetBeforeDraw();
		UpdateEffekseer3D();

		if (hwnd == subWindowH_)
		{
			// 追加ウィンドウ用の描画（例えば、エネミーを描画しない）
			scene_->Draw(false); // 引数でエネミー描画を制御
			fader_->Draw(false);
		}
		else
		{
			// メインウィンドウ用の描画（例えば、エネミーを描画する）
			scene_->Draw(true); // 引数でエネミー描画を制御
			fader_->Draw(true);
		}

		camera_->Draw();

		if (sceneId_ != SCENE_ID::GAME)
		{
			//	ゲームシーンはスクリーンが複数あって、それぞれのZバッファに書き込む必要があるのでハンドルを明示してクラスごとに行う
			DrawEffekseer3D();
		}


		ScreenFlip();
	}

	//	背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen_, true);
	ScreenFlip();
}

void SceneManager::Destroy(void)
{

	if (scene_ != nullptr)
	{
		delete scene_;
	}

	delete fader_;
	delete camera_;

	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	//	フェード処理が終わってからシーンを変える場合もあるため、
	//	遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	//	フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

void SceneManager::SetSubWindowH(HWND hdl)
{
	subWindowH_ = hdl;
}

int SceneManager::GetMainScreen(void)
{
	return mainScreen_;
}

void SceneManager::SetPlayerNum(PLNUM plNum)
{
	plNum_ = plNum;
}

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	//	デルタタイム
	deltaTime_ = INIT_DELTATIME;

	camera_ = nullptr;

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = INIT_DELTATIME;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	//	リソースの解放
	ResourceManager::GetInstance().Release();

	//	シーンを変更する
	sceneId_ = sceneId;

	//	現在のシーンを解放
	if (scene_ != nullptr)
	{
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	}

	scene_->SetPlayerNum(plNum_);
	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		//	明転中
		if (fader_->IsEnd())
		{
			//	明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		//	暗転中
		if (fader_->IsEnd())
		{
			//	完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			//	暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}
