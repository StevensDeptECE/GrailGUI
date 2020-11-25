# Windows Setup
[comment]: <> (Need to pull in the getting started on windows instructions for MSys2 and installing the various packages)
## Windows Terminal MSys2 Integration
1. Open the `Microsoft Store` app and search for `Windows Terminal`. 
2. Install the first result by `Microsoft Corporation`. 
3. Launch the app. 
4. In the top bar, click on the downwards facing arrow, and then `Settings`. 
5. VSCode or another editor should pop up with a `settings.json` file. 
6. Copy the following into `list`: 
    ```
    {
      "closeOnExit": true,
      "commandline": "cmd.exe /c \"set MSYSTEM=MINGW64&& C:\\msys64\\usr\\bin\\bash.exe --login\"",
      "fontFace": "Source Code Pro for Powerline",
      "fontSize": 12,
      "guid": "{09dc5eef-6840-4050-ae69-21e55e6a2e63}",
      "historySize": 9001,
      "icon": "C:\\\\msys64\\msys2.ico",
      "name": "MSYS2",
      "padding": "0, 0, 0, 0",
      "snapOnInput": true
    }
    ```
  7. Modify the code snippet to match your install path of MSys2. If you did not use custom install paths, then this should not affect you. 
  8. Save and Close the file, go back to Windows Terminal, Click on the new MSys2 that pops up in the same menu where you found the settings. 