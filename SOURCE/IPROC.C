/*
// File:        IPROC.C
//
// Prefix:      IP_
//
// Description: Intermediate code processor.
//              This implements a simple stack based processor.
//              There are 4 registers:
//              IP is the Instruction Pointer
//              ESP is the evaluation stack pointer
//              RSP is the return stack pointer
//              FP is a frame pointer which is used to set up
//              stack frames on the return stack.
//              Note that the IP is used as a running flag: when
//              IP is non-NULL the processor is running, when NULL
//              it is halted.
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include "prsif.h"
#include "iproc.h"
#include "error.h"
#include "pstring.h"
#include "malloc.h"
#include "parser.h"
#include "plant.h"
#include "printf.h"

#define RETURN_STACK_SIZE 4096  /* in bytes */
#define EVAL_STACK_SIZE 256     /* in longs */


/* Default fault handler */
static int DefaultFaultHandler(int fault);
static PFII FaultHandler = DefaultFaultHandler;  /* I-Code processor fault-handler*/

static char* ReturnStack = NULL;
static long* EvalStack = NULL;

static Opcode* IP = 0;/* Instruction pointer */
static char* FP = 0;  /* Frame pointer */

static int RSP = 0;   /* Return stack pointer */
static int ESP = 0;   /* Evaluation stack pointer */



#define PUSH(n) {if(ESP >= EVAL_STACK_SIZE)\
                fault = EVAL_STACK_OVERFLOW_FAULT;\
                else EvalStack[ESP++] = n;}



#if !defined NDEBUG && defined SHOW


typedef struct s_opcodename
{
  char *name;
  Opcode op;
}OpcodeName;

static OpcodeName OpcodeTranslationTable[] =
{
{ "NOP",   NOP},
{ "ADD",   ADD},
{ "SUB",   SUB},
{ "MULT",  MULT},
{ "DIV",   DIV},
{ "MOD",   MOD},
{ "AND",   AND},
{ "OR",    OR},
{ "NOT",   NOT},
{ "FADD",  FADD},
{ "FSUB",  FSUB},
{ "FMULT", FMULT},
{ "FDIV",  FDIV},
{ "ITOF",  ITOF},
{ "FTOI",  FTOI},
{ "EQU",   EQU},
{ "NEQU",  NEQU},
{ "LT",    LT},
{ "GT",    GT},
{ "LE",    LE},
{ "GE",    GE},
{ "FLT",   FLT},
{ "FGT",   FGT},
{ "FLE",   FLE},
{ "FGE",   FGE},
{ "ZEQU",  ZEQU},
{ "BRA",   BRA},
{ "ZBRA",  ZBRA},
{ "CALL",  CALL},
{ "ENTER", ENTER},
{ "LEAVE", LEAVE},
{ "RET",   RET},
{ "NRET",  NRET},
{ "CCALL", CCALL},
{ "DUP",   DUP},
{ "OVER",  OVER},
{ "DROP",  DROP},
{ "PICK",  PICK},
{ "SWAP",  SWAP},
{ "REPL",  REPL},
{ "ST",    ST},
{ "LD",    LD},
{ "CST",   CST},
{ "CLD",   CLD},
{ "WST",   WST},
{ "WLD",   WLD},
{ "TOR",   TOR},
{ "FROMR", FROMR},
{ "FPADD", FPADD},
{ "FPLD",  FPLD},
{ "FPST",  FPST},
{ "FPWLD", FPWLD},
{ "FPWST", FPWST},
{ "FPCLD", FPCLD},
{ "FPCST", FPCST},
{ "HALT",  HALT},
{ "FAULT", FAULT},
{ "BND",   BND},
{ "ALLOC", ALLOC},
{ "MOV",   MOV},
{ "SCLONE",SCLONE},
{ "SDEL",  SDEL},
{ "SLT",   SLT},
{ "ESC",   ESC},
{ NULL,(Opcode)-1 }
};


#endif

#if !defined NDEBUG && defined SHOW

static int CallLevel = 0;

/**********************************************************************
* ShowStack
* Prints out the evaluation stack
**********************************************************************/
static void ShowStack(void)
{
  int i;

  for(i=0;i<ESP;++i)
    PRSI_printf(" %ld",EvalStack[i]);
}

