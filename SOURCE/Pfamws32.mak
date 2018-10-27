# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=Pfamws16 - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Pfamws16 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Pfamws16 - Win32 Release" && "$(CFG)" !=\
 "Pfamws16 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pfamws32.mak" CFG="Pfamws16 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pfamws16 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Pfamws16 - Win32 Debug" (based on "Win32 (x86) Static Library")
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
# PROP Target_Last_Scanned "Pfamws16 - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"

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

ALL : ".\Release\Pfamws32.lib" ".\Release\Pfamws32.bsc"

CLEAN : 
	-@erase ".\Release\Amble.obj"
	-@erase ".\Release\Amble.sbr"
	-@erase ".\Release\Aplist.obj"
	-@erase ".\Release\Aplist.sbr"
	-@erase ".\Release\Block.obj"
	-@erase ".\Release\Block.sbr"
	-@erase ".\Release\Builtin.obj"
	-@erase ".\Release\Builtin.sbr"
	-@erase ".\Release\Ccall.obj"
	-@erase ".\Release\Ccall.sbr"
	-@erase ".\Release\Constant.obj"
	-@erase ".\Release\Constant.sbr"
	-@erase ".\Release\Enums.obj"
	-@erase ".\Release\Enums.sbr"
	-@erase ".\Release\Error.obj"
	-@erase ".\Release\Error.sbr"
	-@erase ".\Release\Expr.obj"
	-@erase ".\Release\Expr.sbr"
	-@erase ".\Release\Exprtype.obj"
	-@erase ".\Release\Exprtype.sbr"
	-@erase ".\Release\Fndec.obj"
	-@erase ".\Release\Fndec.sbr"
	-@erase ".\Release\Fplist.obj"
	-@erase ".\Release\Fplist.sbr"
	-@erase ".\Release\Gettext.obj"
	-@erase ".\Release\Gettext.sbr"
	-@erase ".\Release\Getvar.obj"
	-@erase ".\Release\Getvar.sbr"
	-@erase ".\Release\Ident.obj"
	-@erase ".\Release\Ident.sbr"
	-@erase ".\Release\Iproc.obj"
	-@erase ".\Release\Iproc.sbr"
	-@erase ".\Release\Lex.obj"
	-@erase ".\Release\Lex.sbr"
	-@erase ".\Release\Malloc.obj"
	-@erase ".\Release\Malloc.sbr"
	-@erase ".\Release\Musthave.obj"
	-@erase ".\Release\Musthave.sbr"
	-@erase ".\Release\Parser.obj"
	-@erase ".\Release\Parser.sbr"
	-@erase ".\Release\Pfamws32.bsc"
	-@erase ".\Release\Pfamws32.lib"
	-@erase ".\Release\Plant.obj"
	-@erase ".\Release\Plant.sbr"
	-@erase ".\Release\Plantexp.obj"
	-@erase ".\Release\Plantexp.sbr"
	-@erase ".\Release\Printf.obj"
	-@erase ".\Release\Printf.sbr"
	-@erase ".\Release\Procdec.obj"
	-@erase ".\Release\Procdec.sbr"
	-@erase ".\Release\Pstring.obj"
	-@erase ".\Release\Pstring.sbr"
	-@erase ".\Release\Record.obj"
	-@erase ".\Release\Record.sbr"
	-@erase ".\Release\Showexpr.obj"
	-@erase ".\Release\Showexpr.sbr"
	-@erase ".\Release\Simpstat.obj"
	-@erase ".\Release\Simpstat.sbr"
	-@erase ".\Release\Statemnt.obj"
	-@erase ".\Release\Statemnt.sbr"
	-@erase ".\Release\Type.obj"
	-@erase ".\Release\Type.sbr"
	-@erase ".\Release\Vars.obj"
	-@erase ".\Release\Vars.sbr"
	-@erase ".\Release\Version.obj"
	-@erase ".\Release\Version.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /G3 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "NOCHECK" /FR /YX /c
# ADD CPP /nologo /W4 /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NOCHECK" /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /c
CPP_PROJ=/nologo /ML /W4 /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NOCHECK"\
 /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/Pfamws32.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Pfamws32.bsc" 
