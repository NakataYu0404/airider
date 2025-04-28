#include <windows.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Object/Common/CollisionManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SHADER = "Data/Shader/";
const std::string Application::PATH_SOUND = "Data/Sound/";

//  追加ウィンドウクラスの名前
const char* szClassNme =
{
    "window1",
};

//  ウィンドウに対するメッセージ処理(クリックだとか、サイズ変更だとか…。)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

void Application::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new Application();
    }
    instance_->Init();
}

Application& Application::GetInstance(void)
{
    return *instance_;
}

void Application::Init(void)
{
    //  パッドの振動有効化
    SetUseJoypadVibrationFlag(true);

    //  ウィンドウ作成、初期化
    InitWindows();

    SetWindowText("Raider");

    SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, INIT_COLORBIT);
    ChangeWindowMode(true);

    SetUseDirect3DVersion(DX_DIRECT3D_11);
    isInitFail_ = false;
    if (DxLib_Init() == -1)
    {
        isInitFail_ = true;
        return;
    }

    //  エフェクシア初期化
    InitEffekseer();

    SetUseDirectInputFlag(true);

    InputManager::CreateInstance();

    ResourceManager::CreateInstance();

    CollisionManager::CreateInstance();

    SceneManager::CreateInstance();
    //  サブウィンドウのハンドルをセット
    SceneManager::GetInstance().SetSubWindowH(hWnd);

    //  メルセンヌツイスターさん
    std::random_device rd;
    gen_ = std::mt19937(rd());
}

void Application::InitWindows(void)
{
    //  ウィンドウの初期設定
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS myProg;

    myProg.style = CS_HREDRAW | CS_VREDRAW;
    myProg.lpfnWndProc = WndProc;
    myProg.cbClsExtra = 0;
    myProg.cbWndExtra = 0;
    myProg.hInstance = hInstance;
    myProg.hIcon = NULL;
    myProg.hCursor = LoadCursor(NULL, IDC_ARROW);
    myProg.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    myProg.lpszMenuName = NULL;

    myProg.lpszClassName = szClassNme;
    if (!RegisterClass(&myProg))
    {
        return;
    }

    //  ウィンドウサイズ
    RECT windowRect = { 0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y };

    //  ウィンドウのサイズを調整
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    //  TODO:ウィンドウをBモニターに出すには、座標を変えるしかないので、ちょうどいいとこに座標を変えることになっているが、なんらかの方法を調べて、自動で位置決定が行われるようにしたい
    //  注意:CreateWindowのとき、出すウィンドウがモニター範囲内の位置にないと、処理がめちゃ重になるタメ注意すること！おそらく全体を30FPSに強制されている？
    hWnd = CreateWindow(
        szClassNme,
        "Survivor",
        WS_OVERLAPPEDWINDOW,
        INIT_SUBSCREEN_POS.x, INIT_SUBSCREEN_POS.y, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL, NULL, hInstance, NULL
    );
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

}

void Application::Run(void)
{
    auto& inputManager = InputManager::GetInstance();
    auto& sceneManager = SceneManager::GetInstance();

    MSG msg;

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        inputManager.Update();
        sceneManager.Update();

        sceneManager.Draw();

    }
}

void Application::Destroy(void)
{
    InputManager::GetInstance().Destroy();
    ResourceManager::GetInstance().Destroy();
    SceneManager::GetInstance().Destroy();
    CollisionManager::GetInstance().Destroy();

    Effkseer_End();

    if (DxLib_End() == -1)
    {
        isReleaseFail_ = true;
    }

    delete instance_;
}

bool Application::IsInitFail(void) const
{
    return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
    return isReleaseFail_;
}

int Application::GetRandomNum(int max)
{
    std::uniform_int_distribution<> distr(0, max);

    return distr(gen_);
}

Application::Application(void)
    : isInitFail_(false), isReleaseFail_(false)
{
}

void Application::InitEffekseer(void)
{
    if (Effekseer_Init(EFFECSEER_MAXPARTICLE) == -1)
    {
        DxLib_End();
    }
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}