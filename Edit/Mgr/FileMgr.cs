using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit;

namespace CsGameEngine.Edit.Mgr
{
    static class FileMgr
    {
        public delegate void OpenFile(string filePath);
        public static DirectoryInfo curGlobalDirectoryInfo;
        public static string curLocalFolderPath;

        public static FileInfo curSelectedGlobalFileInfo;
        public static string curSelectedLocalFilePath;
        public static Dictionary<string, OpenFile> OpenFileDelegates;


        public static void Init()
        {
            curGlobalDirectoryInfo = GetWorkDirectoryInfo();
            curLocalFolderPath = "Assets";
            OpenFileDelegates = new Dictionary<string, OpenFile>();
            CreatFileShowList();
            EventCenterMgr.AddEventListener("ResChange", OnResChange);
        }

        public static void Back()
        {
            if (curLocalFolderPath.Equals("Assets"))
                return;
            curGlobalDirectoryInfo = curGlobalDirectoryInfo.Parent;
            curLocalFolderPath = GlobalDirectoryInfoToLocalPath(curGlobalDirectoryInfo);
            EventCenterMgr.EventTrigger("ResChange");
        }

        public static void OnResChange()
        {
            CreatFileShowList();
        }

        public static void Into(DirectoryInfo directoryInfo)
        {
            curGlobalDirectoryInfo = directoryInfo;
            curLocalFolderPath = GlobalDirectoryInfoToLocalPath(curGlobalDirectoryInfo);
            CreatFileShowList();
        }

        public static FileInfo LocalPathToGlobalFileInfo(string localPath)
        {
            return new FileInfo(localPath);
        }

        public static string GlobalFileInfoToLocalPath(FileInfo globalFileInfo)
        {
            string mp = globalFileInfo.FullName.Substring(globalFileInfo.FullName.IndexOf("Assets"));
            mp = mp.Replace('\\', '/');
            return mp;
        }

        public static string GlobalDirectoryInfoToLocalPath(DirectoryInfo globalDirectoryInfo)
        {
            string mp = globalDirectoryInfo.FullName.Substring(globalDirectoryInfo.FullName.IndexOf("Assets"));
            mp = mp.Replace('\\', '/');
            return mp;
        }

        public static DirectoryInfo GetWorkDirectoryInfo()
        {
            return new DirectoryInfo("Assets");
        }

        public static void CreatFileShowList()
        {
            FileShowHelper.RefrshFileShowList();
        }

        public static string GetLocalWorkPath()
        {
            return "Assets";
        }

        public static Uri GetLocalResUri(string path)
        {
            if (string.IsNullOrEmpty(path))
                return null;
            return new Uri("pack://siteoforigin:,,,/" + path, UriKind.RelativeOrAbsolute);
        }

        public static void AddOpenFileDelegate(string suffix , OpenFile openFileDelegate)
        {
            OpenFileDelegates.Add(suffix, openFileDelegate);
        }

        public static void TryOpenFile(string localFilePath)
        {
            string suffix = localFilePath.Split('.').Last();
            if (OpenFileDelegates.ContainsKey(suffix))
                OpenFileDelegates[suffix].Invoke(localFilePath);
        }

    }
}
