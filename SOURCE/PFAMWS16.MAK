# Microsoft Visual C++ generated build script - Do not modify

PROJ = PFAMWS16
DEBUG = 0
PROGTYPE = 4
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\PROJECTS\PARSER\SOURCE\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = AMBLE.C     
FIRSTCPP =             
RC = rc
CFLAGS_D_LIB = /nologo /G2 /FPi87 /Gy /W4 /Z7 /AL /Od /D "_DEBUG" /D "NOCHECK" /FR /GA 
CFLAGS_R_LIB = /nologo /Gs /G3 /FPi87 /Gy /W4 /AL /D "NDEBUG" /D "NOCHECK" /FR /GA 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = AMBLE.SBR \
		APLIST.SBR \
		BLOCK.SBR \
		BUILTIN.SBR \
		CCALL.SBR \
		CONSTANT.SBR \
		ENUMS.SBR \
		ERROR.SBR \
		EXPR.SBR \
		EXPRTYPE.SBR \
		FNDEC.SBR \
		FPLIST.SBR \
		GETTEXT.SBR \
		GETVAR.SBR \
		IDENT.SBR \
		IPROC.SBR \
		LEX.SBR \
		MUSTHAVE.SBR \
		MALLOC.SBR \
		PARSER.SBR \
		PLANT.SBR \
		PLANTEXP.SBR \
		PRINTF.SBR \
		PROCDEC.SBR \
		PSTRING.SBR \
		RECORD.SBR \
		SHOWEXPR.SBR \
		SIMPSTAT.SBR \
		STATEMNT.SBR \
		TYPE.SBR \
		VARS.SBR \
		VERSION.SBR


AMBLE_DEP = d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\amble.h


APLIST_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\aplist.h


BLOCK_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\statemnt.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\block.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\amble.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h


BUILTIN_DEP = d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\builtin.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h


CCALL_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\fndec.h \
	d:\projects\parser\source\procdec.h \
	d:\projects\parser\source\ident.h


CONSTANT_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\record.h \
	d:\projects\parser\source\enums.h


ENUMS_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\enums.h


ERROR_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\gettext.h


EXPR_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\aplist.h


EXPRTYPE_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\aplist.h


FNDEC_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\block.h \
	d:\projects\parser\source\fplist.h \
	d:\projects\parser\source\fndec.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\iproc.h


FPLIST_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\procdec.h \
	d:\projects\parser\source\fndec.h


GETTEXT_DEP = 

GETVAR_DEP = d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\iproc.h


IDENT_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\builtin.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\printf.h


IPROC_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\printf.h


LEX_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\iproc.h


MUSTHAVE_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\musthave.h


MALLOC_DEP = d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h


PARSER_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\record.h \
	d:\projects\parser\source\enums.h \
	d:\projects\parser\source\procdec.h \
	d:\projects\parser\source\fndec.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\plant.h


PLANT_DEP = d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\printf.h


PLANTEXP_DEP = d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\printf.h


PRINTF_DEP = d:\projects\parser\source\printf.h \
	d:\projects\parser\source\prsif.h


PROCDEC_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\block.h \
	d:\projects\parser\source\fplist.h \
	d:\projects\parser\source\procdec.h \
	d:\projects\parser\source\plant.h \
	d:\projects\parser\source\iproc.h


PSTRING_DEP = d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\prsif.h


RECORD_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\record.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\musthave.h


SHOWEXPR_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\printf.h


SIMPSTAT_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\aplist.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\plant.h


STATEMNT_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\expr.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\statemnt.h \
	d:\projects\parser\source\iproc.h \
	d:\projects\parser\source\plant.h


TYPE_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\record.h \
	d:\projects\parser\source\enums.h


VARS_DEP = d:\projects\parser\source\prsif.h \
	d:\projects\parser\source\lex.h \
	d:\projects\parser\source\tokens.h \
	d:\projects\parser\source\parser.h \
	d:\projects\parser\source\pstring.h \
	d:\projects\parser\source\musthave.h \
	d:\projects\parser\source\ident.h \
	d:\projects\parser\source\error.h \
	d:\projects\parser\source\errcode.h


VERSION_DEP = d:\projects\parser\source\prsif.h


all:	$(PROJ).LIB $(PROJ).BSC

