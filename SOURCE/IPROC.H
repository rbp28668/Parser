/* 
// File:          IPROC.H
// 
// Prefix:        IP
// 
// Description:   Types and prototypes for I-Code interpreter.
//
*/

#ifndef _IPROC_H
#define _IPROC_H

typedef unsigned char Opcode;


/* Basic set of opcodes.  These may all be combined
// with one of the opcode modifiers
*/

enum IP_Codes
{
NOP,    /*                         ( no operation) */

ADD,    /* ia,ib  -  ia + ib       ( integer addition) */
SUB,    /* ia,ib  -  ia - ib       ( integer subtraction) */
MULT,   /* ia,ib  -  ia * ib       ( integer multiplication) */
DIV,    /* ia,ib  -  ia / ib       ( integer division) */
MOD,    /* ia,ib  -  ia MOD ib     ( integer modulus) */
AND,    /* ia,ib  -  ia AND ib     ( bitwise and) */
OR,     /* ia,ib  -  ia OR ib      ( bitwise or) */
NOT,    /* ia     -  NOT ia        ( 1s complement) */

FADD,   /* fa,fb  -  fa + fb       ( floating pt addition) */
FSUB,   /* fa,fb  -  fa - fb       ( floating pt subtraction) */
FMULT,  /* fa,fb  -  fa * fb       ( floating pt multiplication) */
FDIV,   /* fa,fb  -  fa / fb       ( floating pt division) */

ITOF,   /* ia     -  fa            ( integer to float) */
FTOI,   /* fa     -  ia            ( float to integer) */

EQU,    /* a,b    -  a = b         ( bitwise comparison ) */
NEQU,   /* a,b    -  a <> b        ( bitwise comparsion ) */

LT,     /* a,b    -  a < b         ( integer comparison ) */
GT,     /* a,b    -  a > b         ( integer comparsion ) */
LE,     /* a,b    -  a <= b        ( integer comparison ) */
GE,     /* a,b    -  a >= b        ( integer comparsion ) */

FLT,    /* fa,fb  -  fa < fb       ( floating pt comparison ) */
FGT,    /* fa,fb  -  fa > fb       ( floating pt comparison ) */
FLE,    /* fa,fb  -  fa <= fb      ( floating pt comparison ) */
FGE,    /* fa,fb  -  fa >= fb      ( floating pt comparison ) */

ZEQU,   /* bl     -  not-bl        ( boolean not (true if tos=0,else false) */

BRA,    /* offset  -               ( relative branch (IP = IP+offset)) */
ZBRA,   /* flg,off -               ( rel branch if tos = 0) */
CALL,   /* addr    -               ( PUSH(RSP) IP, IP = addr) */
ENTER,  /* n       -               ( set up stack frame for n bytes of locals) */
LEAVE,  /*         -               ( unwind stack frame ) */

RET,    /*         -               ( POP(RSP) -> IP ) */
NRET,   /* n       -               ( POP(RSP)->IP, RSP-=N ) */

CCALL,  /* addr    -               ( call C fn at addr) */

DUP,    /* n     -  n,n            ( duplicate TOS ) */
OVER,   /* a,b   -  a,b,a          ( pick 2nd TOS ) */
DROP,   /* a     -                 ( drop TOS ) */
PICK,   /* (..)n -  (..) n-th item ( pick n-TOS ) */
SWAP,   /* a,b   -  b,a            ( swap TOS and 2-TOS ) */
REPL,   /* a,b   -  b              ( replace 2-TOS with TOS ) */

ST,     /* v,a   -                 ( [a] = v) */
LD,     /* a     -  v              ( v = [a] ) */

CST,    /* c,a   -                 ( character store) */
CLD,    /* a     -  c              ( character fetch) */

WST,    /* w,a   -                 ( word store) */
WLD,    /* a     -                 ( word fetch) */

TOR,    /* n    -                  ( tos to return stack ) */
FROMR,  /*      - n                ( return stack to tos ) */

FPADD,  /* n    - FP+n             ( push FP+n ) */

FPLD,   /* n    - val              ( return stack[FP+n] ->tos ) */
FPST,   /* v,n  -                  ( 2tos -> return stack[FP+n] ) */

FPWLD,  /* n    - w                ( return stack[FP+n] -> tos (word load )) */
FPWST,  /* w,n  -                  ( 2tos -> (word) return stack[FP+n] ) */

FPCLD,  /* n    - c                ( return stack[FP+n] -> tos (char load )) */
FPCST,  /* c,n  -                  ( 2tos -> (char) return stack[FP+n] ) */

HALT,   /*         -               ( halt execution ) */
FAULT,  /* n       -               ( raise fault n & halt execution ) */
BND,    /* v,l,u - v               ( check l<=v<=u (integer), fault if not ) */

ALLOC,  /* n     - a               ( allocates n bytes on RS, returns its addr ) */
MOV,    /* s,d,n -                 ( move n bytes from source to dest ) */

SCLONE, /* p     - p               ( clones a string ) */
SDEL,   /* p     -                 ( deletes a string ) */

SLT,    /* - p.                    ( string literal: plants string addr & skips in code*/

ESC,
END_OPCODES
};


/* Opcode modifiers that determine whether the
// opcode is followed by no, a byte, word or
// long literal value.
*/
enum IP_Modifiers
{
  IP_Nothing = 0,
  IP_Byte    = 0x40,
  IP_Word    = 0x80,
  IP_Long    = 0xC0
};

/* Masks to seperate the 2 parts of an opcode */
#define IP_OpcodeMask 0x3f
#define IP_ModifierMask 0xC0

extern PFII PRS_SetFaultHandler(PFII FaultHandler);
extern void PRSI_InitialiseICode(void);
extern void PRSI_TerminateICode(void);

#ifndef NDEBUG
extern const char* IP_TranslateOpcode(Opcode op);
#endif



#endif
