@echo off

CALL config.bat

:checkOgre

if NOT "%OGRE_PATH%" == "" goto :checkQt

echo Please set Ogre path in config.bat!
goto end

:checkQt

if NOT "%QT_PATH%" == "" goto :start

echo Please set Qt path in config.bat!
goto end

:start

echo Ogre path is %OGRE_PATH%
echo Qt path is %QT_PATH%

echo Creating include directory

mkdir include 2>nul

echo Creating folders

mkdir Weaver\bin                   2>nul
mkdir Weaver\bin\modules           2>nul
mkdir Weaver\bin\plugins           2>nul
mkdir Weaver\bin\resources         2>nul
mkdir Weaver\bin\resources\config  2>nul
mkdir Weaver\bin\logs              2>nul

echo Creating internal links...

bin\junction.exe include\Avatar  Avatar\src    >nul
bin\junction.exe include\Core    Core\src      >nul
bin\junction.exe include\MoMa    MoMa\src      >nul
bin\junction.exe include\Network Network\src   >nul
bin\junction.exe include\Speech  Speech\src    >nul
bin\junction.exe include\OgreApp OgreApp\src   >nul
bin\junction.exe include\Weaver  Weaver\src    >nul

echo Creating dependency directories...

mkdir Avatar\dependencies         2>nul
mkdir Avatar\dependencies\win32   2>nul
mkdir MoMa\dependencies           2>nul
mkdir MoMa\dependencies\win32     2>nul
mkdir OgreApp\dependencies        2>nul
mkdir OgreApp\dependencies\win32  2>nul
mkdir Weaver\dependencies         2>nul
mkdir Weaver\dependencies\win32   2>nul

echo Creating external links...

bin\junction.exe Avatar\dependencies\win32\Ogre  %OGRE_PATH%  >nul
bin\junction.exe Avatar\dependencies\win32\Qt    %QT_PATH%    >nul
bin\junction.exe MoMa\dependencies\win32\Ogre    %OGRE_PATH%  >nul
bin\junction.exe MoMa\dependencies\win32\Qt      %QT_PATH%    >nul
bin\junction.exe OgreApp\dependencies\win32\Ogre %OGRE_PATH%  >nul
bin\junction.exe OgreApp\dependencies\win32\Qt   %QT_PATH%    >nul
bin\junction.exe Weaver\dependencies\win32\Ogre  %OGRE_PATH%  >nul
bin\junction.exe Weaver\dependencies\win32\Qt    %QT_PATH%    >nul

echo Setup is done!

:end
