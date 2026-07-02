# Microsoft Developer Studio Generated NMAKE File, Based on SAPI.dsp
!IF "$(CFG)" == ""
CFG=SAPI - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SAPI - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SAPI - Win32 Release" && "$(CFG)" != "SAPI - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SAPI.mak" CFG="SAPI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SAPI - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SAPI - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SAPI - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SAPI.dll"


CLEAN :
	-@erase "$(INTDIR)\CConnection.obj"
	-@erase "$(INTDIR)\CPSession.obj"
	-@erase "$(INTDIR)\SAPIlib.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SAPI.dll"
	-@erase "$(OUTDIR)\SAPI.exp"
	-@erase "$(OUTDIR)\SAPI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SAPI_EXPORTS" /Fp"$(INTDIR)\SAPI.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SAPI.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\SAPI.pdb" /machine:I386 /def:".\SAPIlib.def" /out:"$(OUTDIR)\SAPI.dll" /implib:"$(OUTDIR)\SAPI.lib" 
DEF_FILE= \
	".\SAPIlib.def"
LINK32_OBJS= \
	"$(INTDIR)\SAPIlib.obj" \
	"$(INTDIR)\CPSession.obj" \
	"$(INTDIR)\CConnection.obj"

"$(OUTDIR)\SAPI.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SAPI - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SAPILib.dll"


CLEAN :
	-@erase "$(INTDIR)\CConnection.obj"
	-@erase "$(INTDIR)\CPSession.obj"
	-@erase "$(INTDIR)\SAPIlib.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SAPILib.dll"
	-@erase "$(OUTDIR)\SAPILib.exp"
	-@erase "$(OUTDIR)\SAPILib.ilk"
	-@erase "$(OUTDIR)\SAPILib.lib"
	-@erase "$(OUTDIR)\SAPILib.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SAPI_EXPORTS" /Fp"$(INTDIR)\SAPI.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SAPILib.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=s7msstd.lib s7msc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\SAPILib.pdb" /debug /machine:I386 /def:".\SAPIlib.def" /out:"$(OUTDIR)\SAPILib.dll" /implib:"$(OUTDIR)\SAPILib.lib" /pdbtype:sept 
DEF_FILE= \
	".\SAPIlib.def"
LINK32_OBJS= \
	"$(INTDIR)\SAPIlib.obj" \
	"$(INTDIR)\CPSession.obj" \
	"$(INTDIR)\CConnection.obj"

"$(OUTDIR)\SAPILib.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\SAPILib.dll"
   DLLCopy.cmd
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

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
!IF EXISTS("SAPI.dep")
!INCLUDE "SAPI.dep"
!ELSE 
!MESSAGE Warning: cannot find "SAPI.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SAPI - Win32 Release" || "$(CFG)" == "SAPI - Win32 Debug"
SOURCE=.\CConnection.cpp

"$(INTDIR)\CConnection.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CPSession.cpp

"$(INTDIR)\CPSession.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SAPIlib.cpp

"$(INTDIR)\SAPIlib.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