BSC32_SBRS= \
	".\Release\Amble.sbr" \
	".\Release\Aplist.sbr" \
	".\Release\Block.sbr" \
	".\Release\Builtin.sbr" \
	".\Release\Ccall.sbr" \
	".\Release\Constant.sbr" \
	".\Release\Enums.sbr" \
	".\Release\Error.sbr" \
	".\Release\Expr.sbr" \
	".\Release\Exprtype.sbr" \
	".\Release\Fndec.sbr" \
	".\Release\Fplist.sbr" \
	".\Release\Gettext.sbr" \
	".\Release\Getvar.sbr" \
	".\Release\Ident.sbr" \
	".\Release\Iproc.sbr" \
	".\Release\Lex.sbr" \
	".\Release\Malloc.sbr" \
	".\Release\Musthave.sbr" \
	".\Release\Parser.sbr" \
	".\Release\Plant.sbr" \
	".\Release\Plantexp.sbr" \
	".\Release\Printf.sbr" \
	".\Release\Procdec.sbr" \
	".\Release\Pstring.sbr" \
	".\Release\Record.sbr" \
	".\Release\Showexpr.sbr" \
	".\Release\Simpstat.sbr" \
	".\Release\Statemnt.sbr" \
	".\Release\Type.sbr" \
	".\Release\Vars.sbr" \
	".\Release\Version.sbr"

".\Release\Pfamws32.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/Pfamws32.lib" 
LIB32_OBJS= \
	".\Release\Amble.obj" \
	".\Release\Aplist.obj" \
	".\Release\Block.obj" \
	".\Release\Builtin.obj" \
	".\Release\Ccall.obj" \
	".\Release\Constant.obj" \
	".\Release\Enums.obj" \
	".\Release\Error.obj" \
	".\Release\Expr.obj" \
	".\Release\Exprtype.obj" \
	".\Release\Fndec.obj" \
	".\Release\Fplist.obj" \
	".\Release\Gettext.obj" \
	".\Release\Getvar.obj" \
	".\Release\Ident.obj" \
	".\Release\Iproc.obj" \
	".\Release\Lex.obj" \
	".\Release\Malloc.obj" \
	".\Release\Musthave.obj" \
	".\Release\Parser.obj" \
	".\Release\Plant.obj" \
	".\Release\Plantexp.obj" \
	".\Release\Printf.obj" \
	".\Release\Procdec.obj" \
	".\Release\Pstring.obj" \
	".\Release\Record.obj" \
	".\Release\Showexpr.obj" \
	".\Release\Simpstat.obj" \
	".\Release\Statemnt.obj" \
	".\Release\Type.obj" \
	".\Release\Vars.obj" \
	".\Release\Version.obj"

".\Release\Pfamws32.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"

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

ALL : ".\Debug\Pfamws32.lib" ".\Debug\Pfamws32.bsc"

