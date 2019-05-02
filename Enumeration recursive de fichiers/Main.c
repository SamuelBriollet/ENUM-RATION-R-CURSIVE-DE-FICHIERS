#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

void DisplayErrorBox(LPTSTR lpszFunction);

int _tmain(int argc, TCHAR *argv[])
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Si le r�pertoire n'est pas sp�cifi� en tant qu'argument de ligne de commande,
	// utilisation de l'impression.

	if (argc != 2)
	{
		_tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
		return (-1);
	}

	// V�rifiez que le chemin d'entr�e plus 3 n'est pas plus long que MAX_PATH.
	// Trois caract�res correspondent � "\ *" et � NULL, ajout�s ci-dessous.

	StringCchLength(argv[1], MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

	// pr�pare la cha�ne � utiliser avec les fonctions FindFile. Tout d'abord, copiez le
	// Puis ajoutez '\ *' au nom du r�pertoire.

	StringCchCopy(szDir, MAX_PATH, argv[1]);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Trouve le premier fichier du r�pertoire.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
		return dwError;
	}

	// Liste tous les fichiers du r�pertoire avec quelques informations � leur sujet.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	return dwError;
}