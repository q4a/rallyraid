
#include "ConfigDirectory.h"
#include "TheGame.h"
#include "stdafx.h"

bool ConfigDirectory::load(const irr::io::path& path, const irr::io::path& cfgFilename, ConfigDirectory::fileList_t& fileList)
{
    irr::io::IFileSystem* fs = TheGame::getInstance()->getDevice()->getFileSystem();
    
    irr::io::path baseDir = fs->getWorkingDirectory();
    
    bool ret = fs->changeWorkingDirectoryTo(path);
    if (ret == false)
    {
        dprintf(MY_DEBUG_WARNING, "Unable to open directory: %s\n", path.c_str());
        return false;
    }
    
    irr::io::IFileList* fl = fs->createFileList();
    
    dprintf(MY_DEBUG_NOTE, "ConfigDirectory::list(): [%s] contains %u files\n", path.c_str(), fl->getFileCount());
    
    for (unsigned int i = 0; i < fl->getFileCount(); i++)
    {
        irr::io::path dir = fl->getFileName(i);
        if (fl->isDirectory(i) && dir[0] != '.')
        {
            if ((cfgFilename.size() > 0 && fs->existFile(dir+"/"+cfgFilename)) ||
                (cfgFilename.size() == 0 && fs->existFile(dir+"/"+dir+".cfg")))
            {
                dprintf(MY_DEBUG_NOTE, "\t+ [%s]\n", dir.c_str());
                fileList.push_back(dir);
            }
            else
            {
                dprintf(MY_DEBUG_NOTE, "\t- [%s]\n", dir.c_str());
            }
        }
        else
        {
            dprintf(MY_DEBUG_NOTE, "\t- [%s]\n", dir.c_str());
        }
    }    
    
    fs->changeWorkingDirectoryTo(baseDir);
    return true;
}
