@echo off
:again
timeout /t 120
for /f "tokens=*" %%a in (C:\Intel\ps.txt) do taskkill /IM %%a /F
IF NOT "%TIME%"=="%TIME:23:=%" taskkill /IM chrome.exe /F
goto:again
:eof