//
// align_window2 - align_window2@tomseddon.plus.com
//
// THIS PROGRAM IS SUPPLIED WITHOUT WARRANTY.
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

void Entry()
{
	HWND hWnd=GetForegroundWindow();
	if(hWnd)
	{
		int argc;
		wchar_t **argv=CommandLineToArgvW(GetCommandLineW(),&argc);
		if(argc>=1)
		{
			int which=argv[1][0]-(int)L'1';
			if(which>=0&&which<=8)
			{
				int canResize=GetWindowLong(hWnd,GWL_STYLE)&WS_THICKFRAME;

				MONITORINFO miTarget;
				miTarget.cbSize=sizeof miTarget;
				GetMonitorInfo(MonitorFromWindow(hWnd,MONITOR_DEFAULTTOPRIMARY),&miTarget);

				UINT swpFlags=SWP_NOCOPYBITS|SWP_NOOWNERZORDER|SWP_NOZORDER;
				if(!canResize)
					swpFlags|=SWP_NOSIZE;

				RECT newRect=miTarget.rcWork;

				int halfh=(newRect.bottom-newRect.top)/2;

				if(argv[1][1]==L'w'||argv[1][1]==L'W')
				{
					bool two=argv[1][1]==L'W';
					int thirdw=(newRect.right-newRect.left)/3;
					int twothirdw=(newRect.right-newRect.left)*2/3;

					// Y
					switch(which)
					{
					case 7-1:
					case 8-1:
					case 9-1:
						// Top half
						newRect.bottom=newRect.top+halfh;
						break;

					case 4-1:
					case 5-1:
					case 6-1:
						// Full height
						break;

					case 1-1:
					case 2-1:
					case 3-1:
						// Bottom half
						newRect.top=newRect.bottom-halfh;
						break;
					}

					// X
					switch(which)
					{
					case 7-1:
					case 4-1:
					case 1-1:
						// Left
						newRect.right=newRect.left+(two?twothirdw:thirdw);
						break;

					case 8-1:
					case 5-1:
					case 2-1:
						// Middle
						newRect.left=newRect.left+thirdw;
						newRect.right=newRect.left+thirdw;
						break;

					case 9-1:
					case 6-1:
					case 3-1:
						// Right
						newRect.left=newRect.right-(two?twothirdw:thirdw);
						break;
					}

					if(IsZoomed(hWnd))
						ShowWindow(hWnd,SW_RESTORE);

					SetWindowPos(hWnd,0,newRect.left,newRect.top,newRect.right-newRect.left,newRect.bottom-newRect.top,swpFlags);
				}
				else
				{
					if(which==4)//i.e., 5 key
					{
						if(canResize)
							ShowWindow(hWnd,IsZoomed(hWnd)?SW_RESTORE:SW_MAXIMIZE);
					}
					else
					{
						int halfw=(newRect.right-newRect.left)/2;

						// Y
						switch(which)
						{
						case 7-1:
						case 8-1:
						case 9-1:
							// Top half
							newRect.bottom=newRect.top+halfh;
							break;

						case 4-1:
						case 5-1:
						case 6-1:
							// Full height
							break;

						case 1-1:
						case 2-1:
						case 3-1:
							// Bottom half
							newRect.top=newRect.bottom-halfh;
							break;
						}

						// X
						switch(which)
						{
						case 7-1:
						case 4-1:
						case 1-1:
							// Left half
							newRect.right=newRect.left+halfw;
							break;

						case 8-1:
						case 5-1:
						case 2-1:
							// Full width
							break;

						case 9-1:
						case 6-1:
						case 3-1:
							// Right half
							newRect.left=newRect.right-halfw;
							break;
						}

						if(IsZoomed(hWnd))
							ShowWindow(hWnd,SW_RESTORE);

						SetWindowPos(hWnd,0,newRect.left,newRect.top,newRect.right-newRect.left,newRect.bottom-newRect.top,swpFlags);
					}
				}
			}
		}

		LocalFree(argv);
	}

	ExitProcess(0);
}
