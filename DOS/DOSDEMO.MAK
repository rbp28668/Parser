# Microsoft Visual C++ generated build script - Do not modify

PROJ = DOSDEMO
DEBUG = 1
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\PROJECTS\PARSER\DOS\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = CK_MAL.C    
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W3 /Zi /AL /Od /D "_DEBUG" /D "_DOS" /I "d:\projects\utils" /FR /Fd"DOSDEMO.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /W3 /AL /Ox /D "NDEBUG" /D "_DOS" /I "d:\projects\utils" /FR 
LFLAGS_D_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /CO /STACK:5120
LFLAGS_R_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /STACK:5120
LIBS_D_DEXE = oldnames llibce
LIBS_R_DEXE = oldnames llibce
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = ..\..\UTILS\DOSUTIL.LIB ..\LIB\PFAMDS16.LIB 
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
SBRS = CK_MAL.SBR \
		EXPAND.SBR \
		GET.SBR \
		GET_KEY.SBR \
		MAIN.SBR \
		MOUSE.SBR \
		PDRIVE.SBR \
		STRING.SBR


DOSUTIL_DEP = 

CK_MAL_DEP = d:\projects\parser\source\printf.h


EXPAND_DEP = d:\projects\parser\dos\expand.h


GET_DEP = d:\projects\parser\dos\get_key.h \
	d:\projects\parser\dos\mouse.h


GET_KEY_DEP = d:\projects\parser\dos\mouse.h \
	d:\projects\parser\dos\get_key.h


MAIN_DEP = d:\projects\parser\dos\expand.h \
	d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\environ.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\dos\pdrive.h \
	d:\projects\parser\dos\mouse.h \
	d:\projects\parser\dos\get.h


MOUSE_DEP = d:\projects\parser\dos\mouse.h


PDRIVE_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\environ.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\dos\pdrive.h


STRING_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\environ.h \
	d:\projects\parser\dos\pdrive.h


PFAMDS16_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

CK_MAL.OBJ:	CK_MAL.C $(CK_MAL_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c CK_MAL.C

EXPAND.OBJ:	EXPAND.C $(EXPAND_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c EXPAND.C

GET.OBJ:	GET.C $(GET_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GET.C

GET_KEY.OBJ:	GET_KEY.C $(GET_KEY_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GET_KEY.C

MAIN.OBJ:	MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MAIN.C

MOUSE.OBJ:	MOUSE.C $(MOUSE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MOUSE.C

PDRIVE.OBJ:	PDRIVE.C $(PDRIVE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PDRIVE.C

STRING.OBJ:	STRING.C $(STRING_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c STRING.C

$(PROJ).EXE::	CK_MAL.OBJ EXPAND.OBJ GET.OBJ GET_KEY.OBJ MAIN.OBJ MOUSE.OBJ PDRIVE.OBJ \
	STRING.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
CK_MAL.OBJ +
EXPAND.OBJ +
GET.OBJ +
GET_KEY.OBJ +
MAIN.OBJ +
MOUSE.OBJ +
PDRIVE.OBJ +
STRING.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
..\..\UTILS\DOSUTIL.LIB+
..\LIB\PFAMDS16.LIB+
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
