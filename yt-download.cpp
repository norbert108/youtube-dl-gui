#include <windows.h>

const char g_szClassName[] = "myWindowClass";
HWND hwnd;
HWND hwndButton;
HWND hwndLinkTextField;

char fullCommand[1024];

void TriggerYTDownload(LPSTR link) 
{
	static const char command[] = "youtube-dl.exe -o \"downloads/%(title)s.%(ext)s\" -x --audio-format mp3 --audio-quality 0 ";
	memset(fullCommand, 0, 1024);
	strcpy(fullCommand, command);
	strcat(fullCommand, "\"");
	strcat(fullCommand, link);
	strcat(fullCommand, "\"");
	system(fullCommand);	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    	case WM_COMMAND:
			if((HWND) lParam == hwndButton)
			{
				DWORD linkLength = GetWindowTextLength(hwndLinkTextField);
				LPSTR buffer = (LPSTR) GlobalAlloc(GPTR, linkLength + 1);
				GetWindowText(hwndLinkTextField, buffer, linkLength + 1);
				TriggerYTDownload(buffer);
				GlobalFree(buffer);
			}
			break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Download mp3 from youtube", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, hInstance, NULL);

	hwndLinkTextField = CreateWindowEx(WS_EX_WINDOWEDGE,TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE, 10, 10, 500, 25, hwnd, NULL, NULL, NULL);
	hwndButton = CreateWindow("BUTTON", "OK", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 45, 30, 20, hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
