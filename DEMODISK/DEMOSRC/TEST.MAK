# Microsoft Visual C++ generated build script - Do not modify

PROJ = TEST
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\PROJECTS\PARSER\WINDOWS\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = INPUT.C     
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /G2 /FPi87 /Gy /W4 /Zi /AL /Od /D "_DEBUG" /FR /GA /Fd"TEST.PDB"
CFLAGS_R_WEXE = /nologo /G3 /FPi87 /Gy /W4 /AL /O1 /D "NDEBUG" /FR /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO /MAP /LINE  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = oldnames libw llibcew commdlg.lib 
LIBS_R_WEXE = oldnames libw llibcew commdlg.lib 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = TEST.DEF
OBJS_EXT = 
LIBS_EXT = ..\SOURCE\PFAMWS16.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = INPUT.SBR \
		FILE.SBR \
		FIND.SBR \
		FRAME.SBR \
		MAIN.SBR \
		OUTPUT.SBR \
		BUFFER.SBR \
		PDRIVE.SBR \
		ERROR.SBR \
		STRING.SBR


PFAMWS16_DEP = 

INPUT_DEP = d:\projects\parser\windows\app.h \
	d:\projects\parser\windows\frame.h \
	d:\projects\parser\windows\input.h \
	d:\projects\parser\windows\output.h \
	d:\projects\parser\windows\file.h \
	d:\projects\parser\windows\find.h \
	d:\projects\parser\windows\resource.h \
	d:\projects\parser\windows\prsif.h \
	d:\projects\parser\windows\environ.h


FILE_DEP = d:\projects\parser\windows\file.h


FIND_DEP = d:\projects\parser\windows\find.h


FRAME_DEP = d:\projects\parser\windows\resource.h \
	d:\projects\parser\windows\frame.h \
	d:\projects\parser\windows\input.h \
	d:\projects\parser\windows\output.h \
	d:\projects\parser\windows\app.h


MAIN_DEP = d:\projects\parser\windows\resource.h \
	d:\projects\parser\windows\prsif.h \
	d:\projects\parser\windows\environ.h \
	d:\projects\parser\windows\frame.h \
	d:\projects\parser\windows\input.h \
	d:\projects\parser\windows\output.h \
	d:\projects\parser\windows\error.h \
	d:\projects\parser\windows\app.h \
	d:\projects\parser\windows\buffer.h \
	d:\projects\parser\windows\pdrive.h


OUTPUT_DEP = d:\projects\parser\windows\output.h \
	d:\projects\parser\windows\input.h \
	d:\projects\parser\windows\frame.h \
	d:\projects\parser\windows\buffer.h \
	d:\projects\parser\windows\app.h \
	d:\projects\parser\windows\resource.h


DEMO_RCDEP = d:\projects\parser\windows\resource.h \
	d:\projects\parser\windows\pfa.ico \
	d:\projects\parser\windows\input.ico \
	d:\projects\parser\windows\output.ico


BUFFER_DEP = d:\projects\parser\windows\buffer.h


PDRIVE_DEP = d:\projects\parser\windows\prsif.h \
	d:\projects\parser\windows\environ.h \
	d:\projects\parser\windows\pstring.h \
	d:\projects\parser\windows\pdrive.h \
	d:\projects\parser\windows\frame.h \
	d:\projects\parser\windows\resource.h


ERROR_DEP = d:\projects\parser\windows\error.h \
	d:\projects\parser\windows\app.h


STRING_DEP = d:\projects\parser\windows\prsif.h \
	d:\projects\parser\windows\environ.h \
	d:\projects\parser\windows\pdrive.h


all:	$(PROJ).EXE $(PROJ).BSC

INPUT.OBJ:	INPUT.C $(INPUT_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c INPUT.C

FILE.OBJ:	FILE.C $(FILE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c FILE.C

FIND.OBJ:	FIND.C $(FIND_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c FIND.C

FRAME.OBJ:	FRAME.C $(FRAME_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c FRAME.C

MAIN.OBJ:	MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MAIN.C

OUTPUT.OBJ:	OUTPUT.C $(OUTPUT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c OUTPUT.C

DEMO.RES:	DEMO.RC $(DEMO_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r DEMO.RC

BUFFER.OBJ:	BUFFER.C $(BUFFER_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c BUFFER.C

PDRIVE.OBJ:	PDRIVE.C $(PDRIVE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PDRIVE.C

ERROR.OBJ:	ERROR.C $(ERROR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ERROR.C

STRING.OBJ:	STRING.C $(STRING_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c STRING.C


$(PROJ).EXE::	DEMO.RES

$(PROJ).EXE::	INPUT.OBJ FILE.OBJ FIND.OBJ FRAME.OBJ MAIN.OBJ OUTPUT.OBJ BUFFER.OBJ \
	PDRIVE.OBJ ERROR.OBJ STRING.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
INPUT.OBJ +
FILE.OBJ +
FIND.OBJ +
FRAME.OBJ +
MAIN.OBJ +
OUTPUT.OBJ +
BUFFER.OBJ +
PDRIVE.OBJ +
ERROR.OBJ +
STRING.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
..\SOURCE\PFAMWS16.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) DEMO.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	DEMO.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) DEMO.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
