#include "Win32Window.h"
#include "../Core/Logger.h"

#pragma comment(lib, "comctl32.lib")

Win32Window::Win32Window(const wchar_t* title, int width, int height) {
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MapEditorWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    m_hWnd = CreateWindowExW(
        0,
        L"MapEditorWindowClass",
        title,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, GetModuleHandle(NULL), this
    );

    CreateUI();
}

Win32Window::~Win32Window() {
    DestroyWindow(m_hWnd);
}

bool Win32Window::ProcessMessages() {
    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

const wchar_t* Win32Window::ToWChar(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    m_TempWStr.resize(size);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &m_TempWStr[0], size);
    return m_TempWStr.c_str();
}

void Win32Window::CreateUI() {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    // Панель вкладок (слева)
    m_hTab = CreateWindowW(WC_TABCONTROLW, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
        0, 0, 400, 720, m_hWnd, NULL, GetModuleHandle(NULL), NULL);

    TCITEMW tie = { 0 };
    tie.mask = TCIF_TEXT;
    tie.pszText = const_cast<LPWSTR>(L"IDE Objects");
    TabCtrl_InsertItem(m_hTab, 0, &tie);
    tie.pszText = const_cast<LPWSTR>(L"IPL Placements");
    TabCtrl_InsertItem(m_hTab, 1, &tie);

    // Виртуальный список IDE (LVS_OWNERDATA)
    m_hListIDE = CreateWindowW(WC_LISTVIEWW, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_OWNERDATA | LVS_NOSORTHEADER,
        2, 25, 395, 690, m_hTab, NULL, GetModuleHandle(NULL), NULL);
    ListView_SetExtendedListViewStyle(m_hListIDE, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Виртуальный список IPL
    m_hListIPL = CreateWindowW(WC_LISTVIEWW, L"", WS_CHILD | WS_BORDER | LVS_REPORT | LVS_OWNERDATA | LVS_NOSORTHEADER,
        2, 25, 395, 690, m_hTab, NULL, GetModuleHandle(NULL), NULL);
    ListView_SetExtendedListViewStyle(m_hListIPL, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    LVCOLUMNW lvc = { 0 };
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;

    // Колонки IDE
    lvc.cx = 50; lvc.pszText = const_cast<LPWSTR>(L"ID"); ListView_InsertColumn(m_hListIDE, 0, &lvc);
    lvc.cx = 150; lvc.pszText = const_cast<LPWSTR>(L"Model"); ListView_InsertColumn(m_hListIDE, 1, &lvc);
    lvc.cx = 150; lvc.pszText = const_cast<LPWSTR>(L"TXD"); ListView_InsertColumn(m_hListIDE, 2, &lvc);

    // Колонки IPL
    lvc.cx = 50; lvc.pszText = const_cast<LPWSTR>(L"ID"); ListView_InsertColumn(m_hListIPL, 0, &lvc);
    lvc.cx = 150; lvc.pszText = const_cast<LPWSTR>(L"Model"); ListView_InsertColumn(m_hListIPL, 1, &lvc);
    lvc.cx = 150; lvc.pszText = const_cast<LPWSTR>(L"Position"); ListView_InsertColumn(m_hListIPL, 2, &lvc);
}

void Win32Window::SetIDEData(const std::vector<IDEObject>& ideData) {
    m_IDEData = ideData;
    ListView_SetItemCount(m_hListIDE, m_IDEData.size());
}

void Win32Window::SetIPLData(const std::vector<IPLPlacement>& iplData) {
    m_IPLData = iplData;
    ListView_SetItemCount(m_hListIPL, m_IPLData.size());
}

bool Win32Window::IsKeyPressed(int key) const {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

bool Win32Window::IsRightMouseButtonPressed() const {
    return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}

std::pair<float, float> Win32Window::GetMousePosition() const {
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);
    return { static_cast<float>(pt.x), static_cast<float>(pt.y) };
}

LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Win32Window* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Win32Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Win32Window::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_NOTIFY: {
        LPNMHDR nmhdr = reinterpret_cast<LPNMHDR>(lParam);
        if (!nmhdr) break;

        if (nmhdr->hwndFrom == m_hTab && nmhdr->code == TCN_SELCHANGE) {
            int tabIndex = TabCtrl_GetCurSel(m_hTab);
            ShowWindow(m_hListIDE, tabIndex == 0 ? SW_SHOW : SW_HIDE);
            ShowWindow(m_hListIPL, tabIndex == 1 ? SW_SHOW : SW_HIDE);
            return 0;
        }

        if (nmhdr->code == LVN_GETDISPINFOW) {
            NMLVDISPINFOW* plvdi = reinterpret_cast<NMLVDISPINFOW*>(lParam);

            if (plvdi->item.mask & LVIF_TEXT) {
                if (nmhdr->hwndFrom == m_hListIDE && plvdi->item.iItem < (int)m_IDEData.size()) {
                    const auto& item = m_IDEData[plvdi->item.iItem];
                    switch (plvdi->item.iSubItem) {
                    case 0: wcsncpy_s(plvdi->item.pszText, plvdi->item.cchTextMax, std::to_wstring(item.id).c_str(), _TRUNCATE); break;
                    case 1: wcsncpy_s(plvdi->item.pszText, plvdi->item.cchTextMax, ToWChar(item.modelName), _TRUNCATE); break;
                    case 2: wcsncpy_s(plvdi->item.pszText, plvdi->item.cchTextMax, ToWChar(item.txdName), _TRUNCATE); break;
                    }
                }
                else if (nmhdr->hwndFrom == m_hListIPL && plvdi->item.iItem < (int)m_IPLData.size()) {
                    const auto& item = m_IPLData[plvdi->item.iItem];
                    switch (plvdi->item.iSubItem) {
                    case 0: wcsncpy_s(plvdi->item.pszText, plvdi->item.cchTextMax, std::to_wstring(item.id).c_str(), _TRUNCATE); break;
                    case 1: wcsncpy_s(plvdi->item.pszText, plvdi->item.cchTextMax, ToWChar(item.modelName), _TRUNCATE); break;
                    case 2: {
                        std::wstring coords = std::to_wstring(static_cast<int>(item.posX)) + L", " +
                            std::to_wstring(static_cast<int>(item.posY)) + L", " +
                            std::to_wstring(static_cast<int>(item.posZ));
                        wcsncpy_s(plvdi->item.pszText, plvdi->item.cchTextMax, coords.c_str(), _TRUNCATE);
                        break;
                    }
                    }
                }
            }
            return 0;
        }
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}