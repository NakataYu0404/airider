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

//  �ǉ��E�B���h�E�N���X�̖��O
const char* szClassNme =
{
    "window1",
};

//  �E�B���h�E�ɑ΂��郁�b�Z�[�W����(�N���b�N���Ƃ��A�T�C�Y�ύX���Ƃ��c�B)
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
    //  �p�b�h�̐U���L����
    SetUseJoypadVibrationFlag(true);

    //  �E�B���h�E�쐬�A������
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

    //  �G�t�F�N�V�A������
    InitEffekseer();

    SetUseDirectInputFlag(true);

    InputManager::CreateInstance();

    ResourceManager::CreateInstance();

    CollisionManager::CreateInstance();

    SceneManager::CreateInstance();
    //  �T�u�E�B���h�E�̃n���h�����Z�b�g
    SceneManager::GetInstance().SetSubWindowH(hWnd);

    //  �����Z���k�c�C�X�^�[����
    std::random_device rd;
    gen_ = std::mt19937(rd());
}

void Application::InitWindows(void)
{
    //  �E�B���h�E�̏����ݒ�
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

    //  �E�B���h�E�T�C�Y
    RECT windowRect = { 0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y };

    //  �E�B���h�E�̃T�C�Y�𒲐�
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    //  TODO:�E�B���h�E��B���j�^�[�ɏo���ɂ́A���W��ς��邵���Ȃ��̂ŁA���傤�ǂ����Ƃ��ɍ��W��ς��邱�ƂɂȂ��Ă��邪�A�Ȃ�炩�̕��@�𒲂ׂāA�����ňʒu���肪�s����悤�ɂ�����
    //  ����:CreateWindow�̂Ƃ��A�o���E�B���h�E�����j�^�[�͈͓��̈ʒu�ɂȂ��ƁA�������߂���d�ɂȂ�^�����ӂ��邱�ƁI�����炭�S�̂�30FPS�ɋ�������Ă���H
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