#include <chrono>

#include <Windows.h>
#include <dwmapi.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#include "Deep.h"

#include "main.h"
#include "renderer.h"

#include <WinUser.h>

HGLRC m_hrc;

BOOL initSC() {
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

    return 0;
}

void resizeSC(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

BOOL CreateHGLRC(HWND hWnd) {
    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,                                // Version Number
      PFD_DRAW_TO_WINDOW |         // Format Must Support Window
      PFD_SUPPORT_OPENGL |         // Format Must Support OpenGL
      PFD_SUPPORT_COMPOSITION |         // Format Must Support Composition
      PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
      PFD_TYPE_RGBA,                    // Request An RGBA Format
      32,                               // Select Our Color Depth
      0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
      8,                                // An Alpha Buffer
      0,                                // Shift Bit Ignored
      0,                                // No Accumulation Buffer
      0, 0, 0, 0,                       // Accumulation Bits Ignored
      24,                               // 16Bit Z-Buffer (Depth Buffer)
      8,                                // Some Stencil Buffer
      0,                                // No Auxiliary Buffer
      PFD_MAIN_PLANE,                   // Main Drawing Layer
      0,                                // Reserved
      0, 0, 0                           // Layer Masks Ignored
    };

    HDC hdc = GetDC(hWnd);
    int PixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (PixelFormat == 0) {
        return FALSE;
    }

    BOOL bResult = SetPixelFormat(hdc, PixelFormat, &pfd);
    if (bResult == FALSE) {
        return FALSE;
    }

    m_hrc = wglCreateContext(hdc);
    if (!m_hrc) {
        return FALSE;
    }

    ReleaseDC(hWnd, hdc);

    return TRUE;
}

int running = 1;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_CHAR:
        if (wparam == VK_ESCAPE)
        {
            running = 0;
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_DESTROY:
        if (m_hrc)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(m_hrc);
        }
        running = 0;
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

int main()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE, 65535, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE, 65535, 0, 0, 0);

    /*Hide terminal*/
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    while (true)
    {
        running = 1;

        const char* windowClassName = "Window in Console";
        WNDCLASS windowClass = { 0 };
        windowClass.hbrBackground = (HBRUSH)CreateSolidBrush(0x00000000);
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.hInstance = NULL;
        windowClass.lpfnWndProc = WndProc;
        windowClass.lpszClassName = windowClassName;
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        if (!RegisterClass(&windowClass))
            MessageBox(NULL, "Could not register class", "Error", MB_OK);

        w = GetSystemMetrics(SM_CXSCREEN);
        h = GetSystemMetrics(SM_CYSCREEN);

        HWND windowHandle = CreateWindowEx(
            WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
            windowClassName,
            NULL,
            WS_POPUP, //borderless
            0, //x coordinate of window start point
            0, //y start point
            w, //width of window; this function
            h, //height of the window
            NULL, //handles and such, not needed
            NULL,
            NULL,
            NULL);
        ShowWindow(windowHandle, SW_MAXIMIZE);
        SetLayeredWindowAttributes(windowHandle, 0, 255, LWA_ALPHA);

        DWM_BLURBEHIND bb = { 0 };
        HRGN hRgn = CreateRectRgn(0, 0, -1, -1);
        bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
        bb.hRgnBlur = hRgn;
        bb.fEnable = TRUE;
        DwmEnableBlurBehindWindow(windowHandle, &bb);

        SetLayeredWindowAttributes(windowHandle, 0, 255, LWA_ALPHA);

        CreateHGLRC(windowHandle);

        HDC hdc = GetDC(windowHandle);
        wglMakeCurrent(hdc, m_hrc);
        initSC();
        resizeSC(w, h);

        MSG messages;

        Init();

        auto prev = std::chrono::high_resolution_clock::now();

        //Deep::StartMemoryDebug();

        while (running)
        {
#include "Deep_Debug_Memory_Undef.h"
            if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }
#include "Deep_Debug_Memory_Def.h"
            else
            {
                auto now = std::chrono::high_resolution_clock::now();
                float dt = std::chrono::duration<float, std::milli>(now - prev).count() / 1000.0f;
                prev = now;

                //std::cout << dt << "ms" << std::endl;
                Update(dt);
                Render();

                SwapBuffers(hdc);

                Sleep(10);
            }
        }

        //Deep::PrintAllocationMap();
        //Deep::EndMemoryDebug();

        ReleaseDC(windowHandle, hdc);
        DeleteObject(windowHandle); //doing it just in case

        while (IsWindow(windowHandle)) {}
    }

    return 1;
}