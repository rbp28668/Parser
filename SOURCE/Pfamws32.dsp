# Microsoft Developer Studio Project File - Name="Pfamws16" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Pfamws16 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Pfamws32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pfamws32.mak" CFG="Pfamws16 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pfamws16 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Pfamws16 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G3 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "NOCHECK" /FR /YX /c
# ADD CPP /nologo /W4 /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NOCHECK" /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Z7 /Od /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NOCHECK" /FR /YX /c
# ADD CPP /nologo /W4 /Z7 /Od /Gy /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NOCHECK" /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Pfamws16 - Win32 Release"
# Name "Pfamws16 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Amble.c
# End Source File
# Begin Source File

SOURCE=.\Aplist.c
# End Source File
# Begin Source File

SOURCE=.\Block.c
# End Source File
# Begin Source File

SOURCE=.\Builtin.c
# End Source File
# Begin Source File

SOURCE=.\Ccall.c
# End Source File
# Begin Source File

SOURCE=.\Constant.c
# End Source File
# Begin Source File

SOURCE=.\Enums.c
# End Source File
# Begin Source File

SOURCE=.\Error.c
# End Source File
# Begin Source File

SOURCE=.\Expr.c
# End Source File
# Begin Source File

SOURCE=.\Exprtype.c
# End Source File
# Begin Source File

SOURCE=.\Fndec.c
# End Source File
# Begin Source File

SOURCE=.\Fplist.c
# End Source File
# Begin Source File

SOURCE=.\Gettext.c
# End Source File
# Begin Source File

SOURCE=.\Getvar.c
# End Source File
# Begin Source File

SOURCE=.\Ident.c
# End Source File
# Begin Source File

SOURCE=.\Iproc.c
# End Source File
# Begin Source File

SOURCE=.\Lex.c
# End Source File
# Begin Source File

SOURCE=.\Malloc.c
# End Source File
# Begin Source File

SOURCE=.\Musthave.c
# End Source File
# Begin Source File

SOURCE=.\Parser.c
# End Source File
# Begin Source File

SOURCE=.\Plant.c
# End Source File
# Begin Source File

SOURCE=.\Plantexp.c
# End Source File
# Begin Source File

SOURCE=.\Printf.c
# End Source File
# Begin Source File

SOURCE=.\Procdec.c
# End Source File
# Begin Source File

SOURCE=.\Pstring.c
# End Source File
# Begin Source File

SOURCE=.\Record.c
# End Source File
# Begin Source File

SOURCE=.\Showexpr.c
# End Source File
# Begin Source File

SOURCE=.\Simpstat.c
# End Source File
# Begin Source File

SOURCE=.\Statemnt.c
# End Source File
# Begin Source File

SOURCE=.\Type.c
# End Source File
# Begin Source File

SOURCE=.\Vars.c
# End Source File
# Begin Source File

SOURCE=.\Version.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Amble.h
# End Source File
# Begin Source File

SOURCE=.\Aplist.h
# End Source File
# Begin Source File

SOURCE=.\Block.h
# End Source File
# Begin Source File

SOURCE=.\Builtin.h
# End Source File
# Begin Source File

SOURCE=.\Enums.h
# End Source File
# Begin Source File

SOURCE=.\Environ.h
# End Source File
# Begin Source File

SOURCE=.\Errcode.h
# End Source File
# Begin Source File

SOURCE=.\Error.h
# End Source File
# Begin Source File

SOURCE=.\Expr.h
# End Source File
# Begin Source File

SOURCE=.\Fndec.h
# End Source File
# Begin Source File

SOURCE=.\Fplist.h
# End Source File
# Begin Source File

SOURCE=.\Gettext.h
# End Source File
# Begin Source File

SOURCE=.\Ident.h
# End Source File
# Begin Source File

SOURCE=.\Iproc.h
# End Source File
# Begin Source File

SOURCE=.\Lex.h
# End Source File
# Begin Source File

SOURCE=.\Malloc.h
# End Source File
# Begin Source File

SOURCE=.\Musthave.h
# End Source File
# Begin Source File

SOURCE=.\Parser.h
# End Source File
# Begin Source File

SOURCE=.\Plant.h
# End Source File
# Begin Source File

SOURCE=.\Printf.h
# End Source File
# Begin Source File

SOURCE=.\Procdec.h
# End Source File
# Begin Source File

SOURCE=.\Prsif.h
# End Source File
# Begin Source File

SOURCE=.\Pstring.h
# End Source File
# Begin Source File

SOURCE=.\Record.h
# End Source File
# Begin Source File

SOURCE=.\Statemnt.h
# End Source File
# Begin Source File

SOURCE=.\Tokens.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
