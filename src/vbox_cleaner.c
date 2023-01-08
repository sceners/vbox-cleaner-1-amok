#include <windows.h>

HWND Handle, button_Begin1, button_About1, button_Close1, label_text1, label_text2, button_Check1, button_Check2;
int hFont1;
char szFile[MAX_PATH];
char Vboxkeys[256][256];
int autofiledel,autokeydel,foundfiles,foundkeys,delfiles,delkeys,initdel;


void addtowininit(char dir[255], char filename[255])
{
	HANDLE hFile;
	char windir[255],wininit[255];
	char delname[255];
	
	GetWindowsDirectory(windir, MAX_PATH);
	wsprintf(wininit, "%s\\WININIT.INI", windir);
	wsprintf(delname, "%s\\%s", dir, filename);
	
	if (WritePrivateProfileString("Rename", "NUL", delname, wininit) != 0)
	{
		initdel++;
	}
}

int getvalue(char subkey[255])
{
	char lpData[4096], msg[256];
	DWORD lpcData;
	DWORD dwValueType = REG_SZ;
	
	lpcData=4096;
	if (RegQueryValue(HKEY_CLASSES_ROOT, subkey, &lpData, &lpcData) == ERROR_SUCCESS)
	{
		if (lpcData > 200)
		{
			if (lpData[220] == 0xd && lpData[221] == 0xa)
			{
				foundkeys++;
				if (autokeydel == 0)
				{
					wsprintf(msg,"The following seems to be a Vbox Registry Key:\n\nHKEY_CLASSES_ROOT\\%s\n\nDelete this key?",subkey);
					if (MessageBox(Handle, msg, "Confirm Registry key deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
					{
						if (RegDeleteKey(HKEY_CLASSES_ROOT, subkey) == ERROR_SUCCESS)
						{
							delkeys++;
						}
					}
				}
				else
				{
					if (RegDeleteKey(HKEY_CLASSES_ROOT, subkey) == ERROR_SUCCESS)
					{
						delkeys++;
					}
				}	
			}
		}
	}
	return;
}

int scanregistry()
{
	char lpData[4096],value[4096];
	char lpName[255],lpName2[255],msg[256],subkey[255];
	HANDLE hKey,hSubKey;
	DWORD dwValueType = REG_SZ;
	DWORD lpcData = 4096;
	DWORD lpcName = 255;
	DWORD lpcName2 = 255;
	DWORD lpcSubKeys;
	DWORD lpcValues;
	int g,h,i,k,t,u,len;
	PFILETIME wtime,wtime2;


//************************************
// Search Registry: HKEY_CLASSES_ROOT
//************************************

	SetWindowText(label_text2, "Searching Registry:\nHKEY_CLASSES_ROOT");
	i=0;
	g=0;
	foundkeys=0;
	delkeys=0;
	while (1)
	{
		lpcName=255;
		t=RegEnumKeyEx(HKEY_CLASSES_ROOT, i, &lpName, &lpcName, NULL, NULL, NULL, &wtime);
		if (t != ERROR_SUCCESS)
		{
			break;
		}
		if (lpcName <= 5 && lpName[0] == 0x2e)
		{
			wsprintf(msg,"Searching Registry:\nHKEY_CLASSES_ROOT\\%s",lpName);
			SetWindowText(label_text2, msg);
			if (RegOpenKeyEx(HKEY_CLASSES_ROOT, &lpName, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
			{
				h=0;
				while (1)
				{
					lpcName2=255;
					u=RegEnumKeyEx(hKey, h, &lpName2, &lpcName2, NULL, NULL, NULL, &wtime2);
					if (u != ERROR_SUCCESS)
					{
						break;
					}
					if (lpcName2 == 38 && lpName2[0] == 0x7b && lpName2[37] == 0x7d)
					{
						wsprintf(subkey, "%s\\%s", &lpName, &lpName2);
						getvalue(subkey);
					}
					h++;
				}
				RegCloseKey(hKey);
			}
		}
		i++;
	}

//Delete Vbox file extensions
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, ".vdc3", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		foundkeys+=2;
		lpcData=255;
		RegQueryValue(HKEY_CLASSES_ROOT, ".vdc3", &lpData, &lpcData);
		RegCloseKey(hKey);

		if (autokeydel == 0)
		{
			if (MessageBox(Handle, "Delete \"Vbox Download Container\" file extension (HKEY_CLASSES_ROOT\\.vdc3) ?", "Confirm Registry key deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				if (RegDeleteKey(HKEY_CLASSES_ROOT, ".vdc3") == ERROR_SUCCESS)
				{
					delkeys++;
				}
				if (RegDeleteKey(HKEY_CLASSES_ROOT, lpData) == ERROR_SUCCESS)
				{
					delkeys++;
				}
			}
		}
		else
		{
			if (RegDeleteKey(HKEY_CLASSES_ROOT, ".vlf3") == ERROR_SUCCESS)
			{
				delkeys++;
			}
			if (RegDeleteKey(HKEY_CLASSES_ROOT, lpData) == ERROR_SUCCESS)
			{
				delkeys++;
			}
		}
	}
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, ".vlf3", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		foundkeys+=2;
		lpcData=255;
		RegQueryValue(HKEY_CLASSES_ROOT, ".vlf3", &lpData, &lpcData);
		RegCloseKey(hKey);
		if (autokeydel == 0)
		{
			if (MessageBox(Handle, "Delete \"Vbox License File\" file extension (HKEY_CLASSES_ROOT\\.vlf3) ?", "Confirm Registry key deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				if (RegDeleteKey(HKEY_CLASSES_ROOT, ".vlf3") == ERROR_SUCCESS)
				{
					delkeys++;
				}
				if (RegDeleteKey(HKEY_CLASSES_ROOT, lpData) == ERROR_SUCCESS)
				{
					delkeys++;
				}
			}
		}
		else
		{
			if (RegDeleteKey(HKEY_CLASSES_ROOT, ".vlf3") == ERROR_SUCCESS)
			{
				delkeys++;
			}
			if (RegDeleteKey(HKEY_CLASSES_ROOT, lpData) == ERROR_SUCCESS)
			{
				delkeys++;
			}
		}
	}


//******************************************
// Search Registry: HKEY_CLASSES_ROOT\CLSID
//******************************************

	SetWindowText(label_text2, "Searching Registry:\nHKEY_CLASSES_ROOT\\CLSID");
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, "CLSID", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		i=0;
		while (1)
		{
			lpcName=255;
			t=RegEnumKeyEx(hKey, i, &lpName, &lpcName, NULL, NULL, NULL, &wtime);
			if (t != ERROR_SUCCESS)
			{
				break;
			}

			wsprintf(msg,"Searching Registry:\nHKCR\\CLSID\\%s",&lpName);
			SetWindowText(label_text2, msg);

			if (lpcName == 38 && lpName[0] == 0x7b && lpName[37] == 0x7d)
			{
				if (RegOpenKeyEx(hKey, &lpName, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS)
				{
					if (RegQueryInfoKey(hSubKey, NULL, NULL, NULL, &lpcSubKeys, NULL, NULL, &lpcValues, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
					{
						if (lpcSubKeys == 0 && lpcValues == 1)
						{
							wsprintf(subkey, "CLSID\\%s", &lpName);
							getvalue(subkey);
						}
					}
					RegCloseKey(hSubKey);
				}
			}
			i++;
		}
		RegCloseKey(hKey);
	}

	SetWindowText(label_text2, "Searching Registry:\nHKEY_LOCAL_MACHINE\\Software\\Preview Systems");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Preview Systems", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		foundkeys++;
		RegCloseKey(hKey);
		if (autokeydel == 0)
		{
			if (MessageBox(Handle, "Delete Registry key \"HKEY_LOCAL_MACHINE\\Software\\Preview Systems\" ?", "Confirm Registry key deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				if (RegDeleteKey(HKEY_LOCAL_MACHINE, "Software\\Preview Systems") == ERROR_SUCCESS)
				{
					delkeys++;
				}
			}
		}
		else
		{
			if (RegDeleteKey(HKEY_LOCAL_MACHINE, "Software\\Preview Systems") == ERROR_SUCCESS)
			{
				delkeys++;
			}
		}
	}

	SetWindowText(label_text2, "Searching Registry:\nHKEY_CURRENT_USER\\Software\\PreviewSoft");
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\PreviewSoft", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		foundkeys++;

		if (autokeydel == 0)
		{
			if (MessageBox(Handle, "Delete Registry key \"HKEY_CURRENT_USER\\Software\\PreviewSoft\" ?", "Confirm Registry key deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				if (RegDeleteKey(HKEY_CURRENT_USER, "Software\\PreviewSoft") == ERROR_SUCCESS)
				{
					delkeys++;
				}
			}
		}
		else
		{
			if (RegDeleteKey(HKEY_CURRENT_USER, "Software\\PreviewSoft") == ERROR_SUCCESS)
			{
				delkeys++;
			}
		}
	}
}

int del_wsocx()
{
	typedef struct _WIN32_FIND_DATA
	{
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		TCHAR    cFileName[ MAX_PATH ];
		TCHAR    cAlternateFileName[ 14 ];
	} WIN32_FIND_DATA, *PWIN32_FIND_DATA; 

	int len,n,i,ispene;
	unsigned int p;
	HANDLE hSearch,file;
	char sysdir[MAX_PATH],searchname[MAX_PATH],filename[MAX_PATH];
	char msg[256];
	unsigned char buf[4096];
	WIN32_FIND_DATA FindData;

	i=0;
	len=GetSystemDirectory(sysdir, MAX_PATH);
	if (len == 0)
	{
		MessageBox(Handle, "Could not find SYSTEM directory", "Error", MB_OK|MB_ICONSTOP);
		return;
	}
	wsprintf(searchname, "%s\\ws??????.ocx", &sysdir);

	wsprintf(msg, "Searching for files:\n%s", searchname);
	SetWindowText(label_text2, msg);

	hSearch=FindFirstFile(searchname, &FindData);
	SetCurrentDirectory(sysdir);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		ispene=0;
		lstrcpy(filename,FindData.cFileName);
		file=CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(file != HFILE_ERROR)
		{
			ReadFile(file,&buf,0x400,&n,NULL);
			if (buf[0] == 0x4d && buf[1] == 0x5a)
			{
				p=buf[0x3c]+(buf[0x3d]*0x100)+(buf[0x3e]*0x10000)+(buf[0x3f]*0x1000000);
				if (p < n)
				{
					if (buf[p] == 0x50 && buf[p+1] == 0x45)
					{
						ispene=1;
					}
					if (buf[p] == 0x4e && buf[p+1] == 0x45)
					{
						ispene=1;
					}
				}
			}
			CloseHandle(file);
		}
		
		if (ispene == 0)
		{
			i++;
			if (autofiledel == 0)
			{
				wsprintf(msg,"The following file seems to be a Vbox data file:\n\n%s\\%s (%d Bytes)\n\nDelete it?", &sysdir, &filename,FindData.nFileSizeLow);
				if (MessageBox(Handle, msg, "Confirm file deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
					if (!DeleteFile(filename))
					{
						wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &sysdir, &filename);
						if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
						{
							addtowininit(sysdir, filename);
						}
					}
					else
						delfiles++;
				}
			}
			else
			{
				SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
				if (!DeleteFile(filename))
				{
					wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &sysdir, &filename);
					if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
					{
						addtowininit(sysdir, filename);
					}
				}
				else
					delfiles++;
			}	
		}		
		if (FindNextFile(hSearch, &FindData) == 0)
			break;
	}
	return i;
}

int del_osbin()
{
	typedef struct _WIN32_FIND_DATA
	{
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		TCHAR    cFileName[ MAX_PATH ];
		TCHAR    cAlternateFileName[ 14 ];
	} WIN32_FIND_DATA, *PWIN32_FIND_DATA; 

	int len,i,ispene;
	HANDLE hSearch,file;
	char sysdir[MAX_PATH],searchname[MAX_PATH],dir[MAX_PATH],filename[MAX_PATH];
	char msg[256];
	WIN32_FIND_DATA FindData;

	i=0;
	len=GetSystemDirectory(sysdir, MAX_PATH);
	if (len == 0)
	{
		sysdir[0]="C";	
	}
	wsprintf(searchname, "%c:\\os??????.bin", sysdir[0]);
	hSearch=FindFirstFile(searchname, &FindData);
	wsprintf(msg, "Searching for files:\n%s", searchname);
	SetWindowText(label_text2, msg);
	wsprintf(dir, "%c:\\", sysdir[0]);
	SetCurrentDirectory(dir);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		lstrcpy(filename,FindData.cFileName);
		i++;
		if (autofiledel == 0)
		{
			wsprintf(msg,"The following file seems to be a Vbox data file:\n\n%s%s (%d Bytes)\n\nDelete the file?", &dir, &filename, FindData.nFileSizeLow);
			if (MessageBox(Handle, msg, "Confirm file deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
				if (!DeleteFile(filename))
				{
					wsprintf(msg, "Could not delete file %s%s. Do you want the file to be deleted on next Windows startup?", &dir, &filename);
					if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
					{
						addtowininit(dir, filename);
					}
				}
				else
					delfiles++;
			}
		}
		else
		{
			SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
			if (!DeleteFile(filename))
			{
				wsprintf(msg, "Could not delete file %s%s. Do you want the file to be deleted on next Windows startup?", &dir, &filename);
				if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
				{
					addtowininit(dir, filename);
				}
			}
			else
				delfiles++;
		}
		
		if (FindNextFile(hSearch, &FindData) == 0)
			break;
	}
	return i;
}

int del_vboxdlls()
{
	typedef struct _WIN32_FIND_DATA
	{
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		TCHAR    cFileName[ MAX_PATH ];
		TCHAR    cAlternateFileName[ 14 ];
	} WIN32_FIND_DATA, *PWIN32_FIND_DATA; 

	int len,n,i,ispene;
	unsigned int p;
	HANDLE hSearch,file;
	char sysdir[MAX_PATH],searchname[MAX_PATH],filename[MAX_PATH];
	char msg[256];
	unsigned char buf[4096];
	WIN32_FIND_DATA FindData;

	i=0;
	len=GetSystemDirectory(sysdir, MAX_PATH);
	if (len == 0)
	{
		MessageBox(Handle, "Could not find SYSTEM directory", "Error", MB_OK|MB_ICONSTOP);
		return;
	}
	wsprintf(searchname, "%s\\vbox????.dll", &sysdir);

	wsprintf(msg, "Searching for files:\n%s", searchname);
	SetWindowText(label_text2, msg);

	hSearch=FindFirstFile(searchname, &FindData);
	SetCurrentDirectory(sysdir);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		ispene=0;
		lstrcpy(filename,FindData.cFileName);

		i++;
		if (autofiledel == 0)
		{
			wsprintf(msg,"Delete Vbox DLL \"%s\\%s\" ?", &sysdir, &filename);
			if (MessageBox(Handle, msg, "Confirm file deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
				if (!DeleteFile(filename))
				{
					wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &sysdir, &filename);
					if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
					{
						addtowininit(sysdir, filename);
					}
				}
				else
					delfiles++;
			}
		}
		else
		{
			SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
			if (!DeleteFile(filename))
			{
				wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &sysdir, &filename);
				if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
				{
					addtowininit(sysdir, filename);
				}
			}
		else
				delfiles++;
		}	

		if (FindNextFile(hSearch, &FindData) == 0)
			break;
	}
	return i;
}

void searchsubdir(char dirname[MAX_PATH], char subdirname[MAX_PATH])
{
	typedef struct _WIN32_FIND_DATA
	{
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		TCHAR    cFileName[ MAX_PATH ];
		TCHAR    cAlternateFileName[ 14 ];
	} WIN32_FIND_DATA, *PWIN32_FIND_DATA; 

	HANDLE hSearch;
	char msg[256],searchname[MAX_PATH],filename[MAX_PATH],dir[MAX_PATH],ftype[256];
	WIN32_FIND_DATA FindData;
	
	wsprintf(dir,"%s\\%s",dirname,subdirname);
	wsprintf(searchname, "%s\\*.*", &dir);
	wsprintf(msg, "Searching %s", searchname);
	SetWindowText(label_text2, msg);
	SetCurrentDirectory(dir);
	hSearch=FindFirstFile(searchname, &FindData);

	while (hSearch != INVALID_HANDLE_VALUE)
	{
		lstrcpy(filename,FindData.cFileName);

		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if ( lstrcmp(filename,".")!=0 && lstrcmp(filename,"..")!=0)
			{
				searchsubdir(dir, filename);
				SetCurrentDirectory(dir);
				RemoveDirectory(filename);
			}
		}
		else
		{	
			foundfiles++;
			lstrcpy(ftype, "file ");
			if (lstrcmpi(subdirname,"common") == 0)
			{
				lstrcpy(ftype,"Vbox Common file ");
			}
			if (lstrcmpi(subdirname,"data") == 0)
			{
				lstrcpy(ftype,"Vbox Data file ");
			}
			if (lstrcmpi(subdirname,"licenses") == 0)
			{
				lstrcpy(ftype,"Vbox License file ");
			}
			if (lstrcmpi(subdirname,"packingslips") == 0)
			{
				lstrcpy(ftype,"Vbox PackingSlips file ");
			}
			
			if (autofiledel == 0)
			{
				wsprintf(msg, "Delete %s\"%s\\%s\"?", &ftype, &dir, &filename);
				if (MessageBox(Handle, msg, "Confirm file deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
					if (!DeleteFile(filename))
					{
						wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
						if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
						{
							addtowininit(dir, filename);
						}
					}
					else
						delfiles++;
				}
			}
			else
			{
				SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
				if (!DeleteFile(filename))
				{
					wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
					if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
					{
						addtowininit(dir, filename);
					}
				}
				else
					delfiles++;
			}
		}
		if (FindNextFile(hSearch, &FindData) == 0)
			break;
	}
}

void start()
{
	typedef struct _WIN32_FIND_DATA
	{
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		TCHAR    cFileName[ MAX_PATH ];
		TCHAR    cAlternateFileName[ 14 ];
	} WIN32_FIND_DATA, *PWIN32_FIND_DATA; 

	char lpData[255];
	HANDLE hKey;
	DWORD dwValueType = REG_SZ;
	DWORD lpcData = 256;
	int len;
	int lvccmdline;
	char vccmdline[255], vbox_cleaner_path[255];
	HANDLE hSearch,file;
	char sysdir[MAX_PATH],searchname[MAX_PATH],windir[MAX_PATH],commondir[MAX_PATH],dir[MAX_PATH],filename[MAX_PATH];
	char msg[256];
	unsigned char buf[4096];
	WIN32_FIND_DATA FindData;

	foundfiles=0;
	delfiles=0;
	initdel=0;

	if (SendMessage(button_Check1,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		autofiledel=1;
	}
	else
	{
		autofiledel=0;
	}

	if (SendMessage(button_Check2,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		autokeydel=1;
	}
	else
	{
		autokeydel=0;
	}

     // look for ws******.ocx in WINDOWS\SYSTEM	
	foundfiles+=del_wsocx();
     // look for vbox****.dll in WINDOWS\SYSTEM
	foundfiles+=del_vboxdlls();
     // look for os******.bin on system drive
	foundfiles+=del_osbin();
	
	len=GetWindowsDirectory(windir, MAX_PATH);
	if (len == 0)
	{
		MessageBox(Handle, "Could not find WINDOWS directory", "Error", MB_OK|MB_ICONSTOP);
		return;
	}


//********************
// check WINDOWS\Vbox
//********************

	wsprintf(dir,"%s\\Vbox",&windir);
	wsprintf(searchname, "%s\\*.*", &dir);
	wsprintf(msg, "Searching %s", searchname);
	SetWindowText(label_text2, msg);
	SetCurrentDirectory(dir);
	hSearch=FindFirstFile(searchname, &FindData);

	while (hSearch != INVALID_HANDLE_VALUE)
	{
		lstrcpy(filename,FindData.cFileName);
		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if ( lstrcmp(filename,".")!=0 && lstrcmp(filename,"..")!=0)
			{
				searchsubdir(dir, filename);
				SetCurrentDirectory(dir);
				RemoveDirectory(filename);
			}
		}
		else
		{	
			foundfiles++;
			if (autofiledel == 0)
			{
				wsprintf(msg, "Delete file \"%s\\%s\"?", &dir, &filename);
				if (MessageBox(Handle, msg, "Confirm file deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
					if (!DeleteFile(filename))
					{
						wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
						if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
						{
							addtowininit(dir, filename);
						}
					}
					else
						delfiles++;
				}
			}
			else
			{
				SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
				if (!DeleteFile(filename))
				{
					wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
					if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
					{
						addtowininit(dir, filename);
					}
				}
				else
					delfiles++;
			}
		}
		if (FindNextFile(hSearch, &FindData) == 0)
			break;
	}
	SetCurrentDirectory(windir);
	RemoveDirectory("Vbox");

//***************************
// check WINDOWS\PreviewSoft
//***************************

	wsprintf(dir,"%s\\PreviewSoft",&windir);
	wsprintf(searchname, "%s\\*.*", &dir);
	wsprintf(msg, "Searching %s", searchname);
	SetWindowText(label_text2, msg);
	SetCurrentDirectory(dir);
	hSearch=FindFirstFile(searchname, &FindData);

	while (hSearch != INVALID_HANDLE_VALUE)
	{
		lstrcpy(filename,FindData.cFileName);
		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if ( lstrcmp(filename,".")!=0 && lstrcmp(filename,"..")!=0)
			{
				searchsubdir(dir, filename);
				SetCurrentDirectory(dir);
				RemoveDirectory(filename);
			}
		}
		else
		{	
			foundfiles++;
			if (autofiledel == 0)
			{
				wsprintf(msg, "Delete file \"%s\\%s\"?", &dir, &filename);
				if (MessageBox(Handle, msg, "Confirm file deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
					if (!DeleteFile(filename))
					{
						wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
						if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
						{
							addtowininit(dir, filename);
						}
					}
					else
						delfiles++;
				}
			}
			else
			{
				SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
				if (!DeleteFile(filename))
				{
					wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
					if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
					{
						addtowininit(dir, filename);
					}
				}
				else
					delfiles++;
			}
		}
		if (FindNextFile(hSearch, &FindData) == 0)
			break;
	}
	SetCurrentDirectory(windir);
	RemoveDirectory("PreviewSoft");

//*************************
// check COMMON FILES\Vbox
//*************************

	if (RegCreateKey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion", &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, "CommonFilesDir", NULL, &dwValueType, &lpData, &lpcData) == ERROR_SUCCESS)
		{
			wsprintf(commondir, "%s", &lpData);
		}
	}
	RegCloseKey(hKey);

	wsprintf(dir,"%s\\Vbox",&commondir);
	wsprintf(searchname, "%s\\*.*", &dir);
	wsprintf(msg, "Searching %s", searchname);
	SetWindowText(label_text2, msg);
	SetCurrentDirectory(dir);
	hSearch=FindFirstFile(searchname, &FindData);

	while (hSearch != INVALID_HANDLE_VALUE)
	{
		lstrcpy(filename,FindData.cFileName);
		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if ( lstrcmp(filename,".")!=0 && lstrcmp(filename,"..")!=0)
			{
				searchsubdir(dir, filename);
				SetCurrentDirectory(dir);
				RemoveDirectory(filename);
			}
		}
		else
		{	
			foundfiles++;
			if (autofiledel == 0)
			{
				wsprintf(msg, "Delete file \"%s\\%s\"?", &dir, &filename);
				if (MessageBox(Handle, msg, "Confirm file deletion", MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
					if (!DeleteFile(filename))
					{
						wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
						if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
						{
							addtowininit(dir, filename);
						}
					}
					else
						delfiles++;
				}
			}
			else
			{
				SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
				if (!DeleteFile(filename))
				{
					wsprintf(msg, "Could not delete file %s\\%s (might be in use). Do you want the file to be deleted on next Windows startup?", &dir, &filename);
					if (MessageBox(Handle, msg, "Error", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
					{
						addtowininit(dir, filename);
					}
				}
				else
					delfiles++;
			}
		}
		if (FindNextFile(hSearch, &FindData) == 0)
			break;
	}
	SetCurrentDirectory(commondir);
	RemoveDirectory("Vbox");

	scanregistry();

	SetWindowText(label_text2, "");
	
	if (foundfiles == 0 && foundkeys == 0)
	{
		MessageBox(Handle, "Could not find any Vbox files/Registry entries on your system.","Info",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		if (foundfiles == 1)
		{
			if (foundkeys == 1)
			{
				wsprintf(msg,"SUMMARY\n\n%d Vbox-related file found, %d deleted\n%d Vbox-related Registry key found, %d deleted", foundfiles, delfiles, foundkeys, delkeys);
			}
			else
			{
				wsprintf(msg,"SUMMARY\n\n%d Vbox-related file found, %d deleted\n%d Vbox-related Registry keys found, %d deleted", foundfiles, delfiles, foundkeys, delkeys);
			}
		}
		else
		{
			if (foundkeys == 1)
			{
				wsprintf(msg,"SUMMARY\n\n%d Vbox-related files found, %d of them deleted\n%d Vbox-related Registry key found, %d deleted", foundfiles, delfiles, foundkeys, delkeys);
			}
			else
			{
				wsprintf(msg,"SUMMARY\n\n%d Vbox-related files found, %d of them deleted\n%d Vbox-related Registry keys found, %d deleted", foundfiles, delfiles, foundkeys, delkeys);
			}
		}
		MessageBox(Handle, msg, "Info", MB_OK|MB_ICONINFORMATION);
	}
	
	if (initdel > 0)
	{
		if (initdel == 1)
		{
			wsprintf(msg, "%d file could not be removed and you want it to be deleted on next Windows startup.\n\nClick OK to restart your Computer now. If you want to restart later remember to run Vbox Cleaner again.", initdel);
		}
		else
		{
			wsprintf(msg, "%d files could not be removed and you want them to be deleted on next Windows startup.\n\nClick OK to restart your Computer now. If you want to restart later remember to run Vbox Cleaner again.", initdel);
		}
		if (MessageBox(Handle, msg, "System Reboot", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
		{
			if (RegCreateKey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", &hKey) == ERROR_SUCCESS)
			{
				GetModuleFileName(NULL, vbox_cleaner_path, 255);
				if (autofiledel == 0 && autokeydel == 0)
					lvccmdline=wsprintf(vccmdline, "%s /scan", vbox_cleaner_path);
				if (autofiledel == 1 && autokeydel == 0)
					lvccmdline=wsprintf(vccmdline, "%s /scan -delf", vbox_cleaner_path);
				if (autofiledel == 0 && autokeydel == 1)
					lvccmdline=wsprintf(vccmdline, "%s /scan -delr", vbox_cleaner_path);
				if (autofiledel == 1 && autokeydel == 1)
					lvccmdline=wsprintf(vccmdline, "%s /scan -delf -delr", vbox_cleaner_path);
				if (RegSetValueEx(hKey, "Vbox Cleanup", 0, REG_SZ, vccmdline, lvccmdline) != ERROR_SUCCESS)
				{
					MessageBox(Handle, "Remember to run Vbox Cleaner after this reboot.", "Note", MB_OK|MB_ICONINFORMATION);
				}
			}
			RegCloseKey(hKey);

			if (ExitWindowsEx(EWX_REBOOT, 0) == 0)
			{
				MessageBox(Handle, "Please reboot your computer now to finish removal.", "Reboot computer", MB_OK|MB_ICONINFORMATION);
			}
		}
			
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
		if (uMsg == WM_COMMAND)
		{
			if (lParam == button_Begin1)
				start();
			if (lParam == button_About1)
				MessageBox(Handle, "Vbox Cleaner 1.0, (c) 2001 by Bud [AmoK]\n\nThis program is distributed under terms of Gnu Publical License.\n\nRefer to vbox_cleaner.txt for additional information.","About Vbox Cleaner",MB_OK|MB_ICONINFORMATION);
			if (lParam == button_Close1)
				exit(0);
		}
		if (uMsg == WM_DESTROY)
			exit(0);
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char *CmdLine, int CmdShow)
{
	typedef struct _WNDCLASS
	{	UINT       style;
		WNDPROC    lpfnWndProc; 
		int        cbClsExtra;
		int        cbWndExtra;
		HINSTANCE  hInstance;
		HICON      hIcon;
		HCURSOR    hCursor;
		HBRUSH     hbrBackground;
		LPCTSTR    lpszMenuName;
		LPCTSTR    lpszClassName;
	} WNDCLASS, *PWNDCLASS;
	
	typedef struct tagMSG
	{
		HWND   hwnd;
		UINT   message;
		WPARAM wParam;
		LPARAM lParam;
		DWORD  time;
		POINT  pt;
	} MSG, *PMSG;
	
	WNDCLASS WinClass;
	LPMSG lpMsg;
	int xOffset,yOffset;
	
	WinClass.style = CS_CLASSDC | CS_PARENTDC;
	WinClass.lpfnWndProc = (WNDPROC)WindowProc;
	WinClass.cbClsExtra = 0;
	WinClass.cbWndExtra = 0;
	WinClass.hInstance = hInst;
	WinClass.hIcon = LoadIcon (hInst, "MAIN");
	WinClass.hCursor = LoadCursor(0, IDC_ARROW);
	WinClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	WinClass.lpszMenuName  = NULL;
	WinClass.lpszClassName = "Vbox_cleaner_class";
	
	if (!RegisterClass(&WinClass))
		exit(0);

	xOffset=GetSystemMetrics(SM_CXSCREEN);
	yOffset=GetSystemMetrics(SM_CYSCREEN);

	xOffset=(xOffset-340)/2;
	yOffset=(yOffset-220)/2;

	Handle = CreateWindowEx(WS_EX_TOPMOST, "Vbox_cleaner_class", "Vbox Cleaner 1.0", WS_VISIBLE | WS_CAPTION | WS_SYSMENU, xOffset, yOffset, 340, 220, 0, 0, hInst, NULL);

	button_Begin1 = CreateWindow("Button", "Scan", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_DEFPUSHBUTTON, 10, 162, 75, 23, Handle, 0, hInst, NULL);

	button_About1 = CreateWindow("Button", "About...", WS_VISIBLE | WS_CHILD | BS_TEXT, 130, 162, 75, 23, Handle, 0, hInst, NULL);

	button_Close1 = CreateWindow("Button", "Exit", WS_VISIBLE | WS_CHILD | BS_TEXT, 250, 162, 75, 23, Handle, 0, hInst, NULL);

	button_Check1 = CreateWindow("Button", "Automatically delete detected Vbox files", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_AUTOCHECKBOX, 10, 110, 206, 16, Handle, 0, hInst, NULL);

	button_Check2 = CreateWindow("Button", "Automatically delete detected Vbox Registry keys", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_AUTOCHECKBOX, 10, 130, 252, 16, Handle, 0, hInst, NULL);

	label_text1 = CreateWindow("Static", "This tool is intended to detect and remove Vbox files and Registry entries from your system.\n\nNOTE: You use this tool at your own risk.", WS_VISIBLE | WS_CHILD, 8, 11, 316, 56, Handle, 0, hInst, NULL);

	label_text2 = CreateWindow("Static", "", WS_VISIBLE | WS_CHILD, 16, 70, 316, 36, Handle, 0, hInst, NULL);

	hFont1 = CreateFont(-11, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	if ( hFont1 != 0 )
	{
		SendMessage(label_text1, WM_SETFONT, hFont1, 0);
		SendMessage(label_text2, WM_SETFONT, hFont1, 0);
		SendMessage(button_Check1, WM_SETFONT, hFont1, 0);
		SendMessage(button_Check2, WM_SETFONT, hFont1, 0);
		SendMessage(button_Begin1, WM_SETFONT, hFont1, 0);
		SendMessage(button_About1, WM_SETFONT, hFont1, 0);
		SendMessage(button_Close1, WM_SETFONT, hFont1, 0);
  	}
	
	SetFocus(button_Begin1);
	UpdateWindow(Handle);

	if (lstrcmp(CmdLine, "/scan") == 0)
	{
		if (MessageBox(Handle, "Vbox Cleaner will now scan for Vbox files/Registry keys.\n\nClick OK to begin.", "Vbox Cleaner Notification", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
		{
			start();
		}
	}

	if (lstrcmp(CmdLine, "/scan -delf") == 0)
	{
		if (MessageBox(Handle, "Vbox Cleaner will now scan for Vbox files/Registry keys.\nDetected Vbox files will be deleted automatically.\n\nClick OK to begin.", "Vbox Cleaner Notification", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
		{
			SendMessage(button_Check1,BM_SETCHECK,BST_CHECKED,0);
			start();
		}
	}

	if (lstrcmp(CmdLine, "/scan -delr") == 0)
	{
		if (MessageBox(Handle, "Vbox Cleaner will now scan for Vbox files/Registry keys.\nDetected Vbox Registry keys will be deleted automatically.\n\nClick OK to begin.", "Vbox Cleaner Notification", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
		{
			SendMessage(button_Check2,BM_SETCHECK,BST_CHECKED,0);
			start();
		}
	}

	if (lstrcmp(CmdLine, "/scan -delf -delr") == 0)
	{
		if (MessageBox(Handle, "Vbox Cleaner will now scan for Vbox files/Registry keys.\nDetected Vbox files and Registry keys will be deleted automatically.\n\nClick OK to begin.", "Vbox Cleaner Notification", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
		{
			SendMessage(button_Check1,BM_SETCHECK,BST_CHECKED,0);
			SendMessage(button_Check2,BM_SETCHECK,BST_CHECKED,0);
			start();
		}
	}

	while (GetMessage(&lpMsg, Handle, 0, 0))
	{
		TranslateMessage(&lpMsg);	
		DispatchMessage(&lpMsg);
	}
}