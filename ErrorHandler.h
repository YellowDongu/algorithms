#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

class Error
{
public:
    static void errorModal(std::exception& errorCode, std::string place);
    static void errorModal(std::string customError, std::string place);
    static void createDbgConsole();

private:
    //string to wstring -> ���ڸ� wstring�� �޾Ƶ��϶�
    static std::wstring toWStr(const std::string& str);

};

#define except(_placeStr)\
catch (const std::exception& err)\
{\
    Error::errorModal(err, _placeStr);\
}\


//���� ó�� - �˾� �����츦 ����ش�
void Error::errorModal(std::exception& errorCode, std::string place)
{
#ifdef _DEBUG
    std::string errorMessage = errorCode.what() + '\n' + place;
    MessageBoxA(nullptr, errorMessage.c_str(), "(Debug)Exception Caught", MB_OK | MB_ICONERROR);
#endif
}
//Ŀ���� ���� ó�� - �˾� �����츦 ����ش�
void Error::errorModal(std::string customError, std::string place)
{
#ifdef _DEBUG
    std::string errorMessage = customError + '\n' + place;
    MessageBoxA(nullptr, errorMessage.c_str(), "(Debug)Exception Caught", MB_OK | MB_ICONERROR);
#endif
}

// �ܼ� â�� ����� ����׿����� �� �� �ְ� ��
void Error::createDbgConsole()
{
#ifdef _DEBUG
    AllocConsole();

    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);

    SetConsoleTitleW(L"Debug Console");

    std::cout << "Console Active!" << std::endl;
#endif
}


std::wstring Error::toWStr(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    return wstr;
}
