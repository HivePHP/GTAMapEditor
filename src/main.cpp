#include "Core/Application.h"
#include <windows.h>

// »спользуем аннотации _In_ и _In_opt_ дл€ строгого соответстви€ Windows SDK
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    Application app;
    app.Run();
    return 0;
}