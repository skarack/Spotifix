// Spotifix.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <tlhelp32.h>
#include "stdafx.h"

enum SPOTIFYCOMMAND
{
	SPOTIFY_MUTE = 524288,
	SPOTIFY_VOLUMEDOWN = 589824,
	SPOTIFY_VOLUMEUP = 655360,
	SPOTIFY_NEXT = 720896,
	SPOTIFY_PREV = 786432,
	SPOTIFY_STOP = 851968,
	SPOTIFY_PLAYPAUSE = 917504
};

DWORD GetSpotifyProcessId()
{
	DWORD processId = MAXDWORD;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
	{
		while (processId == MAXDWORD && Process32Next(snapshot, &entry))
		{
			if (_tcsicmp(entry.szExeFile, _T("spotify.exe")) == 0)
			{
				processId = entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);

	return processId;
}

HWND GetSpotifyHwnd(DWORD processId)
{
	if (processId == MAXDWORD)
	{
		return NULL;
	}

	HWND h = ::GetTopWindow(0);
	while (h)
	{
		DWORD pid;
		DWORD dwTheardId = ::GetWindowThreadProcessId(h, &pid);

		if (pid == processId)
		{
			break;
		}

		h = ::GetNextWindow(h, GW_HWNDNEXT);
	}

	return h;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		return 0;
	}

	HWND spotifyHwnd = GetSpotifyHwnd(GetSpotifyProcessId());
	if (spotifyHwnd == NULL)
	{
		return 0;
	}

	if (_tcsicmp(argv[1], _T("next")) == 0)
	{
		SendMessage(spotifyHwnd, WM_APPCOMMAND, 0, SPOTIFY_NEXT);
	}
	else if (_tcsicmp(argv[1], _T("prev")) == 0)
	{
		SendMessage(spotifyHwnd, WM_APPCOMMAND, 0, SPOTIFY_PREV);
	}

	return 0;
}

