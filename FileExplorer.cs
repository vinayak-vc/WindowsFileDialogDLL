using System;
using System.Runtime.InteropServices;

public static class FileExplorer
{
    [DllImport("WindowsFileDialog", CharSet = CharSet.Unicode)]
    private static extern IntPtr OpenWindowsFile();

    [DllImport("WindowsFileDialog", CharSet = CharSet.Unicode)]
    private static extern IntPtr OpenFileWithExtension(string extensions);
    
    [DllImport("WindowsFileDialog", CharSet = CharSet.Unicode)]
    private static extern IntPtr OpenFolderDialog(string extensions);

    public static string OpenFileExplorer() {
        IntPtr ptr = OpenWindowsFile();
        return Marshal.PtrToStringUni(ptr);
    }

    public static string OpenFileExplorer(string filter) {
        IntPtr ptr = OpenFileWithExtension(filter);
        return Marshal.PtrToStringUni(ptr);
    }
    
    public static string OpenFolder(string filter) {
        IntPtr ptr = OpenFolderDialog(filter);
        return Marshal.PtrToStringUni(ptr);
    }
}