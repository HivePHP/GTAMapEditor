#pragma once
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <utility>

#include "../GTA/IDE/IDETypes.h"
#include "../GTA/IPL/IPLTypes.h"

class Win32Window {
public:
    Win32Window(const wchar_t* title, int width, int height);
    ~Win32Window();

    HWND GetHWND() const { return m_hWnd; }
    bool ProcessMessages();

    // API для передачи данных в UI
    void SetIDEData(const std::vector<IDEObject>& ideData);
    void SetIPLData(const std::vector<IPLPlacement>& iplData);

    // Методы ввода для камеры
    bool IsKeyPressed(int key) const;
    bool IsRightMouseButtonPressed() const;
    std::pair<float, float> GetMousePosition() const;

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