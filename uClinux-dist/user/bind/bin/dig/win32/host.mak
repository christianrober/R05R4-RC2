# Microsoft Developer Studio Generated NMAKE File, Based on host.dsp
!IF "$(CFG)" == ""
CFG=host - Win32 Debug
!MESSAGE No configuration specified. Defaulting to host - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "host - Win32 Release" && "$(CFG)" != "host - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "host.mak" CFG="host - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "host - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "host - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "host - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\Build\Release\host.exe"

!ELSE 

ALL : "liblwres - Win32 Release" "libbind9 - Win32 Release" "libisc - Win32 Release" "libdns - Win32 Release" "..\..\..\Build\Release\host.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libdns - Win32 ReleaseCLEAN" "libisc - Win32 ReleaseCLEAN" "libbind9 - Win32 ReleaseCLEAN" "liblwres - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\dighost.obj"
	-@erase "$(INTDIR)\host.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\..\Build\Release\host.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "./" /I "../include" /I "../../../" /I "../../../lib/isc/win32" /I "../../../lib/isc/win32/include" /I "../../../lib/isc/include" /I "../../../lib/dns/include" /I "../../../lib/dns/sec/dst/include" /I "../../../lib/bind9/include" /I "../../../lib/lwres/win32/include" /I "../../../lib/lwres/include" /D "WIN32" /D "__STDC__" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\host.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\host.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib advapi32.lib ws2_32.lib ../../../lib/isc/win32/Release/libisc.lib ../../../lib/dns/win32/Release/libdns.lib ../../../lib/bind9/win32/Release/libbind9.lib  ../../../lib/lwres/win32/Release/liblwres.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\host.pdb" /machine:I386 /out:"../../../Build/Release/host.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dighost.obj" \
	"$(INTDIR)\host.obj" \
	"..\..\..\lib\dns\win32\Release\libdns.lib" \
	"..\..\..\lib\isc\win32\Release\libisc.lib" \
	"..\..\..\lib\bind9\win32\Release\libbind9.lib" \
	"..\..\..\lib\lwres\win32\Release\liblwres.lib"

"..\..\..\Build\Release\host.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "host - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\Build\Debug\host.exe" "$(OUTDIR)\host.bsc"

!ELSE 

ALL : "liblwres - Win32 Debug" "libbind9 - Win32 Debug" "libisc - Win32 Debug" "libdns - Win32 Debug" "..\..\..\Build\Debug\host.exe" "$(OUTDIR)\host.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libdns - Win32 DebugCLEAN" "libisc - Win32 DebugCLEAN" "libbind9 - Win32 DebugCLEAN" "liblwres - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\dighost.obj"
	-@erase "$(INTDIR)\dighost.sbr"
	-@erase "$(INTDIR)\host.obj"
	-@erase "$(INTDIR)\host.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\host.bsc"
	-@erase "$(OUTDIR)\host.pdb"
	-@erase "..\..\..\Build\Debug\host.exe"
	-@erase "..\..\..\Build\Debug\host.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "./" /I "../include" /I "../../../" /I "../../../lib/isc/win32" /I "../../../lib/isc/win32/include" /I "../../../lib/isc/include" /I "../../../lib/dns/include" /I "../../../lib/dns/sec/dst/include" /I "../../../lib/bind9/include" /I "../../../lib/lwres/win32/include" /I "../../../lib/lwres/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\host.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dighost.sbr" \
	"$(INTDIR)\host.sbr"

"$(OUTDIR)\host.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib advapi32.lib ws2_32.lib ../../../lib/isc/win32/Debug/libisc.lib ../../../lib/dns/win32/Debug/libdns.lib ../../../lib/bind9/win32/Debug/libbind9.lib  ../../../lib/lwres/win32/Debug/liblwres.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\host.pdb" /debug /machine:I386 /out:"../../../Build/Debug/host.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\dighost.obj" \
	"$(INTDIR)\host.obj" \
	"..\..\..\lib\dns\win32\Debug\libdns.lib" \
	"..\..\..\lib\isc\win32\Debug\libisc.lib" \
	"..\..\..\lib\bind9\win32\Debug\libbind9.lib" \
	"..\..\..\lib\lwres\win32\Debug\liblwres.lib"

