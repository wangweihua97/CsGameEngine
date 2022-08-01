using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit.Mgr;

namespace CsGameEngine.Edit
{
    static class FileShowHelper
    {

        public static void RefrshFileShowList()
        {
            List<FileData> fileDatas = new List<FileData>();
            foreach(var dir in FileMgr.curGlobalDirectoryInfo.GetDirectories().OrderBy(x => x.Name))
            {
                FileData fileData = new FileData();
                fileData.globalDirectoryInfo = dir;
                fileData.isFolder = true;
                fileData.localPath = FileMgr.GlobalDirectoryInfoToLocalPath(dir);
                fileDatas.Add(fileData);
            }

            foreach (var file in FileMgr.curGlobalDirectoryInfo.GetFiles().OrderBy(x => x.Name))
            {
                FileData fileData = new FileData();
                fileData.globalFileInfo = file;
                fileData.isFolder = false;
                fileData.localPath = FileMgr.GlobalFileInfoToLocalPath(file);
                fileDatas.Add(fileData);
            }
            MainWindow.Instance.ShowFileList(fileDatas);
            MainWindow.Instance.curFolder.Text = FileMgr.curLocalFolderPath;

        }
    }
}
