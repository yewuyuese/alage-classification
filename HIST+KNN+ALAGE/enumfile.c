
#include <windows.h>
#include <tchar.h>
#include <iostream>

//遍历文件夹函数
void enumfile(LPCTSTR lpPath)
{
    TCHAR szFind[MAX_PATH] = {_T("\0")};
    WIN32_FIND_DATA findFileData;
    BOOL bRet;
 
    _tcscpy_s(szFind, MAX_PATH, lpPath);
    _tcscat_s(szFind, _T("\\*.*"));     //这里一定要指明通配符，不然不会读取所有文件和目录
 
    HANDLE hFind = ::FindFirstFile(szFind, &findFileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return;
    }
 
    //遍历文件夹
	FILE *fp;
	fopen_s(&fp,"file_list","w+");
    while (TRUE)
    {
        if (findFileData.cFileName[0] != _T('.'))
        {//不是当前路径或者父目录的快捷方式
            _tprintf(_T("%s\\%s\n"), lpPath, findFileData.cFileName);
			fwrite((char* )&findFileData.cFileName,16,1,fp);
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {//这是一个普通目录
                //设置下一个将要扫描的文件夹路径
                _tcscpy_s(szFind, MAX_PATH, lpPath); 
                _tcscat_s(szFind, _T("\\"));
                _tcscat_s(szFind, findFileData.cFileName);
                ///_tcscat_s(szNextDir, _T("\\*"));
                //遍历该目录
                enumfile(szFind);
            }
        }
        //如果是当前路径或者父目录的快捷方式，或者是普通目录，则寻找下一个目录或者文件
        bRet = ::FindNextFile(hFind, &findFileData);
        if (!bRet)
        {//函数调用失败
            //cout << "FindNextFile failed, error code: "
            //  << GetLastError() << endl;
            break;
        }
    }
	fclose(fp);
 
    ::FindClose(hFind);
}