/**********************************************************************
* show
* Prints the opcode indented by the current call level (using >)
**********************************************************************/
static void show(Opcode op)
{
  int i;
  PRSI_printf("%p: ",IP-1); /* -1 as IP will already have been incremented */
  for(i=-1;i<CallLevel;++i)
    PRSI_printf(">");
  PRSI_printf("%s",IP_TranslateOpcode(op));
}

#endif

/**********************************************************************
* PRSI_InitialiseICode
* Initialises the I-Code interpreter
**********************************************************************/
void PRSI_InitialiseICode(void)
{
  ReturnStack = (char*) malloc(RETURN_STACK_SIZE);
  EvalStack = (long *)malloc(EVAL_STACK_SIZE * sizeof(long));

  if(ReturnStack == NULL || EvalStack == NULL)
    {
    PRS_Error(IDS_ERR_IPROC_MEMORY);
    if(ReturnStack != NULL)
      free(ReturnStack);
    if(EvalStack != NULL)
      free(EvalStack);
    ReturnStack = NULL;
    EvalStack = NULL;
    }

  FP = ReturnStack;
  RSP = 0;
  ESP = 0;
  IP = 0;

  #if !defined NDEBUG && defined SHOW
  PRSI_printf("ReturnStack at %p, EvalStack at %p, FP at %p\n",
    ReturnStack,EvalStack,FP);
  #endif

  return;
}

/**********************************************************************
* PRSI_TerminateICode
* Terminates the I-Code interpeter
**********************************************************************/
void PRSI_TerminateICode(void)
{
  if(ReturnStack != NULL)
    free(ReturnStack);
  if(EvalStack != NULL)
    free(EvalStack);
  ReturnStack = NULL;
  EvalStack = NULL;
}

/**********************************************************************
* Function:     DefaultFaultHandler
*
* This translates the the fault code to a message, displays the
* message and returns the fault unchanged.
* Parameters:   fault is the fault code
*
* Returns:      The processor fault state
**********************************************************************/
static int DefaultFaultHandler(int fault)
{
  switch (fault)
    {
    case NO_FAULT:
      break;

    case HALT_FAULT:
      #if !defined NDEBUG && defined SHOW
      PRSI_printf("Processor Halted\n");
      #endif
      break;

    case BOUNDS_FAULT:
      PRS_Error(IDS_ERR_IPROC_BOUNDS);
      break;

    case ZDIV_FAULT:
      PRS_Error(IDS_ERR_IPROC_DIV_ZERO);
      break;

    case EVAL_STACK_OVERFLOW_FAULT:
      PRS_Error(IDS_ERR_IPROC_EVAL_STACK_OVER);
      break;

    case EVAL_STACK_UNDERFLOW_FAULT:
      PRS_Error(IDS_ERR_IPROC_EVAL_STACK_UNDER);
      break;

    case RETURN_STACK_OVERFLOW_FAULT:
      PRS_Error(IDS_ERR_IPROC_RETURN_STACK_OVER);
      break;

    case RETURN_STACK_UNDERFLOW_FAULT:
      PRS_Error(IDS_ERR_IPROC_RETURN_STACK_UNDER);
      break;

    case NULL_CALL_FAULT:
      PRS_Error(IDS_ERR_IPROC_NULL_CALL);
      break;

    case MISMATCH_CASE_FAULT:
      PRS_Error(IDS_ERR_IPROC_MISMATCH_CASE);
      break;

    case UNIMPLEMENTED_INSTR_FAULT:
      PRS_Error(IDS_ERR_IPROC_UNIMP);
      break;

    default:
      PRS_Error(IDS_ERR_IPROC_UNKNOWN_ERR,fault);
      break;
    }
  return fault;
}

/**********************************************************************
* Function:     Page2
*
* This processes a single Page2 opcode after the ESC code has been
* read processed.
* Parameters:   NONE
*
* Returns:      The processor fault state
**********************************************************************/
static int Page2()
{
  int iFault = 0;
  assert(FALSE); /* no page 2 opcodes yet */
  return iFault;
}

