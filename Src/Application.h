#pragma once
#include <iostream>
#include <random>
#include <string>
#include <windows.h>
#include "Utility/Utility.h"

class Application
{
public:
    // �X�N���[���T�C�Y
    static constexpr int SCREEN_SIZE_X = 1600;
    static constexpr int SCREEN_SIZE_Y = 900;

    // �f�[�^�p�X�֘A
    // -------------------------------------------
    static const std::string PATH_IMAGE;
    static const std::string PATH_MODEL;
    static const std::string PATH_EFFECT;
    static const std::string PATH_SHADER;
    static const std::string PATH_SOUND;
    // -------------------------------------------

    // �����I�ɃC���X�^���X�𐶐�����
    static void CreateInstance(void);

    // �ÓI�C���X�^���X�̎擾
    static Application& GetInstance(void);

    // ������
    void Init(void);

    // �Q�[�����[�v�̊J�n
    void Run(void);

    // ���\�[�X�̔j��
    void Destroy(void);

    // �����������^���s�̔���
    bool IsInitFail(void) const;

    // ��������^���s�̔���
    bool IsReleaseFail(void) const;

    //  �����_���Ȑ����擾����
    int GetRandomNum(int max);
private:

    static constexpr VECTOR INIT_SUBSCREEN_POS = { 200,0,0 };
    static constexpr int INIT_COLORBIT = 32;
    static constexpr int EFFECSEER_MAXPARTICLE = 8000;

    // �f�t�H���g�R���X�g���N�^��private�ɂ��āA�O�����琶���ł��Ȃ��l�ɂ���
    Application(void);
    Application(const Application& manager) = default;
    ~Application(void) = default;

    // Effekseer�̏�����
    void InitEffekseer(void);

    // �E�B���h�E�̏�����
    void InitWindows(void);

    // �ÓI�C���X�^���X
    static Application* instance_;

    // ���������s
    bool isInitFail_;

    // ������s
    bool isReleaseFail_;

    // �E�B���h�E�n���h��
    HWND hWnd;

    std::mt19937 gen_;
};