AMBLE.OBJ:	AMBLE.C $(AMBLE_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c AMBLE.C

APLIST.OBJ:	APLIST.C $(APLIST_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c APLIST.C

BLOCK.OBJ:	BLOCK.C $(BLOCK_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c BLOCK.C

BUILTIN.OBJ:	BUILTIN.C $(BUILTIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c BUILTIN.C

CCALL.OBJ:	CCALL.C $(CCALL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CCALL.C

CONSTANT.OBJ:	CONSTANT.C $(CONSTANT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CONSTANT.C

ENUMS.OBJ:	ENUMS.C $(ENUMS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ENUMS.C

ERROR.OBJ:	ERROR.C $(ERROR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ERROR.C

EXPR.OBJ:	EXPR.C $(EXPR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c EXPR.C

EXPRTYPE.OBJ:	EXPRTYPE.C $(EXPRTYPE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c EXPRTYPE.C

FNDEC.OBJ:	FNDEC.C $(FNDEC_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c FNDEC.C

FPLIST.OBJ:	FPLIST.C $(FPLIST_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c FPLIST.C

GETTEXT.OBJ:	GETTEXT.C $(GETTEXT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GETTEXT.C

GETVAR.OBJ:	GETVAR.C $(GETVAR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GETVAR.C

IDENT.OBJ:	IDENT.C $(IDENT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c IDENT.C

IPROC.OBJ:	IPROC.C $(IPROC_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c IPROC.C

LEX.OBJ:	LEX.C $(LEX_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LEX.C

MUSTHAVE.OBJ:	MUSTHAVE.C $(MUSTHAVE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MUSTHAVE.C

MALLOC.OBJ:	MALLOC.C $(MALLOC_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MALLOC.C

PARSER.OBJ:	PARSER.C $(PARSER_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PARSER.C

PLANT.OBJ:	PLANT.C $(PLANT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PLANT.C

PLANTEXP.OBJ:	PLANTEXP.C $(PLANTEXP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PLANTEXP.C

PRINTF.OBJ:	PRINTF.C $(PRINTF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PRINTF.C

PROCDEC.OBJ:	PROCDEC.C $(PROCDEC_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PROCDEC.C

PSTRING.OBJ:	PSTRING.C $(PSTRING_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PSTRING.C

RECORD.OBJ:	RECORD.C $(RECORD_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c RECORD.C

SHOWEXPR.OBJ:	SHOWEXPR.C $(SHOWEXPR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SHOWEXPR.C

SIMPSTAT.OBJ:	SIMPSTAT.C $(SIMPSTAT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SIMPSTAT.C

STATEMNT.OBJ:	STATEMNT.C $(STATEMNT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c STATEMNT.C

TYPE.OBJ:	TYPE.C $(TYPE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c TYPE.C

VARS.OBJ:	VARS.C $(VARS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VARS.C

VERSION.OBJ:	VERSION.C $(VERSION_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VERSION.C

$(PROJ).LIB::	AMBLE.OBJ APLIST.OBJ BLOCK.OBJ BUILTIN.OBJ CCALL.OBJ CONSTANT.OBJ ENUMS.OBJ \
	ERROR.OBJ EXPR.OBJ EXPRTYPE.OBJ FNDEC.OBJ FPLIST.OBJ GETTEXT.OBJ GETVAR.OBJ IDENT.OBJ \
	IPROC.OBJ LEX.OBJ MUSTHAVE.OBJ MALLOC.OBJ PARSER.OBJ PLANT.OBJ PLANTEXP.OBJ PRINTF.OBJ \
	PROCDEC.OBJ PSTRING.OBJ RECORD.OBJ SHOWEXPR.OBJ SIMPSTAT.OBJ STATEMNT.OBJ TYPE.OBJ \
	VARS.OBJ VERSION.OBJ $(OBJS_EXT)
	echo >NUL @<<$(PROJ).CRF
$@ /PAGESIZE:64
y
+AMBLE.OBJ &
+APLIST.OBJ &
+BLOCK.OBJ &
+BUILTIN.OBJ &
+CCALL.OBJ &
+CONSTANT.OBJ &
+ENUMS.OBJ &
+ERROR.OBJ &
+EXPR.OBJ &
+EXPRTYPE.OBJ &
+FNDEC.OBJ &
+FPLIST.OBJ &
+GETTEXT.OBJ &
+GETVAR.OBJ &
+IDENT.OBJ &
+IPROC.OBJ &
+LEX.OBJ &
+MUSTHAVE.OBJ &
+MALLOC.OBJ &
+PARSER.OBJ &
+PLANT.OBJ &
+PLANTEXP.OBJ &
+PRINTF.OBJ &
+PROCDEC.OBJ &
+PSTRING.OBJ &
+RECORD.OBJ &
+SHOWEXPR.OBJ &
+SIMPSTAT.OBJ &
+STATEMNT.OBJ &
+TYPE.OBJ &
+VARS.OBJ &
+VERSION.OBJ &
;
<<
	if exist $@ del $@
	lib @$(PROJ).CRF

$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
