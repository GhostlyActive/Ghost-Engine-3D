/*
	MIT License

	C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
	Ghost Engine 3D (https://github.com/GhostlyActive/Ghost-Engine-3D)

	Copyright (c) 2019-2020, PardCode
	Copyright (c) 2021, GhostlyActive  

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "Window.h"

Window::Window()
{
	
}

/*
	- this is like a interrupt function
	- windows Procedore we can handle events like creation or destroying events. We call it "WndProc"
		 an application-defined function that processes messages sent to a window.

	- msg -> message paramter-> what event has been called ike WM_CREATE or WM_DESTROY or Default which pass all paramters available
*/

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg, WPARAM wparam, LPARAM lparam)
{
	
	switch (msg)
	{

	case WM_CREATE:
	{
		// event fired when the window is created
		//  get "this" pointer passed in CreateWindowEx
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;

		// save pointer "this" in structure data HWND
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		window->setHWND(hwnd);

		// call the create event
		window->onCreate();
		break;
	}

	case WM_DESTROY:
	{
		// event fired when the window is destroyed
		// recover "this" from the HWND structure
		Window* window =(Window*) GetWindowLongPtr(hwnd, GWLP_USERDATA);

		// call the destroy event
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}


/*
	RegisterClassEx: function registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function

		ATOM RegisterClassEx(
			const WNDCLASSEX *Arg1
		);

	WNDCLASSEX: structure contains window class information. It is used with the the RegisterClassEx and GetClassInfoEx functions

	CreateWindowEx: function creates an overlapped, pop-up, or child window with an extended style. CreateWindowEx returns a handle to the new window, or zero if the function fails
*/

bool Window::init()
{
	// setting up WNDCLASSEX object
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "MyWindowClass";
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc)) // if the registration of class will fail, the function will return false
		return false;

	// creation of the window
	m_hwnd=::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MyWindowClass", "Ghost Engine 3D", 
		WS_CAPTION|WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, this);

	// if the creation fail return false
	if (!m_hwnd) 
		return false;

	// show up the window
	::ShowWindow(m_hwnd, SW_SHOW);

	// allow to redraw the content of our window
	::UpdateWindow(m_hwnd);

	// set this flag to true to indicate that the window is initialized and running
	m_is_run = true;

	return true;
}


/*
	- pick the event messages of the window from operating system -> translate an dispatch to the windows precudure
	- this allows procudure -> WM_CREATE and WM_DESTROY
	-  PeekMessage: Dispatches incoming sent messages, checks the thread message queue for a posted message, and retrieves the message (if any exist).
		while() -> queue to get messages from operating system -> until queue is empty <= 0
*/

bool Window::broadcast()
{
	MSG msg;

	// event onUpdate to render the frames.
	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		// for each message
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}


bool Window::release()
{
	// destroy the window
	if (!::DestroyWindow(m_hwnd))
		return false;

	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

// get size of the window without titlebar
RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

//set handle of the window 
void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

Window::~Window()
{
}