/**********************************************************************
* Function:     ProcessOpcode
*
* This processes a single opcode.
* Parameters:   NONE
*
* Returns:      The processor fault state
**********************************************************************/
static int ProcessOpcode(void)
{
  int nModifier = *IP & IP_ModifierMask;
  int nOpcode = *IP++ & IP_OpcodeMask;
  int fault = NO_FAULT;

  #if !defined NDEBUG && defined SHOW
  show(nOpcode);
  switch (nModifier)
    {
    case IP_Nothing :
      break;

    case IP_Byte:
      PRSI_printf(".B(%d)",(int)(*(signed char *)IP));
      break;

    case IP_Word:
      PRSI_printf(".W(%d)",*(int *)IP);
      break;

    case IP_Long:
      PRSI_printf(".L(%ld)",*(long *)IP);
      break;
    }

  ShowStack();
  PRSI_printf(" - ");
  #endif

  switch (nModifier)
    {
    case IP_Nothing :
      break;

    case IP_Byte:
      PUSH((long)*(signed char *)IP++);
      break;

    case IP_Word:
      PUSH((long)*(int *)IP);
      IP += sizeof(int);
      break;

    case IP_Long:
      PUSH(*(long *)IP);
      IP += sizeof(long);
      break;

    }


  switch (nOpcode)
    {
    case NOP:    /* no operation) */
      break;

    /*=============================================
    * INTEGER ARITHMETIC                          *
    =============================================*/
    
    case ADD:    /* integer addition) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] += EvalStack[ESP-1];
        --ESP;
        }
      break;

    case SUB:    /* integer subtraction) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] -= EvalStack[ESP-1];
        --ESP;
        }
      break;

    case MULT:   /* integer multiplication) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] *= EvalStack[ESP-1];
        --ESP;
        }
      break;

    case DIV:    /* integer division) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        if(EvalStack[ESP-1] == 0)
          {
          fault = ZDIV_FAULT;
          EvalStack[ESP-2] = LONG_MAX;
          }
        else
          EvalStack[ESP-2] /= EvalStack[ESP-1];
        --ESP;
        }
      break;

    case MOD:    /* integer modulus) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        if(EvalStack[ESP-1] == 0)
          {
          fault = ZDIV_FAULT;
          EvalStack[ESP-2] = 0;
          }
        else
          EvalStack[ESP-2] %= EvalStack[ESP-1];
        --ESP;
        }
      break;

    case AND:    /* bitwise and) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] &= EvalStack[ESP-1];
        --ESP;
        }
      break;

    case OR:     /* bitwise or) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] |= EvalStack[ESP-1];
        --ESP;
        }
      break;

    case NOT:    /* 1s complement) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = ~EvalStack[ESP-1];
      break;

    /*=============================================
    * FLOATING POINT ARITHMETIC                   *
    =============================================*/

    case FADD:   /* floating pt addition) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        ((float*)EvalStack)[ESP-2] += ((float*)EvalStack)[ESP-1];
        --ESP;
        }
      break;

    case FSUB:   /* floating pt subtraction) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        ((float*)EvalStack)[ESP-2] -= ((float*)EvalStack)[ESP-1];
        --ESP;
        }
      break;

    case FMULT:  /* floating pt multiplication) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        ((float*)EvalStack)[ESP-2] *= ((float*)EvalStack)[ESP-1];
        --ESP;
        }
      break;

    case FDIV:   /* floating pt division) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        if(((float*)EvalStack)[ESP-1] == 0.0f)
          {
          fault = ZDIV_FAULT;
          ((float*)EvalStack)[ESP-2] = FLT_MAX;
          }
        else
          ((float*)EvalStack)[ESP-2] /= ((float*)EvalStack)[ESP-1];
        --ESP;
        }
      break;

    /*=============================================
    * FLOAT TO INTEGER TO FLOAT CONVERSION        *
    =============================================*/
    case ITOF:   /* integer to float) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        ((float*)EvalStack)[ESP-1] = (float)EvalStack[ESP-1];
      break;

    case FTOI:   /* float to integer) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = (long)((float*)EvalStack)[ESP-1];
      break;

    /*=============================================
    * COMPARISON OPERATORS                        *
    =============================================*/
    case EQU:    /* bitwise comparison ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (EvalStack[ESP-2]==EvalStack[ESP-1]);
        --ESP;
        }
      break;

    case NEQU:   /* bitwise comparsion ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (EvalStack[ESP-2]!=EvalStack[ESP-1]);
        --ESP;
        }
      break;

    case LT:     /* integer comparison ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (EvalStack[ESP-2]<EvalStack[ESP-1]);
        --ESP;
        }
      break;

    case GT:     /* integer comparsion ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (EvalStack[ESP-2]>EvalStack[ESP-1]);
        --ESP;
        }
      break;

    case LE:     /* integer comparison ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (EvalStack[ESP-2]<=EvalStack[ESP-1]);
        --ESP;
        }
      break;

    case GE:     /* integer comparsion ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (EvalStack[ESP-2]>=EvalStack[ESP-1]);
        --ESP;
        }
      break;

    case FLT:    /* floating pt comparison ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (((float*)EvalStack)[ESP-2] <
                            ((float*)EvalStack)[ESP-1]);
        --ESP;
        }
      break;

    case FGT:    /* floating pt comparison ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (((float*)EvalStack)[ESP-2] >
                            ((float*)EvalStack)[ESP-1]);
        --ESP;
        }
      break;

    case FLE:    /* floating pt comparison ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (((float*)EvalStack)[ESP-2] <=
                            ((float*)EvalStack)[ESP-1]);
        --ESP;
        }
      break;

    case FGE:    /* floating pt comparison ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = (((float*)EvalStack)[ESP-2] >=
                            ((float*)EvalStack)[ESP-1]);
        --ESP;
        }
      break;

    case ZEQU:   /* boolean not (true if tos=0,else false) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = !EvalStack[ESP-1];
      break;

    /*=============================================
    * FLOW CONTROL                                *
    =============================================*/

    case BRA:    /* relative branch (IP = IP+offset)) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        IP += (int)EvalStack[--ESP];

      #ifdef SHOW
      PRSI_printf(" -->%p ",IP);
      #endif
      break;

    case ZBRA:   /* rel branch if tos = 0) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        if(EvalStack[ESP-2] == 0)
          {
          IP += (int)EvalStack[--ESP];
          #ifdef SHOW
          PRSI_printf(" -->%p ",IP);
          #endif
          }
        else
          {
          --ESP;
          #ifdef SHOW
          PRSI_printf(" NJ>%p ",IP);
          #endif
          }
        --ESP;  /* drop flag */
        }
      break;

    case CALL:   /* PUSH(RSP) IP, IP = addr) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else if (EvalStack[ESP-1] == 0)
        fault = NULL_CALL_FAULT;
      else
        {
        Opcode* OldIP = IP;
        IP = (Opcode*)(EvalStack[--ESP]);
        *(Opcode **)(ReturnStack+RSP) = OldIP;
        RSP += sizeof(Opcode*);
        }

      #ifdef SHOW
      ++CallLevel;
      #endif
      break;

    case ENTER:  /* set up stack frame for n bytes of locals) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        *(char **)(ReturnStack+RSP) = FP;  /* PUSH(RSP) FP */
        RSP += sizeof(char *);
        FP = ReturnStack+RSP;              /* RSP->FP */
        RSP += (int)EvalStack[--ESP];           /* RSP += N */
        }
      break;

    case LEAVE:  /* unwind stack frame ) */
      RSP = (int)(FP-ReturnStack);               /*  FP->RSP */
      RSP -= sizeof(char *);
      FP = *(char**)(ReturnStack+RSP);    /*  POP(RSP) FP */
      break;

    case RET:    /* POP(RSP) -> IP ) */
      RSP -= sizeof(Opcode *);
      IP = *(Opcode**)(ReturnStack+RSP);    /*  POP(RSP) IP */
      #ifdef SHOW
      --CallLevel;
      #endif
      break;

    case NRET:   /* POP(RSP)->IP, RSP-=N ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        RSP -= sizeof(Opcode *);
        IP = *(Opcode**)(ReturnStack+RSP);    /*  POP(RSP) IP */
        RSP -= (int)EvalStack[--ESP];         /* RSP -= N */
        }
      #ifdef SHOW
      --CallLevel;
      #endif
      break;

    case CCALL:  /* call C fn at addr) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else if (EvalStack[ESP-1] == 0)
        fault = NULL_CALL_FAULT;
      else
        {
        /* Save IP: this allows the C-Call stuff to
        // start up another process, hopefully without breaking
        // what we have here.
        */
        Opcode* OldIP = IP;
        /* Hopefully this should cast EvalStack[--ESP] to a
        // pointer to a void function returning int and call
        // this function !
        */
        fault = (*(PFVI)EvalStack[--ESP] )();

        /* Restore IP  */
        IP = OldIP;
        }
      break;

    /*=============================================
    * STACK MANIPULATION                          *
    =============================================*/

    case DUP:    /* duplicate TOS ) */
      if(ESP >= EVAL_STACK_SIZE)
        fault = EVAL_STACK_OVERFLOW_FAULT;
      else
        {
        EvalStack[ESP] = EvalStack[ESP-1];
        ++ESP;
        }
      break;

    case OVER:   /* pick 2nd TOS ) */
      if(ESP >= EVAL_STACK_SIZE)
        fault = EVAL_STACK_OVERFLOW_FAULT;
      else
        {
        EvalStack[ESP] = EvalStack[ESP-2];
        ++ESP;
        }
      break;

    case DROP:   /* drop TOS ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        --ESP;
      break;

    case PICK:   /* pick n-TOS ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else if(ESP-1-EvalStack[ESP-1] < 0)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = EvalStack[ESP-1-(int)EvalStack[ESP-1]];
      break;

    case SWAP:   /* swap TOS and 2-TOS ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP] = EvalStack[ESP-2];
        EvalStack[ESP-2] = EvalStack[ESP-1];
        EvalStack[ESP-1] = EvalStack[ESP];
        }
      break;

    case REPL:   /* replace 2-TOS with TOS ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        EvalStack[ESP-2] = EvalStack[ESP-1];
        --ESP;
        }
      break;

    /*=============================================
    * LOAD and STORE                              *
    =============================================*/

    case ST:     /* [a] = v) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        *(long *)(EvalStack[ESP-1]) = EvalStack[ESP-2];
        ESP -= 2;
        }
      break;

    case LD:     /* v = [a] ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = *(long *)(EvalStack[ESP-1]);
      break;

    case CST:    /* character store) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        *(unsigned char *)(EvalStack[ESP-1]) = (unsigned char)EvalStack[ESP-2];
        ESP -= 2;
        }
      break;

    case CLD:    /* character fetch) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = (long)*(unsigned char *)(EvalStack[ESP-1]);
      break;

    case WST:    /* word store) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        *(int *)(EvalStack[ESP-1]) = (int)EvalStack[ESP-2];
        ESP -= 2;
        }
      break;

    case WLD:    /* word fetch) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = (long)*(int *)(EvalStack[ESP-1]);
      break;

    case TOR:    /* tos to return stack ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else if (RSP >= RETURN_STACK_SIZE-sizeof(long))
        fault = RETURN_STACK_OVERFLOW_FAULT;
      else
        {
        *((long *)(ReturnStack+RSP)) = EvalStack[--ESP];
        RSP += sizeof(long);
        }
      break;

    case FROMR:  /* return stack to tos ) */
      if(ESP >= EVAL_STACK_SIZE)
        fault = EVAL_STACK_OVERFLOW_FAULT;
      else if (RSP < sizeof(long))
        fault = RETURN_STACK_UNDERFLOW_FAULT;
      else
        {
        RSP -= sizeof(long);
        EvalStack[ESP++] = *(long *)(ReturnStack+RSP);
        }
      break;

    case FPADD:  /* push FP+n ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = (long)FP + EvalStack[ESP-1];
      break;

    case FPLD:   /* return stack[FP+n] ->tos ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = *(long*)(FP+(int)EvalStack[ESP-1]);
      break;

    case FPST:   /* 2tos -> return stack[FP+n] ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        *(long*)(FP+(int)EvalStack[ESP-1]) = EvalStack[ESP-2];
        ESP -= 2;
        }
      break;

    case FPWLD:  /* return stack[FP+n] -> tos (word load )) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = (long)*(int*)(FP+(int)EvalStack[ESP-1]);
      break;

    case FPWST:  /* 2tos -> (word) return stack[FP+n] ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        *(int*)(FP+(int)EvalStack[ESP-1]) = (int)EvalStack[ESP-2];
        ESP -= 2;
        }
      break;

    case FPCLD:  /* return stack[FP+n] -> tos (char load )) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = (long)*(unsigned char*)(FP+(int)EvalStack[ESP-1]);
      break;

    case FPCST:  /* 2tos -> (char) return stack[FP+n] ) */
      if(ESP < 2)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        *(unsigned char*)(FP+(int)EvalStack[ESP-1]) = (unsigned char)EvalStack[ESP-2];
        ESP -= 2;
        }
      break;

    case HALT:   /* halt execution ) */
      fault = HALT_FAULT;
      break;

    case FAULT:  /* raise fault n & halt execution ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        fault = (int)EvalStack[--ESP];
      break;

    case BND:    /* check l<=v<=u (integer), fault if not ) */
      if(ESP < 3)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        if(EvalStack[ESP-3] < EvalStack[ESP-2] ||  /* val < lower */
           EvalStack[ESP-3] > EvalStack[ESP-1])    /* val > upper */
          fault = BOUNDS_FAULT;
        ESP -= 2;
        }
      break;

    case ALLOC:  /* ( allocates n bytes on RS, returns its addr ) */
      if(ESP >= EVAL_STACK_SIZE)
        fault = EVAL_STACK_OVERFLOW_FAULT;
      else if (ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        long lBytes = EvalStack[ESP-1];
        EvalStack[ESP-1] = (long)(ReturnStack+RSP);
        RSP += (int)lBytes;
        if(RSP > RETURN_STACK_SIZE)
          fault = RETURN_STACK_OVERFLOW_FAULT;
        }
      break;

    case MOV:    /* ( move n bytes from source to dest ) */
      if(ESP < 3)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        {
        memcpy((void*)EvalStack[ESP-2],
               (void*)EvalStack[ESP-3],
               (int)EvalStack[ESP-1]);
        ESP -= 3;
        }
      break;


    case SCLONE:   /* ( clones a string ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        EvalStack[ESP-1] = (long)PRS_CloneString((StringT*)EvalStack[ESP-1]);
      break;

    case SDEL:  /* ( deletes a string ) */
      if(ESP < 1)
        fault = EVAL_STACK_UNDERFLOW_FAULT;
      else
        PRS_DeleteString((StringT*)EvalStack[--ESP]);
      break;

    case SLT:   /* ( string literal ) */
      if(ESP >= EVAL_STACK_SIZE)
        fault = EVAL_STACK_OVERFLOW_FAULT;
      else
        {
        EvalStack[ESP++] = (long)PRS_CreateString((char*)IP);
        while(*IP)  /* skip over string until terminating 0. */
          ++IP;
        ++IP;       /* skip over terminating 0 */
        }
      break;
      
    case ESC:   /* page 2 escape */
      fault = Page2();
      break;

    default:
      fault = UNIMPLEMENTED_INSTR_FAULT;
      break;
    }

  #if !defined NDEBUG && defined SHOW
  ShowStack();
  PRSI_printf("\n");
  #endif

  return fault;
}


