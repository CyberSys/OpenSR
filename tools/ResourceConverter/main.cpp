#include <libRanger.h>
#include <fstream>
#include <iostream>
#include <list>
#include <cstring>
#include <map>

using namespace std;
using namespace Rangers;

void countFiles(std::wstring dir, PKGItem *base, std::map<RPKGEntry *, PKGItem*>& itemMap)
{
    for(int i = 0; i < base->childCount; i++)
    {
	if(base->childs[i].dataType == 3)
	    countFiles(dir + fromASCII(base->name) + L"/", &base->childs[i], itemMap);
	else
	{
	    RPKGEntry *e= new RPKGEntry;
	    e->name = dir + fromASCII(base->childs[i].name);
	    itemMap[e] = &base->childs[i];
	}
    }
}

void cleanup(PKGItem *root)
{
    for(int i = 0; i < root->childCount; i++)
    {
	if(root->childs[i].dataType == 3)
	    cleanup(&root->childs[i]);
    }
    delete[] root->childs;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " pkgarchive" << " rpkgarchive" << endl;
        return 0;
    }
    ifstream pkgfile(argv[1], ios::binary | ios::in);
    std::map<RPKGEntry*, PKGItem*> itemMap;
    PKGItem *root = loadPKG(pkgfile);
    if(root->childCount != 1)
    {
	std::cout << "Incorrect input file" << endl;
	return 0;
    }
    countFiles(L"", &root->childs[0], itemMap);
    
    //FIXME: OH SHI~
    std::vector<RPKGEntry> files;
    std::vector<PKGItem> pkgfiles;
    for(std::map<RPKGEntry*, PKGItem*>::iterator i = itemMap.begin(); i != itemMap.end(); i++)
    {
	files.push_back(*(i->first));
	pkgfiles.push_back(*(i->second));
    }
    
    int c = itemMap.size();

    uint32_t headerSize = calculateRPKGHeaderSize(files);
    ofstream outfile(argv[2], ios::binary | ios::out);
    //FIXME: Better way to fill file
    for(int i = 0; i < headerSize + 8; i++)
	outfile.put(0);
    
    for(int i = 0; i < files.size(); i++)
    {
        wcout << files[i].name << endl;
	unsigned char *data = extractFile(pkgfiles[i], pkgfile);
	appendRPKGFile(outfile, files[i], (const char*)data, pkgfiles[i].size);
	delete data;
    }
    outfile.seekp(0, ios_base::beg);
    writeRPKGHeader(outfile, files);
    
    cleanup(root);
    delete root;
    outfile.close();
    pkgfile.close();
}
