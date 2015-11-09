/* -----------------------------------------
 * ��ͷ�ļ�����enumfile������ʵ���ļ���������
 * ����ԭ�� void enumfile(LPCTSTR lpPath)
 * ����lpPath��Ҫ�������ļ���·����ע�⣺lpPath���ļ�·���á�\\����
 * enumfile��lpPathĿ¼�µ������ļ�
 *------------------------------------------
 */
#include <windows.h>
#include <tchar.h>
#include <iostream>

#ifndef ENUMFILE_H
#define ENUMFILE_H

//�����ļ��к���
int enumfile(LPCTSTR lpPath)
{
	unsigned int fileNum = 0;	//ɨ����ļ�����
    TCHAR szFind[MAX_PATH] = {_T("\0")};
    WIN32_FIND_DATA findFileData;
    BOOL bRet;
 
    _tcscpy_s(szFind, MAX_PATH, lpPath);
    _tcscat_s(szFind, _T("\\*.jpg"));     //����һ��Ҫָ��ͨ�������Ȼ�����ȡ�����ļ���Ŀ¼
 
    HANDLE hFind = ::FindFirstFile(szFind, &findFileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return fileNum;
    }
 
    //�����ļ���
	FILE *fp;
	fopen_s(&fp,"file_list","w+");
    while (TRUE)
    {
        if (findFileData.cFileName[0] != _T('.'))
        {	//���ǵ�ǰ·�����߸�Ŀ¼�Ŀ�ݷ�ʽ
//			_tprintf(_T("%s\\%s\n"), lpPath, findFileData.cFileName);
			fwrite((char* )&findFileData.cFileName,16,1,fp);	//���ļ���д��fp�ļ�
			fileNum++;
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {//����һ����ͨĿ¼
                //������һ����Ҫɨ����ļ���·��
                _tcscpy_s(szFind, MAX_PATH, lpPath); 
                _tcscat_s(szFind, _T("\\"));
                _tcscat_s(szFind, findFileData.cFileName);
                ///_tcscat_s(szNextDir, _T("\\*"));
                //������Ŀ¼
                enumfile(szFind);
            }
        }
        //����ǵ�ǰ·�����߸�Ŀ¼�Ŀ�ݷ�ʽ����������ͨĿ¼����Ѱ����һ��Ŀ¼�����ļ�
        bRet = ::FindNextFile(hFind, &findFileData);
        if (!bRet)
        {//��������ʧ��
            //cout << "FindNextFile failed, error code: "
            //  << GetLastError() << endl;
            break;
        }
    }
	fclose(fp);
 
    ::FindClose(hFind);
	return fileNum;
}

#endif