# Windows File & Folder Picker DLL

This repository provides a native **Windows DLL** that exposes three exported functions for use in Windows applications (including Unity and other .NET projects). These functions allow opening native **file** and **folder** selection dialogs with **Unicode support** and customizable initial directories.

---

## Features
- Open a standard file open dialog for any file.
- Open a filtered file open dialog with specific extensions.
- Open a folder selection dialog.
- Full **Unicode (wchar_t)** support.
- Optionally set the **initial directory** when dialogs open.
- Safe string handling: returned paths remain valid until the next call.
- Restores current working directory after dialog use.

---

## Exported Functions

### 1. `OpenWindowsFile`
```cpp
extern "C" __declspec(dllexport) const wchar_t* OpenWindowsFile(const wchar_t* initialDir = NULL);
```
Opens a native Windows file open dialog allowing selection of any file.

- **Parameters**:  
  - `initialDir` *(optional)*: Initial directory path. Pass `NULL` to use default.
- **Returns**: Full path of the selected file, or an empty string if cancelled.

---

### 2. `OpenFileWithExtension`
```cpp
extern "C" __declspec(dllexport) const wchar_t* OpenFileWithExtension(const wchar_t* filter, const wchar_t* initialDir = NULL);
```
Opens a file dialog filtered by file types.

- **Parameters**:  
  - `filter`: Windows filter string (e.g., `L"Text files\0*.txt\0All files\0*.*\0"`).
  - `initialDir` *(optional)*: Initial directory path.
- **Returns**: Full path of the selected file, or empty string if cancelled.

---

### 3. `OpenFolderDialog`
```cpp
extern "C" __declspec(dllexport) const wchar_t* OpenFolderDialog(const wchar_t* initialDir = NULL);
```
Opens a folder selection dialog.

- **Parameters**:  
  - `initialDir` *(optional)*: Initial folder path.
- **Returns**: Full path of the selected folder, or empty string if cancelled.

---

## Implementation Details
- Uses **Win32 APIs** (`GetOpenFileNameW`) for file dialogs.
- Uses **COM-based IFileDialog** for modern folder picking.
- Initializes and uninitializes **COM** internally.
- Returned strings are static buffers. Copy them if you need long-term storage.
- Saves and restores the current working directory to avoid side effects.

---

## Example Usage (C++)
```cpp
#include <iostream>
#include <windows.h>

int main() {
    const wchar_t* selectedFile = OpenWindowsFile(L"C:\\");
    wprintf(L"Selected file: %s\n", selectedFile);

    const wchar_t* filter = L"Text files\0*.txt\0All files\0*.*\0";
    selectedFile = OpenFileWithExtension(filter, L"C:\\Users\\");
    wprintf(L"Selected filtered file: %s\n", selectedFile);

    const wchar_t* selectedFolder = OpenFolderDialog(L"C:\\");
    wprintf(L"Selected folder: %s\n", selectedFolder);

    return 0;
}
```

---

## Build Instructions
1. Install the **Windows SDK**.
2. Link against:
   - `Comdlg32.lib`
   - `Ole32.lib`
3. Compile as a **DLL** exporting the functions.

---

## Unity / C# Integration
You can use the DLL inside Unity or other .NET projects via **DllImport**.

### FileExplorer.cs Wrapper
```csharp
using System;
using System.Runtime.InteropServices;

public static class FileExplorer {
    private const string WindowsFileDialog = "YourNativeDllName.dll";

    [DllImport(WindowsFileDialog, CharSet = CharSet.Unicode)]
    private static extern IntPtr OpenWindowsFile(string initialDir);

    [DllImport(WindowsFileDialog, CharSet = CharSet.Unicode)]
    private static extern IntPtr OpenFileWithExtension(string filter, string initialDir);

    [DllImport(WindowsFileDialog, CharSet = CharSet.Unicode)]
    private static extern IntPtr OpenFolderDialog(string initialDir);

    public static string OpenFileExplorer(string initialDir = null) {
        IntPtr ptr = OpenWindowsFile(initialDir);
        return Marshal.PtrToStringUni(ptr);
    }

    public static string OpenFileExplorer(string filter, string initialDir = null) {
        IntPtr ptr = OpenFileWithExtension(filter, initialDir);
        return Marshal.PtrToStringUni(ptr);
    }

    public static string OpenFolder(string initialDir = null) {
        IntPtr ptr = OpenFolderDialog(initialDir);
        return Marshal.PtrToStringUni(ptr);
    }
}
```

### Usage in Unity
```csharp
// Open any file
string file1 = FileExplorer.OpenFileExplorer();
Debug.Log("Selected file: " + file1);

// Open file dialog with filter
string filter = "Text files\0*.txt\0Images\0*.png;*.jpg\0All files\0*.*\0";
string file2 = FileExplorer.OpenFileExplorer(filter, @"C:\\Users\\Public\\Documents");
Debug.Log("Filtered file: " + file2);

// Open folder
string folder = FileExplorer.OpenFolder(@"C:\\Projects");
Debug.Log("Selected folder: " + folder);
```

### Notes
- Place the DLL inside Unity's `Plugins` folder.
- Works on **Windows Editor** and **Windows builds only**.
- Returned string is `""` if user cancels.
- Always use correct Windows filter strings (double-null terminated).

---

## License
This code is provided **as-is**, free to use and modify without warranty.

