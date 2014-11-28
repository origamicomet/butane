//===-- bitbyte/butane/window.c ---------------------------------*- C++ -*-===//
//
//  Butane
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@bitbyte.ca>
//
//===----------------------------------------------------------------------===//

#include "bitbyte/butane/window.h"

//===----------------------------------------------------------------------===//

#include <stdlib.h>

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  #include <windows.h>
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

struct bitbyte_butane_window {
#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  HWND hndl;
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
  char title[256];
};

//===----------------------------------------------------------------------===//

bitbyte_butane_window_t *
bitbyte_butane_window_open(
  const bitbyte_butane_window_opts_t *opts)
{
  bitbyte_butane_assert_debug(opts != NULL);
  bitbyte_butane_assert_debug(opts->title != NULL);
  bitbyte_butane_assert_debug(opts->width > 0);
  bitbyte_butane_assert_debug(opts->height > 0);

  bitbyte_butane_window_t *window =
    (bitbyte_butane_window_t *)malloc(sizeof(bitbyte_butane_window_t));

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  char szUUID[37];
  bitbyte_foundation_uuid_t uuid;
  bitbyte_foundation_uuid_generate(&uuid);
  bitbyte_foundation_uuid_to_s(&uuid, szUUID);

  WCHAR szClassName[37];
  if (!MultiByteToWideChar(CP_UTF8, 0, szUUID, -1, szClassName, 37))
    bitbyte_butane_assertf_always("Generated class name exceeds buffer size!");

  WNDCLASSEXW wcx;
  memset(&wcx, 0, sizeof(WNDCLASSEX));
  wcx.cbSize        = sizeof(WNDCLASSEX);
  wcx.style         = CS_VREDRAW | CS_HREDRAW;
  // TODO(mwilliams): Use our own custom window procedure.
  wcx.lpfnWndProc   = &DefWindowProcW;
  wcx.hInstance     = GetModuleHandle(NULL);
  wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
  // TODO(mwilliams): Use our own icon, IDI_ENGINE_ICON.
  wcx.hIcon         = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_APPLICATION));
  wcx.hIconSm       = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_APPLICATION));
  wcx.lpszClassName = szClassName;

  const BOOL bRegisteredClass = (RegisterClassExW(&wcx) != 0);
  bitbyte_butane_assertf(bRegisteredClass,
                         "Unable to open window at RegisterClassExW! (%d)",
                         GetLastError());

  const DWORD dwStyles = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
  const DWORD dwExStyles = WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;

  WCHAR szTitle[256];
  if (!MultiByteToWideChar(CP_UTF8, 0, opts->title, -1, szTitle, 256))
    bitbyte_butane_assertf_always("Specified title exceeds buffer size!");

  RECT rClientArea = { 0, 0, opts->width, opts->height };
  AdjustWindowRectEx(&rClientArea, dwStyles, FALSE, dwExStyles);
  const DWORD dwAdjustedWidth = rClientArea.right - rClientArea.left;
  const DWORD dwAdjustedHeight = rClientArea.bottom - rClientArea.top;

  window->hndl = CreateWindowExW(dwExStyles, szClassName, szTitle, dwStyles,
                                 0, 0, dwAdjustedWidth, dwAdjustedHeight, NULL,
                                 NULL, GetModuleHandle(NULL), NULL);

  bitbyte_butane_assertf(window->hndl != NULL,
                         "Unable to open window at CreateWindowExW! (%d)",
                         GetLastError());

  SetPropA(window->hndl, "bitbyte_butane_window_t", (HANDLE)window);

  // Register for keyboard and mouse raw-input events:
  // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf.
  RAWINPUTDEVICE RawInputDevices[2];

  // Keyboard:
  RawInputDevices[0].usUsagePage = 0x01;
  RawInputDevices[0].usUsage = 0x06;
  RawInputDevices[0].hwndTarget = window->hndl;
  RawInputDevices[0].dwFlags = 0;

  // Mouse:
  RawInputDevices[1].usUsagePage = 0x01;
  RawInputDevices[1].usUsage = 0x02;
  RawInputDevices[1].hwndTarget = window->hndl;
  RawInputDevices[1].dwFlags = 0;

  const BOOL bRegisteredRawInputDevices =
    RegisterRawInputDevices(&RawInputDevices[0], 2, sizeof(RAWINPUTDEVICE));
  bitbyte_butane_assertf(bRegisteredRawInputDevices,
                         "Unable to open window at RegisterRawInputDevices! (%d)",
                         GetLastError());

  HMONITOR hMonitor = MonitorFromWindow(window->hndl, MONITOR_DEFAULTTONULL);

  bitbyte_butane_assertf(hMonitor != NULL,
                         "Unable to open window at MonitorFromWindow! (%d)",
                         GetLastError());

  MONITORINFO miMonitorInfo;
  memset((void *)&miMonitorInfo, 0, sizeof(MONITORINFO));
  miMonitorInfo.cbSize = sizeof(MONITORINFO);
  const BOOL bHaveMonitorInfo = GetMonitorInfo(hMonitor, &miMonitorInfo);

  bitbyte_butane_assertf(bHaveMonitorInfo,
                         "Unable to open window at GetMonitorInfo! (%d)",
                         GetLastError());

  RECT rEncompassingArea;
  const BOOL bHaveEncompassingArea = GetWindowRect(window->hndl, &rEncompassingArea);

  bitbyte_butane_assertf(bHaveEncompassingArea,
                         "Unable to open window at GetWindowRect! (%d)",
                         GetLastError());

  const DWORD dwMonitorWidth = miMonitorInfo.rcMonitor.right - miMonitorInfo.rcMonitor.left;
  const DWORD dwMonitorHeight = miMonitorInfo.rcMonitor.bottom - miMonitorInfo.rcMonitor.top;
  const DWORD dwEncompassingWidth = rEncompassingArea.right - rEncompassingArea.left - 1;
  const DWORD dwEncompassingHeight = rEncompassingArea.bottom - rEncompassingArea.top - 1;
  const DWORD dwCenteredPositionX = (dwMonitorWidth - dwEncompassingWidth) / 2 + miMonitorInfo.rcMonitor.left;
  const DWORD dwCenteredPositionY = (dwMonitorHeight - dwEncompassingHeight) / 2 + miMonitorInfo.rcMonitor.top;

  SetWindowPos(window->hndl, HWND_TOP, dwCenteredPositionX, dwCenteredPositionY,
               0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif

  return window;
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_close(
  bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  DestroyWindow(window->hndl);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif

  free((void *)window);
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_update(
  bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  MSG Message;
  while (PeekMessage(&Message, window->hndl, 0, 0, PM_REMOVE)) {
    TranslateMessage(&Message);
    DispatchMessage(&Message);
  }
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_rename(
  bitbyte_butane_window_t *window,
  const char *title)
{
  bitbyte_butane_assert_debug(window != NULL);
  bitbyte_butane_assert_debug(title != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  WCHAR szTitle[256];
  if (!MultiByteToWideChar(CP_UTF8, 0, title, -1, szTitle, 256))
    bitbyte_butane_assertf_always("Specified title exceeds buffer size!");
  SetWindowTextW(window->hndl, szTitle);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_resize(
  bitbyte_butane_window_t *window,
  uint32_t width,
  uint32_t height)
{
  bitbyte_butane_assert_debug(window != NULL);
  bitbyte_butane_assert_debug(width > 0);
  bitbyte_butane_assert_debug(height > 0);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  const DWORD dwStyles = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
  const DWORD dwExStyles = WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;

  RECT rClientArea = { 0, 0, width, height };
  AdjustWindowRectEx(&rClientArea, dwStyles, FALSE, dwExStyles);
  const DWORD dwAdjustedWidth = rClientArea.right - rClientArea.left;
  const DWORD dwAdjustedHeight = rClientArea.bottom - rClientArea.top;

  SetWindowPos(window->hndl, NULL, 0, 0, dwAdjustedWidth, dwAdjustedHeight,
               SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_show(
  bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  ShowWindow(window->hndl, SW_SHOW);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_hide(
  bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  ShowWindow(window->hndl, SW_HIDE);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_focus(
  bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_minimize(
  bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  ShowWindow(window->hndl, SW_MINIMIZE);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_maximize(
  bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  ShowWindow(window->hndl, SW_MAXIMIZE);
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}

//===----------------------------------------------------------------------===//

const char *
bitbyte_butane_window_title(
  const bitbyte_butane_window_t *window)
{
  bitbyte_butane_assert_debug(window != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  WCHAR szTitle[256];
  const BOOL bGotTitle = GetWindowTextW(window->hndl, (LPWSTR)szTitle, 256);
  bitbyte_butane_assertf(bGotTitle,
                         "Unable to get window title at GetWindowTextW! (%d)",
                         GetLastError());
  if (!WideCharToMultiByte(CP_UTF8, 0, szTitle, -1, (LPSTR)window->title, 256, NULL, NULL))
    bitbyte_butane_assertf_always("Requested title exceeds buffer size!");
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif

  return (const char *)&window->title;
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_window_dimensions(
  const bitbyte_butane_window_t *window,
  uint32_t *width,
  uint32_t *height)
{
  bitbyte_butane_assert_debug(window != NULL);
  bitbyte_butane_assert_debug(width != NULL);
  bitbyte_butane_assert_debug(height != NULL);

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__
  RECT rClientArea;
  const BOOL bHaveClientArea = GetClientRect(window->hndl, &rClientArea);
  bitbyte_butane_assertf(bHaveClientArea,
                         "Unable to get window dimensions at GetClientRect! (%d)",
                         GetLastError());
  *width = rClientArea.right - rClientArea.left;
  *height = rClientArea.bottom - rClientArea.top;
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_MAC_OS_X__
#elif BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_LINUX__
#endif
}


//===----------------------------------------------------------------------===//

#ifdef __cplusplus
}
#endif // __cplusplus

//============================================================================//

#ifdef __cplusplus

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//

// ...

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//
