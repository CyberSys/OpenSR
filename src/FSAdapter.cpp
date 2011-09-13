/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FSAdapter.h"
#ifdef WIN32
#include <windows.h>
#include <wchar.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#include <errno.h>
#include <libRanger.h>
#include "Log.h"
#include <algorithm>


using namespace Rangers;
using namespace std;

list< wstring > FSAdapter::getFiles() const
{
    return files;
}

void FSAdapter::load(const std::wstring& path)
{
    dirPath = path;
    if(dirPath.at(path.length() - 1) != '/')
        dirPath += '/';
#ifdef WIN32
    doScan(L"");
#else
    doScan("");
#endif
    Log::info() << "Loaded " << files.size() << " files from directory " << path;
}

#ifdef WIN32
void FSAdapter::doScan(const wstring& path)
{
    wstring localPath = dirPath + path;
    WIN32_FIND_DATAW fd;
    HANDLE fh = FindFirstFileW((LPCWSTR)(localPath + L"*").c_str(), &fd);

    if(fh == INVALID_HANDLE_VALUE)
    {
	    Log::error() << "Cannot open directory " << localPath << ": " << fromLocal(strerror(errno));
	    return;
    }
    do
    {
        if(!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L".."))
	    {
	        continue;
	    }            
        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            doScan(path + fd.cFileName + L'\\');
        else 
		{
			wstring fileName = path + fd.cFileName;
			std::replace(fileName.begin(), fileName.end(), L'\\', L'/');
            files.push_back(fileName);
		}
    } 
    while(FindNextFileW(fh, &fd));
}
#else
void FSAdapter::doScan(const string& path)
{
    string localPath = toLocal(dirPath) + path;
    DIR* dir = opendir(localPath.c_str());
    if(!dir)
    {
	Log::error() << "Cannot open directory " << fromLocal(localPath.c_str()) << ": " << fromLocal(strerror(errno));
	return;
    }
    dirent *current;
    while(current = readdir(dir))
    {
	if(!strcmp(current->d_name,".") || !strcmp(current->d_name,".."))
	{
	    continue;
	}
	string fullPath = localPath + current->d_name;
	struct stat entry;
	if(!lstat(fullPath.c_str(), &entry))
	{
	    
	    switch(entry.st_mode & S_IFMT)
	    {
	    case S_IFDIR:
	        doScan(path + current->d_name + '/');
		break;
	    case S_IFREG:
	        files.push_back(fromLocal((path + current->d_name).c_str()));
		break;
	    default:
	        Log::debug() << fromLocal(fullPath.c_str()) << " unknown entry type";
	    }
	}
	else
	{
	     Log::debug() << "Cannot lstat: " << fromLocal(strerror(errno));
	}
    }
    closedir(dir);
}
#endif

char* FSAdapter::loadData(const std::wstring& name, size_t& size)
{
    ifstream s(toLocal(dirPath + name).c_str(), ios::in | ios::binary);
    if(!s.is_open())
    {
	Log::error() << "Cannot load file from FS: " << fromLocal(strerror(errno));
	size = 0;
	return 0;
    }
    s.seekg(0, ios_base::end);
    size = s.tellg();
    s.seekg(0, ios_base::beg);
    char *data = new char[size];
    s.read(data, size);
    s.close();
    return data;
}

FSAdapter::~FSAdapter()
{

}