/**********************************************************************
* Function:   PRS_StartVM
* This sets up the I-code VM to run the current segment. Repeated calls
* to PRS_RunVM will then run the code
* Parameters: NONE
*
* Returns:    0 if the code is runnable, non-zero if not.
**********************************************************************/
int PRS_StartVM(void)
{
  IP = PRSI_PlantCurrentSegment();

  /* Initialise registers (in case they were left at odd values
  // by a faulting program
  */
  FP = ReturnStack;
  RSP = 0;
  ESP = 0;
  
  #if !defined NDEBUG && defined SHOW
  CallLevel=0;
  #endif
  return IP == NULL;
}

/**********************************************************************
* Function:   PRS_RunVM
* This runs the code in the VM to completion (HALT) or another fault
* Parameters: NONE
*
* Returns:    terminating fault. Normally this should be HALT_FAULT.
*             NO_FAULT indicates the code could not run.
**********************************************************************/
int PRS_RunVM(void)
{
  int fault = NO_FAULT;
  if(!PRS_StartVM())
    do
      {
      fault = PRS_TickVM();
      }
    while(fault == NO_FAULT);
  return fault;
}

/**********************************************************************
* Function:   PRS_TickVM
* This processes a single VM opcode
* Parameters: NONE
*
* Returns:    Any fault caused by executing the opcode else NO_FAULT
**********************************************************************/
int PRS_TickVM(void)
{
  int fault = NO_FAULT;
  assert(IP != NULL);

  fault = ProcessOpcode();
  if((fault != NO_FAULT) && (fault != HALT_FAULT) && (FaultHandler != NULL))
    fault = (*FaultHandler)(fault);

  /* Stopped ? */
  if(fault != NO_FAULT)
    IP = NULL;

  return fault;
}

