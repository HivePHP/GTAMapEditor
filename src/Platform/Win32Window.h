#pragma once
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>

#include "../GTA/IDE/IDETypes.h"
#include "../GTA/IPL/IPLTypes.h"

class Win32Window {
public:
    Win32Window(const wchar_t* title, int width, int height);
    ~Win32Window();

    HWND GetHWND() const { return m_hWnd; }
    bool ProcessMessages();

    void SetIDEData(const std::vector<IDEObject>& ideData);
    void SetIPLData(const std::vector<IPLPlacement>& iplData);

    bool IsKeyPressed(int key) const;
    bool IsRightMouseButtonPressed() const;
    void GetMousePosition(int& x, int& y) const; // Изменили float на int

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void CreateUI();

    HWND m_hWnd;
    HWND m_hTab;
    HWND m_hListIDE;
    HWND m_hListIPL;

    std::vector<IDEObject> m_IDEData;
    std::vector<IPLPlacement> m_IPLData;

    std::wstring m_TempWStr;
    const wchar_t* ToWChar(const std::string& str);
};