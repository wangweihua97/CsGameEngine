using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Edit
{
    public class FileData
    {
        public static int FileItemWidth = 100;
        public static int FileItemHeight = 100;
        public FileInfo globalFileInfo;
        public DirectoryInfo globalDirectoryInfo;
        public string localPath;
        public bool isFolder;
    }
}