"..\..\..\Build\Debug\host.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("host.dep")
!INCLUDE "host.dep"
!ELSE 
!MESSAGE Warning: cannot find "host.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "host - Win32 Release" || "$(CFG)" == "host - Win32 Debug"
SOURCE=..\dighost.c

!IF  "$(CFG)" == "host - Win32 Release"


"$(INTDIR)\dighost.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "host - Win32 Debug"


"$(INTDIR)\dighost.obj"	"$(INTDIR)\dighost.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\host.c

!IF  "$(CFG)" == "host - Win32 Release"


"$(INTDIR)\host.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "host - Win32 Debug"


"$(INTDIR)\host.obj"	"$(INTDIR)\host.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "host - Win32 Release"

"libdns - Win32 Release" : 
   cd "..\..\..\lib\dns\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdns.mak" CFG="libdns - Win32 Release" 
   cd "..\..\..\bin\dig\win32"

"libdns - Win32 ReleaseCLEAN" : 
   cd "..\..\..\lib\dns\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdns.mak" CFG="libdns - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ELSEIF  "$(CFG)" == "host - Win32 Debug"

"libdns - Win32 Debug" : 
   cd "..\..\..\lib\dns\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdns.mak" CFG="libdns - Win32 Debug" 
   cd "..\..\..\bin\dig\win32"

"libdns - Win32 DebugCLEAN" : 
   cd "..\..\..\lib\dns\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdns.mak" CFG="libdns - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ENDIF 

!IF  "$(CFG)" == "host - Win32 Release"

"libisc - Win32 Release" : 
   cd "..\..\..\lib\isc\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libisc.mak" CFG="libisc - Win32 Release" 
   cd "..\..\..\bin\dig\win32"

"libisc - Win32 ReleaseCLEAN" : 
   cd "..\..\..\lib\isc\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libisc.mak" CFG="libisc - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ELSEIF  "$(CFG)" == "host - Win32 Debug"

"libisc - Win32 Debug" : 
   cd "..\..\..\lib\isc\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libisc.mak" CFG="libisc - Win32 Debug" 
   cd "..\..\..\bin\dig\win32"

"libisc - Win32 DebugCLEAN" : 
   cd "..\..\..\lib\isc\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libisc.mak" CFG="libisc - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ENDIF 

!IF  "$(CFG)" == "host - Win32 Release"

"libbind9 - Win32 Release" : 
   cd "..\..\..\lib\bind9\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libbind9.mak" CFG="libbind9 - Win32 Release" 
   cd "..\..\..\bin\dig\win32"

"libbind9 - Win32 ReleaseCLEAN" : 
   cd "..\..\..\lib\bind9\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libbind9.mak" CFG="libbind9 - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ELSEIF  "$(CFG)" == "host - Win32 Debug"

"libbind9 - Win32 Debug" : 
   cd "..\..\..\lib\bind9\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libbind9.mak" CFG="libbind9 - Win32 Debug" 
   cd "..\..\..\bin\dig\win32"

"libbind9 - Win32 DebugCLEAN" : 
   cd "..\..\..\lib\bind9\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\libbind9.mak" CFG="libbind9 - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ENDIF 

!IF  "$(CFG)" == "host - Win32 Release"

"liblwres - Win32 Release" : 
   cd "..\..\..\lib\lwres\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\liblwres.mak" CFG="liblwres - Win32 Release" 
   cd "..\..\..\bin\dig\win32"

"liblwres - Win32 ReleaseCLEAN" : 
   cd "..\..\..\lib\lwres\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\liblwres.mak" CFG="liblwres - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ELSEIF  "$(CFG)" == "host - Win32 Debug"

"liblwres - Win32 Debug" : 
   cd "..\..\..\lib\lwres\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\liblwres.mak" CFG="liblwres - Win32 Debug" 
   cd "..\..\..\bin\dig\win32"

"liblwres - Win32 DebugCLEAN" : 
   cd "..\..\..\lib\lwres\win32"
   $(MAKE) /$(MAKEFLAGS) /F ".\liblwres.mak" CFG="liblwres - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\bin\dig\win32"

!ENDIF 


!ENDIF 