CLEAN : 
	-@erase ".\Debug\Amble.obj"
	-@erase ".\Debug\Amble.sbr"
	-@erase ".\Debug\Aplist.obj"
	-@erase ".\Debug\Aplist.sbr"
	-@erase ".\Debug\Block.obj"
	-@erase ".\Debug\Block.sbr"
	-@erase ".\Debug\Builtin.obj"
	-@erase ".\Debug\Builtin.sbr"
	-@erase ".\Debug\Ccall.obj"
	-@erase ".\Debug\Ccall.sbr"
	-@erase ".\Debug\Constant.obj"
	-@erase ".\Debug\Constant.sbr"
	-@erase ".\Debug\Enums.obj"
	-@erase ".\Debug\Enums.sbr"
	-@erase ".\Debug\Error.obj"
	-@erase ".\Debug\Error.sbr"
	-@erase ".\Debug\Expr.obj"
	-@erase ".\Debug\Expr.sbr"
	-@erase ".\Debug\Exprtype.obj"
	-@erase ".\Debug\Exprtype.sbr"
	-@erase ".\Debug\Fndec.obj"
	-@erase ".\Debug\Fndec.sbr"
	-@erase ".\Debug\Fplist.obj"
	-@erase ".\Debug\Fplist.sbr"
	-@erase ".\Debug\Gettext.obj"
	-@erase ".\Debug\Gettext.sbr"
	-@erase ".\Debug\Getvar.obj"
	-@erase ".\Debug\Getvar.sbr"
	-@erase ".\Debug\Ident.obj"
	-@erase ".\Debug\Ident.sbr"
	-@erase ".\Debug\Iproc.obj"
	-@erase ".\Debug\Iproc.sbr"
	-@erase ".\Debug\Lex.obj"
	-@erase ".\Debug\Lex.sbr"
	-@erase ".\Debug\Malloc.obj"
	-@erase ".\Debug\Malloc.sbr"
	-@erase ".\Debug\Musthave.obj"
	-@erase ".\Debug\Musthave.sbr"
	-@erase ".\Debug\Parser.obj"
	-@erase ".\Debug\Parser.sbr"
	-@erase ".\Debug\Pfamws32.bsc"
	-@erase ".\Debug\Pfamws32.lib"
	-@erase ".\Debug\Plant.obj"
	-@erase ".\Debug\Plant.sbr"
	-@erase ".\Debug\Plantexp.obj"
	-@erase ".\Debug\Plantexp.sbr"
	-@erase ".\Debug\Printf.obj"
	-@erase ".\Debug\Printf.sbr"
	-@erase ".\Debug\Procdec.obj"
	-@erase ".\Debug\Procdec.sbr"
	-@erase ".\Debug\Pstring.obj"
	-@erase ".\Debug\Pstring.sbr"
	-@erase ".\Debug\Record.obj"
	-@erase ".\Debug\Record.sbr"
	-@erase ".\Debug\Showexpr.obj"
	-@erase ".\Debug\Showexpr.sbr"
	-@erase ".\Debug\Simpstat.obj"
	-@erase ".\Debug\Simpstat.sbr"
	-@erase ".\Debug\Statemnt.obj"
	-@erase ".\Debug\Statemnt.sbr"
	-@erase ".\Debug\Type.obj"
	-@erase ".\Debug\Type.sbr"
	-@erase ".\Debug\Vars.obj"
	-@erase ".\Debug\Vars.sbr"
	-@erase ".\Debug\Version.obj"
	-@erase ".\Debug\Version.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W4 /Z7 /Od /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NOCHECK" /FR /YX /c
# ADD CPP /nologo /W4 /Z7 /Od /Gy /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NOCHECK" /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /c
CPP_PROJ=/nologo /MLd /W4 /Z7 /Od /Gy /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "NOCHECK" /D "WIN32_LEAN_AND_MEAN" /D "WIN32_EXTRA_LEAN" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/Pfamws32.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Pfamws32.bsc" 
BSC32_SBRS= \
	".\Debug\Amble.sbr" \
	".\Debug\Aplist.sbr" \
	".\Debug\Block.sbr" \
	".\Debug\Builtin.sbr" \
	".\Debug\Ccall.sbr" \
	".\Debug\Constant.sbr" \
	".\Debug\Enums.sbr" \
	".\Debug\Error.sbr" \
	".\Debug\Expr.sbr" \
	".\Debug\Exprtype.sbr" \
	".\Debug\Fndec.sbr" \
	".\Debug\Fplist.sbr" \
	".\Debug\Gettext.sbr" \
	".\Debug\Getvar.sbr" \
	".\Debug\Ident.sbr" \
	".\Debug\Iproc.sbr" \
	".\Debug\Lex.sbr" \
	".\Debug\Malloc.sbr" \
	".\Debug\Musthave.sbr" \
	".\Debug\Parser.sbr" \
	".\Debug\Plant.sbr" \
	".\Debug\Plantexp.sbr" \
	".\Debug\Printf.sbr" \
	".\Debug\Procdec.sbr" \
	".\Debug\Pstring.sbr" \
	".\Debug\Record.sbr" \
	".\Debug\Showexpr.sbr" \
	".\Debug\Simpstat.sbr" \
	".\Debug\Statemnt.sbr" \
	".\Debug\Type.sbr" \
	".\Debug\Vars.sbr" \
	".\Debug\Version.sbr"

