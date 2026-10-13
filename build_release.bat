@echo off
echo Compiling WauBulan Release Build...

:: Run the compiler
g++ *.cpp -o WauBulan.exe -O2 -Wall -I raylib_win/include -L raylib_win/lib -lraylibdll -lopengl32 -lgdi32 -lwinmm -mwindows

:: Check if compilation was successful
IF %ERRORLEVEL% NEQ 0 (
  echo Compilation Failed!
  pause
  exit /b
)

echo Compilation successful. Creating Release folder...
if not exist "Release" mkdir Release

echo Moving executable...
move /Y WauBulan.exe Release\

echo Copying raylib.dll...
copy /Y raylib_win\lib\raylib.dll Release\

echo Copying assets folder...
:: xcopy copies the directory tree. /E copies subdirectories, /I assumes destination is a folder, /Y overwrites
xcopy /E /I /Y assets Release\assets

echo.
echo Build Complete! Your game is packaged and ready in the "Release" folder.