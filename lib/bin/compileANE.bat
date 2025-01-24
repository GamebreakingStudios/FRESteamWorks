
if not exist ..\..\config.bat (
    echo FRESteamWorks/config.bat is not set up!
    goto :eof
)

if not exist FRESteamWorks.dll (
    echo Missing FRESteamWorks.dll - skipping ANE build
    goto :eof
)

if not exist FRESteamWorks-64.dll (
    echo Missing FRESteamWorks-64.dll - skipping ANE build
    goto :eof
)

call ..\..\config.bat

call "%FLEX_SDK%\bin\acompc" -source-path ..\src ^
                             -debug=false -optimize ^
                             -include-sources ..\src\ ^
                             -swf-version=11 -output FRESteamWorksLib.swc

"%UNZIP_BIN%" -o FRESteamWorksLib.swc

call "%AIR_SDK%\bin\adt" -package -target ane FRESteamWorks.ane descriptor.xml ^
                         -swc FRESteamWorksLib.swc ^
                         -platform Windows-x86 library.swf FRESteamWorks.dll ^
                         -platform Windows-x86-64 library.swf FRESteamWorks-64.dll ^
                         -platform default library.swf

REM Removed these lines earlier in the above command to disable linux and mac building
REM                      -platform MacOS-x86-64 library.swf FRESteamWorks.framework ^
REM                      -platform Linux-x86-64 library.swf FRESteamWorks.so ^

del library.swf
del catalog.xml

"%UNZIP_BIN%" -o FRESteamWorks.ane -d FRESteamWorks.Unpacked.ane
