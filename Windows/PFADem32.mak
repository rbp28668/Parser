# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Test - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Test - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Test - Win32 Release" && "$(CFG)" != "Test - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "PFADem32.mak" CFG="Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Test - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Test - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Test - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\Release\PFADem32.exe"

CLEAN : 
	-@erase ".\Release\Buffer.obj"
	-@erase ".\Release\Demo.res"
	-@erase ".\Release\Error.obj"
	-@erase ".\Release\File.obj"
	-@erase ".\Release\Find.obj"
	-@erase ".\Release\Frame.obj"
	-@erase ".\Release\Input.obj"
	-@erase ".\Release\Main.obj"
	-@erase ".\Release\Output.obj"
	-@erase ".\Release\Pdrive.obj"
	-@erase ".\Release\PFADem32.exe"
	-@erase ".\Release\String.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /G3 /W4 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /G3 /W4 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /G3 /ML /W4 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /Fp"$(INTDIR)/PFADem32.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/Demo.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/PFADem32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x2800 /subsystem:windows /machine:IX86
# ADD LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libc.lib /nologo /stack:0x2800 /subsystem:windows /machine:IX86 /nodefaultlib
LINK32_FLAGS=oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib libc.lib /nologo /stack:0x2800 /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/PFADem32.pdb" /machine:IX86 /nodefaultlib\
 /def:".\Test.def" /out:"$(OUTDIR)/PFADem32.exe" 
DEF_FILE= \
	".\Test.def"
LINK32_OBJS= \
	"..\lib\Pfamws32.lib" \
	".\Release\Buffer.obj" \
	".\Release\Demo.res" \
	".\Release\Error.obj" \
	".\Release\File.obj" \
	".\Release\Find.obj" \
	".\Release\Frame.obj" \
	".\Release\Input.obj" \
	".\Release\Main.obj" \
	".\Release\Output.obj" \
	".\Release\Pdrive.obj" \
	".\Release\String.obj"

".\Release\PFADem32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\Debug\PFADem32.exe"

CLEAN : 
	-@erase ".\Debug\Buffer.obj"
	-@erase ".\Debug\Demo.res"
	-@erase ".\Debug\Error.obj"
	-@erase ".\Debug\File.obj"
	-@erase ".\Debug\Find.obj"
	-@erase ".\Debug\Frame.obj"
	-@erase ".\Debug\Input.obj"
	-@erase ".\Debug\Main.obj"
	-@erase ".\Debug\Output.obj"
	-@erase ".\Debug\Pdrive.obj"
	-@erase ".\Debug\PFADem32.exe"
	-@erase ".\Debug\PFADem32.ilk"
	-@erase ".\Debug\PFADem32.map"
	-@erase ".\Debug\PFADem32.pdb"
	-@erase ".\Debug\String.obj"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\vc40.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W4 /Gm /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /W4 /Gm /Zi /Od /Gy /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MLd /W4 /Gm /Zi /Od /Gy /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /Fp"$(INTDIR)/PFADem32.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/Demo.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/PFADem32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x2800 /subsystem:windows /map /debug /machine:IX86
# ADD LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libc.lib /nologo /stack:0x2800 /subsystem:windows /map /debug /machine:IX86 /nodefaultlib
LINK32_FLAGS=oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib libc.lib /nologo /stack:0x2800 /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/PFADem32.pdb" /map:"$(INTDIR)/PFADem32.map"\
 /debug /machine:IX86 /nodefaultlib /def:".\Test.def"\
 /out:"$(OUTDIR)/PFADem32.exe" 
DEF_FILE= \
	".\Test.def"
LINK32_OBJS= \
	"..\lib\Pfamws32.lib" \
	".\Debug\Buffer.obj" \
	".\Debug\Demo.res" \
	".\Debug\Error.obj" \
	".\Debug\File.obj" \
	".\Debug\Find.obj" \
	".\Debug\Frame.obj" \
	".\Debug\Input.obj" \
	".\Debug\Main.obj" \
	".\Debug\Output.obj" \
	".\Debug\Pdrive.obj" \
	".\Debug\String.obj"

".\Debug\PFADem32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Test - Win32 Release"
# Name "Test - Win32 Debug"

!IF  "$(CFG)" == "Test - Win32 Release"

!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Input.c
DEP_CPP_INPUT=\
	".\app.h"\
	".\environ.h"\
	".\file.h"\
	".\find.h"\
	".\frame.h"\
	".\input.h"\
	".\output.h"\
	".\prsif.h"\
	".\resource.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Input.obj" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\Input.obj" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\File.c
DEP_CPP_FILE_=\
	".\file.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\File.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\File.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Find.c
DEP_CPP_FIND_=\
	".\find.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Find.obj" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\Find.obj" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Frame.c
DEP_CPP_FRAME=\
	".\app.h"\
	".\frame.h"\
	".\input.h"\
	".\output.h"\
	".\resource.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\Frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Main.c
DEP_CPP_MAIN_=\
	".\app.h"\
	".\Buffer.h"\
	".\environ.h"\
	".\Error.h"\
	".\frame.h"\
	".\input.h"\
	".\output.h"\
	".\Pdrive.h"\
	".\prsif.h"\
	".\resource.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\Main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Output.c
DEP_CPP_OUTPU=\
	".\app.h"\
	".\Buffer.h"\
	".\frame.h"\
	".\input.h"\
	".\output.h"\
	".\resource.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Output.obj" : $(SOURCE) $(DEP_CPP_OUTPU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\Output.obj" : $(SOURCE) $(DEP_CPP_OUTPU) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Demo.rc

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Demo.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

DEP_RSC_DEMO_=\
	".\Input.ico"\
	".\Output.ico"\
	".\Pfa.ico"\
	".\resource.h"\
	

".\Debug\Demo.res" : $(SOURCE) $(DEP_RSC_DEMO_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Buffer.c

!IF  "$(CFG)" == "Test - Win32 Release"

DEP_CPP_BUFFE=\
	".\Buffer.h"\
	

".\Release\Buffer.obj" : $(SOURCE) $(DEP_CPP_BUFFE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

DEP_CPP_BUFFE=\
	".\Buffer.h"\
	
NODEP_CPP_BUFFE=\
	".\BUFFSIZE"\
	".\nLinePos"\
	".\pchBuff"\
	".\pchBuffHead"\
	

".\Debug\Buffer.obj" : $(SOURCE) $(DEP_CPP_BUFFE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pdrive.c
DEP_CPP_PDRIV=\
	".\environ.h"\
	".\frame.h"\
	".\Pdrive.h"\
	".\prsif.h"\
	".\Pstring.h"\
	".\resource.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Pdrive.obj" : $(SOURCE) $(DEP_CPP_PDRIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\Pdrive.obj" : $(SOURCE) $(DEP_CPP_PDRIV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Error.c
DEP_CPP_ERROR=\
	".\app.h"\
	".\Error.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\Error.obj" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\Error.obj" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\String.c
DEP_CPP_STRIN=\
	".\environ.h"\
	".\Pdrive.h"\
	".\prsif.h"\
	

!IF  "$(CFG)" == "Test - Win32 Release"


".\Release\String.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Test - Win32 Debug"


".\Debug\String.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Test.def

!IF  "$(CFG)" == "Test - Win32 Release"

!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Projects\Parser\lib\Pfamws32.lib

!IF  "$(CFG)" == "Test - Win32 Release"

!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
