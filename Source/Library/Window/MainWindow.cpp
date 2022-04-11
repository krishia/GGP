#include "Window/MainWindow.h"

namespace library
{
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
      Method:   MainWindow::Initialize

      Summary:  Initializes main window

      Args:     HINSTANCE hInstance
                  Handle to the instance
                INT nCmdShow
                    Is a flag that says whether the main application window
                    will be minimized, maximized, or shown normally
                PCWSTR pszWindowName
                    The window name

      Returns:  HRESULT
                  Status code
    M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    /*--------------------------------------------------------------------
      TODO: MainWindow::Initialize definition (remove the comment)
    --------------------------------------------------------------------*/
    HRESULT MainWindow::Initialize(_In_ HINSTANCE hInstance, _In_ INT nCmdShow, _In_ PCWSTR pszWindowName)
    {
        


        initialize(hInstance, nCmdShow, pszWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

        
        
       
        return S_OK;
    }
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
      Method:   MainWindow::GetWindowClassName

      Summary:  Returns the name of the window class

      Returns:  PCWSTR
                  Name of the window class
    M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    /*--------------------------------------------------------------------
      TODO: MainWindow::GetWindowClassName definition (remove the comment)
    --------------------------------------------------------------------*/
    PCWSTR MainWindow::GetWindowClassName() const
    {
        return m_pszWindowName;
    }

    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
      Method:   MainWindow::HandleMessage

      Summary:  Handles the messages

      Args:     UINT uMessage
                  Message code
                WPARAM wParam
                    Additional data the pertains to the message
                LPARAM lParam
                    Additional data the pertains to the message

      Returns:  LRESULT
                  Integer value that your program returns to Windows
    M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    /*--------------------------------------------------------------------
      TODO: MainWindow::HandleMessage definition (remove the comment)
    --------------------------------------------------------------------*/
    LRESULT MainWindow::HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
    {
        RAWINPUTDEVICE rid = {};
        rid.usUsagePage = 0x01;
        rid.usUsage = 0x02;
        rid.dwFlags = RIDEV_INPUTSINK;
        rid.hwndTarget = m_hWnd;
        if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
        {
            return E_FAIL;
        }
        switch (uMsg)
        {

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case 0x57:
                m_directions.bFront = TRUE;
                break;
            case 0x41:
                m_directions.bLeft = TRUE;
                break;
            case 0x53:
                m_directions.bBack = TRUE;
                break;
            case 0x44:
                m_directions.bRight = TRUE;
                break;
            case VK_SPACE:
                m_directions.bUp = TRUE;
                break;
            case VK_SHIFT:
                m_directions.bDown = TRUE;
                break;
            default:
                break;
            }
            break;
        }

        case WM_KEYUP:
        {
            switch (wParam)
            {
            case 0x57:
                m_directions.bFront = FALSE;
                break;
            case 0x41:
                m_directions.bLeft = FALSE;
                break;
            case 0x53:
                m_directions.bBack = FALSE;
                break;
            case 0x44:
                m_directions.bRight = FALSE;
                break;
            case VK_SPACE:
                m_directions.bUp = FALSE;
                break;
            case VK_SHIFT:
                m_directions.bDown = FALSE;
                break;
            default:
                break;
            }
            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hWnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(m_hWnd, &ps);
        }
        case WM_INPUT:
        {
            UINT dwSize = sizeof(RAWINPUT);
            static BYTE lpb[sizeof(RAWINPUT)];
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
            RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);
            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                m_mouseRelativeMovement.X = raw->data.mouse.lLastX;
                m_mouseRelativeMovement.Y = raw->data.mouse.lLastY;
            }
            break;
        }
        default:
            return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
        }
  
        return S_OK;

    };
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
     Method:   MainWindow::GetDirections
     Summary:  Returns the keyboard direction input
     Returns:  const DirectionsInput&
                 Keyboard direction input
   M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    const DirectionsInput& MainWindow::GetDirections() const
    {
        return m_directions;
    }
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
     Method:   MainWindow::GetMouseRelativeMovement
     Summary:  Returns the mouse relative movement
     Returns:  const MouseRelativeMovement&
                 Mouse relative movement
   M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
   /*--------------------------------------------------------------------
     TODO: MainWindow::GetMouseRelativeMovement definition (remove the comment)
   --------------------------------------------------------------------*/
    const MouseRelativeMovement& MainWindow::GetMouseRelativeMovement() const
    {
        return m_mouseRelativeMovement;
    }
   /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
     Method:   MainWindow::ResetMouseMovement
     Summary:  Reset the mouse relative movement to zero
   M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
   /*--------------------------------------------------------------------
     TODO: MainWindow::ResetMouseMovement definition (remove the comment)
   --------------------------------------------------------------------*/
    void MainWindow::ResetMouseMovement()
    {
        m_mouseRelativeMovement = {};
        m_mouseRelativeMovement.X = 0;
        m_mouseRelativeMovement.Y = 0;
    }
}