/**********************************************************************
* Function:   PRS_VMRunning
* This allows code to check whether the VM is running or not: new
* jobs should not be started when the VM is running.
* Parameters: NONE
*
* Returns:    TRUE if running, FALSE if not.
**********************************************************************/
int PRS_VMRunning(void)
{
  return IP != NULL;
}

/**********************************************************************
* Function:   PRS_StopVM
* This should be called if the system wants to stop the VM running
* before it reaches the end of execution
* Parameters: NONE
*
* Returns:    NOTHING
**********************************************************************/
void PRS_StopVM(void)
{
  IP = NULL;
  return;
}


/**********************************************************************
* PRS_SetFaultHandler
* This sets up a fault handler which will be called whenever the
* I-Code processor terminates abnomally.  This can be used by
* application code to shutdown any operation in the event of a fault.
* The fault handler is passed the fault type.
* return the fault type: if it alters the fault type to NO_FAULT the
* process will not terminate.  Use this with caution.
**********************************************************************/
PFII PRS_SetFaultHandler(PFII fn)
{
  PFII OldHandler = NULL;
  OldHandler = FaultHandler;
  FaultHandler = fn;
  return OldHandler;
}


/**********************************************************************
* PRS_Parameters
**********************************************************************/
void* PRS_Parameters(int size)
{
  void *ptr = NULL;

  if(size > 0 || size < RSP)
    {
    RSP -= size;  /* drop parameters from stack */
    ptr = (void*)(ReturnStack + RSP);
    }
  else
    PRS_Error(IDS_ERR_IPROC_INVALID_SIZE);
  return ptr;
}