".\Debug\Pfamws32.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/Pfamws32.lib" 
LIB32_OBJS= \
	".\Debug\Amble.obj" \
	".\Debug\Aplist.obj" \
	".\Debug\Block.obj" \
	".\Debug\Builtin.obj" \
	".\Debug\Ccall.obj" \
	".\Debug\Constant.obj" \
	".\Debug\Enums.obj" \
	".\Debug\Error.obj" \
	".\Debug\Expr.obj" \
	".\Debug\Exprtype.obj" \
	".\Debug\Fndec.obj" \
	".\Debug\Fplist.obj" \
	".\Debug\Gettext.obj" \
	".\Debug\Getvar.obj" \
	".\Debug\Ident.obj" \
	".\Debug\Iproc.obj" \
	".\Debug\Lex.obj" \
	".\Debug\Malloc.obj" \
	".\Debug\Musthave.obj" \
	".\Debug\Parser.obj" \
	".\Debug\Plant.obj" \
	".\Debug\Plantexp.obj" \
	".\Debug\Printf.obj" \
	".\Debug\Procdec.obj" \
	".\Debug\Pstring.obj" \
	".\Debug\Record.obj" \
	".\Debug\Showexpr.obj" \
	".\Debug\Simpstat.obj" \
	".\Debug\Statemnt.obj" \
	".\Debug\Type.obj" \
	".\Debug\Vars.obj" \
	".\Debug\Version.obj"

".\Debug\Pfamws32.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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

# Name "Pfamws16 - Win32 Release"
# Name "Pfamws16 - Win32 Debug"

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"

!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Amble.c
DEP_CPP_AMBLE=\
	".\Amble.h"\
	".\Environ.h"\
	".\Ident.h"\
	".\Iproc.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Amble.obj" : $(SOURCE) $(DEP_CPP_AMBLE) "$(INTDIR)"

