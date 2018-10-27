# Microsoft Visual C++ generated build script - Do not modify

PROJ = PAS2BIN
DEBUG = 1
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\PROJECTS\PARSER\PAS2BIN\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = PAS2BIN.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W3 /Zi /AL /Gt16 /Od /D "_DEBUG" /D "_DOS" /FR /Fd"PAS2BIN.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /W3 /AL /Gt16 /Ox /D "NDEBUG" /D "_DOS" /FR 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE 
LIBS_D_DEXE = oldnames llibce 
LIBS_R_DEXE = oldnames llibce 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = ..\SOURCE\PFAMDS16.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = PAS2BIN.SBR \
		EXPAND.SBR


PAS2BIN_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\environ.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\pas2bin\expand.h


EXPAND_DEP = d:\projects\parser\pas2bin\expand.h


PFAMDS16_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

PAS2BIN.OBJ:	PAS2BIN.C $(PAS2BIN_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c PAS2BIN.C

EXPAND.OBJ:	EXPAND.C $(EXPAND_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c EXPAND.C

$(PROJ).EXE::	PAS2BIN.OBJ EXPAND.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
PAS2BIN.OBJ +
EXPAND.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
..\SOURCE\PFAMDS16.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