/**********************************************************************
* PRS_ReturnReal
**********************************************************************/
int PRS_ReturnReal(float fVal)
{
  int err = FALSE;
  if(ESP >= EVAL_STACK_SIZE)
    err = TRUE;
  else
    {
    *(float*)(EvalStack+ESP) = fVal;
    ++ESP;
    }
  return err;
}

/**********************************************************************
* PRS_ReturnOrdinal
**********************************************************************/
int PRS_ReturnOrdinal(long lVal)
{
  int err = FALSE;

  if(ESP >= EVAL_STACK_SIZE)
    err = TRUE;
  else
    EvalStack[ESP++] = lVal;
  return err;
}

/**********************************************************************
* PRS_ReturnPtr
**********************************************************************/
int PRS_ReturnPtr(void* ptr)
{
  int err = FALSE;
  if(ESP >= EVAL_STACK_SIZE)
    err = TRUE;
  else
    {
    *(void**)(EvalStack+ESP) = ptr;
    ++ESP;
    }
  return err;
}


#if !defined NDEBUG && defined SHOW
/**********************************************************************
* IP_TranslateOpcode
* Translates the opcode to its text mnemonic
**********************************************************************/
const char* IP_TranslateOpcode(Opcode op)
{

  OpcodeName* here;
  op &= IP_OpcodeMask;

  for(here = OpcodeTranslationTable; here->name != NULL; ++here)
    {
    if(here->op == op)
      return here->name;
    }
  return "UNKNOWN";
}
#endif