".\Release\Amble.sbr" : $(SOURCE) $(DEP_CPP_AMBLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Amble.obj" : $(SOURCE) $(DEP_CPP_AMBLE) "$(INTDIR)"

".\Debug\Amble.sbr" : $(SOURCE) $(DEP_CPP_AMBLE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Aplist.c
DEP_CPP_APLIS=\
	".\Aplist.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Aplist.obj" : $(SOURCE) $(DEP_CPP_APLIS) "$(INTDIR)"

".\Release\Aplist.sbr" : $(SOURCE) $(DEP_CPP_APLIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Aplist.obj" : $(SOURCE) $(DEP_CPP_APLIS) "$(INTDIR)"

".\Debug\Aplist.sbr" : $(SOURCE) $(DEP_CPP_APLIS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Block.c
DEP_CPP_BLOCK=\
	".\Amble.h"\
	".\Block.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Statemnt.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Block.obj" : $(SOURCE) $(DEP_CPP_BLOCK) "$(INTDIR)"

".\Release\Block.sbr" : $(SOURCE) $(DEP_CPP_BLOCK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Block.obj" : $(SOURCE) $(DEP_CPP_BLOCK) "$(INTDIR)"

".\Debug\Block.sbr" : $(SOURCE) $(DEP_CPP_BLOCK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Builtin.c
DEP_CPP_BUILT=\
	".\Builtin.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Ident.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Builtin.obj" : $(SOURCE) $(DEP_CPP_BUILT) "$(INTDIR)"

".\Release\Builtin.sbr" : $(SOURCE) $(DEP_CPP_BUILT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Builtin.obj" : $(SOURCE) $(DEP_CPP_BUILT) "$(INTDIR)"

".\Debug\Builtin.sbr" : $(SOURCE) $(DEP_CPP_BUILT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Ccall.c
DEP_CPP_CCALL=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Fndec.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Procdec.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Ccall.obj" : $(SOURCE) $(DEP_CPP_CCALL) "$(INTDIR)"

".\Release\Ccall.sbr" : $(SOURCE) $(DEP_CPP_CCALL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Ccall.obj" : $(SOURCE) $(DEP_CPP_CCALL) "$(INTDIR)"

".\Debug\Ccall.sbr" : $(SOURCE) $(DEP_CPP_CCALL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Constant.c
DEP_CPP_CONST=\
	".\Enums.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Record.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Constant.obj" : $(SOURCE) $(DEP_CPP_CONST) "$(INTDIR)"

".\Release\Constant.sbr" : $(SOURCE) $(DEP_CPP_CONST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Constant.obj" : $(SOURCE) $(DEP_CPP_CONST) "$(INTDIR)"

".\Debug\Constant.sbr" : $(SOURCE) $(DEP_CPP_CONST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Enums.c
DEP_CPP_ENUMS=\
	".\Enums.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Enums.obj" : $(SOURCE) $(DEP_CPP_ENUMS) "$(INTDIR)"

".\Release\Enums.sbr" : $(SOURCE) $(DEP_CPP_ENUMS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Enums.obj" : $(SOURCE) $(DEP_CPP_ENUMS) "$(INTDIR)"

".\Debug\Enums.sbr" : $(SOURCE) $(DEP_CPP_ENUMS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Error.c
DEP_CPP_ERROR=\
	".\Environ.h"\
	".\Gettext.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Prsif.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Error.obj" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"

".\Release\Error.sbr" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Error.obj" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"

".\Debug\Error.sbr" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Expr.c
DEP_CPP_EXPR_=\
	".\Aplist.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Expr.obj" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"

".\Release\Expr.sbr" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Expr.obj" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"

".\Debug\Expr.sbr" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Exprtype.c
DEP_CPP_EXPRT=\
	".\Aplist.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Exprtype.obj" : $(SOURCE) $(DEP_CPP_EXPRT) "$(INTDIR)"

".\Release\Exprtype.sbr" : $(SOURCE) $(DEP_CPP_EXPRT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Exprtype.obj" : $(SOURCE) $(DEP_CPP_EXPRT) "$(INTDIR)"

".\Debug\Exprtype.sbr" : $(SOURCE) $(DEP_CPP_EXPRT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Fndec.c
DEP_CPP_FNDEC=\
	".\Block.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Fndec.h"\
	".\Fplist.h"\
	".\Ident.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Fndec.obj" : $(SOURCE) $(DEP_CPP_FNDEC) "$(INTDIR)"

".\Release\Fndec.sbr" : $(SOURCE) $(DEP_CPP_FNDEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Fndec.obj" : $(SOURCE) $(DEP_CPP_FNDEC) "$(INTDIR)"

".\Debug\Fndec.sbr" : $(SOURCE) $(DEP_CPP_FNDEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Fplist.c
DEP_CPP_FPLIS=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Fndec.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Procdec.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Fplist.obj" : $(SOURCE) $(DEP_CPP_FPLIS) "$(INTDIR)"

".\Release\Fplist.sbr" : $(SOURCE) $(DEP_CPP_FPLIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Fplist.obj" : $(SOURCE) $(DEP_CPP_FPLIS) "$(INTDIR)"

".\Debug\Fplist.sbr" : $(SOURCE) $(DEP_CPP_FPLIS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Gettext.c
DEP_CPP_GETTE=\
	{$(INCLUDE)}"\malloc.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Gettext.obj" : $(SOURCE) $(DEP_CPP_GETTE) "$(INTDIR)"

".\Release\Gettext.sbr" : $(SOURCE) $(DEP_CPP_GETTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Gettext.obj" : $(SOURCE) $(DEP_CPP_GETTE) "$(INTDIR)"

".\Debug\Gettext.sbr" : $(SOURCE) $(DEP_CPP_GETTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Getvar.c
DEP_CPP_GETVA=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Ident.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Getvar.obj" : $(SOURCE) $(DEP_CPP_GETVA) "$(INTDIR)"

".\Release\Getvar.sbr" : $(SOURCE) $(DEP_CPP_GETVA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Getvar.obj" : $(SOURCE) $(DEP_CPP_GETVA) "$(INTDIR)"

".\Debug\Getvar.sbr" : $(SOURCE) $(DEP_CPP_GETVA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Ident.c
DEP_CPP_IDENT=\
	".\Builtin.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Printf.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Ident.obj" : $(SOURCE) $(DEP_CPP_IDENT) "$(INTDIR)"

".\Release\Ident.sbr" : $(SOURCE) $(DEP_CPP_IDENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Ident.obj" : $(SOURCE) $(DEP_CPP_IDENT) "$(INTDIR)"

".\Debug\Ident.sbr" : $(SOURCE) $(DEP_CPP_IDENT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Iproc.c
DEP_CPP_IPROC=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Iproc.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Printf.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Iproc.obj" : $(SOURCE) $(DEP_CPP_IPROC) "$(INTDIR)"

".\Release\Iproc.sbr" : $(SOURCE) $(DEP_CPP_IPROC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Iproc.obj" : $(SOURCE) $(DEP_CPP_IPROC) "$(INTDIR)"

".\Debug\Iproc.sbr" : $(SOURCE) $(DEP_CPP_IPROC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Lex.c
DEP_CPP_LEX_C=\
	".\Environ.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Lex.obj" : $(SOURCE) $(DEP_CPP_LEX_C) "$(INTDIR)"

".\Release\Lex.sbr" : $(SOURCE) $(DEP_CPP_LEX_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Lex.obj" : $(SOURCE) $(DEP_CPP_LEX_C) "$(INTDIR)"

".\Debug\Lex.sbr" : $(SOURCE) $(DEP_CPP_LEX_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Musthave.c
DEP_CPP_MUSTH=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Prsif.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Musthave.obj" : $(SOURCE) $(DEP_CPP_MUSTH) "$(INTDIR)"

".\Release\Musthave.sbr" : $(SOURCE) $(DEP_CPP_MUSTH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Musthave.obj" : $(SOURCE) $(DEP_CPP_MUSTH) "$(INTDIR)"

".\Debug\Musthave.sbr" : $(SOURCE) $(DEP_CPP_MUSTH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Malloc.c
DEP_CPP_MALLO=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Malloc.h"\
	{$(INCLUDE)}"\malloc.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"

".\Release\Malloc.sbr" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Malloc.obj" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"

".\Debug\Malloc.sbr" : $(SOURCE) $(DEP_CPP_MALLO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parser.c
DEP_CPP_PARSE=\
	".\Enums.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Fndec.h"\
	".\Ident.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Procdec.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Record.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Parser.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"

".\Release\Parser.sbr" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Parser.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"

".\Debug\Parser.sbr" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Plant.c
DEP_CPP_PLANT=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Iproc.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Printf.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Plant.obj" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"

".\Release\Plant.sbr" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Plant.obj" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"

".\Debug\Plant.sbr" : $(SOURCE) $(DEP_CPP_PLANT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Plantexp.c
DEP_CPP_PLANTE=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Printf.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Plantexp.obj" : $(SOURCE) $(DEP_CPP_PLANTE) "$(INTDIR)"

".\Release\Plantexp.sbr" : $(SOURCE) $(DEP_CPP_PLANTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Plantexp.obj" : $(SOURCE) $(DEP_CPP_PLANTE) "$(INTDIR)"

".\Debug\Plantexp.sbr" : $(SOURCE) $(DEP_CPP_PLANTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Printf.c
DEP_CPP_PRINT=\
	".\Environ.h"\
	".\Printf.h"\
	".\Prsif.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Printf.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"

".\Release\Printf.sbr" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Printf.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"

".\Debug\Printf.sbr" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Procdec.c
DEP_CPP_PROCD=\
	".\Block.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Fplist.h"\
	".\Ident.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Procdec.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Procdec.obj" : $(SOURCE) $(DEP_CPP_PROCD) "$(INTDIR)"

".\Release\Procdec.sbr" : $(SOURCE) $(DEP_CPP_PROCD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Procdec.obj" : $(SOURCE) $(DEP_CPP_PROCD) "$(INTDIR)"

".\Debug\Procdec.sbr" : $(SOURCE) $(DEP_CPP_PROCD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pstring.c
DEP_CPP_PSTRI=\
	".\Environ.h"\
	".\Malloc.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Pstring.obj" : $(SOURCE) $(DEP_CPP_PSTRI) "$(INTDIR)"

".\Release\Pstring.sbr" : $(SOURCE) $(DEP_CPP_PSTRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Pstring.obj" : $(SOURCE) $(DEP_CPP_PSTRI) "$(INTDIR)"

".\Debug\Pstring.sbr" : $(SOURCE) $(DEP_CPP_PSTRI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Record.c
DEP_CPP_RECOR=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Record.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Record.obj" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"

".\Release\Record.sbr" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Record.obj" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"

".\Debug\Record.sbr" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Showexpr.c
DEP_CPP_SHOWE=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Printf.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Showexpr.obj" : $(SOURCE) $(DEP_CPP_SHOWE) "$(INTDIR)"

".\Release\Showexpr.sbr" : $(SOURCE) $(DEP_CPP_SHOWE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Showexpr.obj" : $(SOURCE) $(DEP_CPP_SHOWE) "$(INTDIR)"

".\Debug\Showexpr.sbr" : $(SOURCE) $(DEP_CPP_SHOWE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Simpstat.c
DEP_CPP_SIMPS=\
	".\Aplist.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Ident.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Simpstat.obj" : $(SOURCE) $(DEP_CPP_SIMPS) "$(INTDIR)"

".\Release\Simpstat.sbr" : $(SOURCE) $(DEP_CPP_SIMPS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Simpstat.obj" : $(SOURCE) $(DEP_CPP_SIMPS) "$(INTDIR)"

".\Debug\Simpstat.sbr" : $(SOURCE) $(DEP_CPP_SIMPS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Statemnt.c
DEP_CPP_STATE=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Expr.h"\
	".\Ident.h"\
	".\Iproc.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Plant.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Statemnt.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Statemnt.obj" : $(SOURCE) $(DEP_CPP_STATE) "$(INTDIR)"

".\Release\Statemnt.sbr" : $(SOURCE) $(DEP_CPP_STATE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Statemnt.obj" : $(SOURCE) $(DEP_CPP_STATE) "$(INTDIR)"

".\Debug\Statemnt.sbr" : $(SOURCE) $(DEP_CPP_STATE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Type.c
DEP_CPP_TYPE_=\
	".\Enums.h"\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Record.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Type.obj" : $(SOURCE) $(DEP_CPP_TYPE_) "$(INTDIR)"

".\Release\Type.sbr" : $(SOURCE) $(DEP_CPP_TYPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Type.obj" : $(SOURCE) $(DEP_CPP_TYPE_) "$(INTDIR)"

".\Debug\Type.sbr" : $(SOURCE) $(DEP_CPP_TYPE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Vars.c
DEP_CPP_VARS_=\
	".\Environ.h"\
	".\Errcode.h"\
	".\Error.h"\
	".\Ident.h"\
	".\Lex.h"\
	".\Malloc.h"\
	".\Musthave.h"\
	".\Parser.h"\
	".\Prsif.h"\
	".\Pstring.h"\
	".\Tokens.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Vars.obj" : $(SOURCE) $(DEP_CPP_VARS_) "$(INTDIR)"

".\Release\Vars.sbr" : $(SOURCE) $(DEP_CPP_VARS_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Vars.obj" : $(SOURCE) $(DEP_CPP_VARS_) "$(INTDIR)"

".\Debug\Vars.sbr" : $(SOURCE) $(DEP_CPP_VARS_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Version.c
DEP_CPP_VERSI=\
	".\Environ.h"\
	".\Prsif.h"\
	

!IF  "$(CFG)" == "Pfamws16 - Win32 Release"


".\Release\Version.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"

".\Release\Version.sbr" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pfamws16 - Win32 Debug"


".\Debug\Version.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"

".\Debug\Version.sbr" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
