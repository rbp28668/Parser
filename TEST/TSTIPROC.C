/*
// File:        TSTIPROC.C
// 
// Prefix:
// 
// Description: Test harness for IPROC
//
*/

#include <stdio.h>
#include "prsif.h"
#include "plant.h"
#include "iproc.h"
#include "pstring.h"
#include "printf.h"

/* Variables for testing load and store */
static long lStore;
static int  wStore;
static char cStore;

/* Block of memory for testing MOV */
static long alStore[4] = {72L, 73L, 74L, 75L};

/* String pointer for testing string referencing */
static StringT* pString = NULL;

/* Function to test CCALL */
static int testfn(void)
{
  PRSI_printf("testfn called\n");
  return 0;
}

/* Function to validate values have been stored correctly */
static int CheckStore(void)
{
  PRSI_printf("Stored values are (long)%ld, (word)%d, (char)%d\n",
    lStore,wStore,(int)cStore);
  return 0;
}

static void PRSI_PlantTestData(void)
{
  Opcode* poBRA;     /* Stores branch offset addr */
  Opcode* poSubAddr; /* Stores call target addr */

  PRSI_Plant(NOP);

  /* Test stack manipulation */
  PRSI_PlantInteger(55);     /* --> 55 */
  PRSI_Plant(DUP);           /* --> 55,55 */
  PRSI_Plant(DROP);          /* --> 55 */

  PRSI_PlantInteger(56);     /* --> 55,56 */
  PRSI_Plant(OVER);          /* --> 55,56,55 */

  PRSI_PlantInteger(2);      /* --> 55,56,55,2 */
  PRSI_Plant(PICK);          /* --> 55,56,55,56 */
  PRSI_Plant(SWAP);          /* --> 55,56,56,55 */
  PRSI_Plant(REPL);          /* --> 55,56,55 */
  PRSI_Plant(DROP);          /* --> 55,56 */
  PRSI_Plant(DROP);          /* --> 55 */
  PRSI_Plant(DROP);          /* -->  */

  PRSI_PlantInteger(6);
  PRSI_PlantInteger(7);
  PRSI_Plant(ADD);   /* --> 13 */

  PRSI_PlantInteger(5);
  PRSI_Plant(SUB);   /* --> 8 */

  PRSI_PlantInteger(3);
  PRSI_Plant(MULT);  /* --> 24 */

  PRSI_PlantInteger(7);
  PRSI_Plant(DIV);   /* --> 3 */

  PRSI_PlantInteger(2);
  PRSI_Plant(MOD);   /* --> 1 */

  PRSI_PlantInteger(5);
  PRSI_Plant(AND);   /* --> 1 */

  PRSI_PlantInteger(6);
  PRSI_Plant(OR);    /* --> 7 */

  PRSI_Plant(NOT);   /* --> -8 */
  PRSI_Plant(DROP);

  PRSI_PlantFloat(6.0);
  PRSI_PlantFloat(7.0);
  PRSI_Plant(FADD);  /* --> 13.0 */

  PRSI_PlantFloat(5.0);
  PRSI_Plant(FSUB);  /* --> 8.0 */

  PRSI_PlantFloat(3.0);
  PRSI_Plant(FMULT); /* --> 15.0 */

  PRSI_PlantFloat(7.0);
  PRSI_Plant(FDIV);  /*  --> 7.12 */
  PRSI_Plant(DROP);

  PRSI_PlantInteger(6);
  PRSI_Plant(ITOF);  /* --> 6.0 */

  PRSI_Plant(FTOI);  /* --> 6 */
  PRSI_Plant(DROP);

  PRSI_PlantInteger(6);
  PRSI_PlantInteger(6);
  PRSI_Plant(EQU);
  PRSI_Plant(DROP);

  PRSI_PlantInteger(6);
  PRSI_PlantInteger(7);
  PRSI_Plant(NEQU);
  PRSI_Plant(DROP);

  /* LT: a < b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(7);
  PRSI_Plant(LT);
  PRSI_Plant(DROP);

  /* LT: a = b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(6);
  PRSI_Plant(LT);
  PRSI_Plant(DROP);

  /* LT: a > b */
  PRSI_PlantInteger(7);
  PRSI_PlantInteger(6);
  PRSI_Plant(LT);
  PRSI_Plant(DROP);

  /* GT: a < b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(7);
  PRSI_Plant(GT);
  PRSI_Plant(DROP);

  /* GT: a = b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(6);
  PRSI_Plant(GT);
  PRSI_Plant(DROP);

  /* GT: a > b */  
  PRSI_PlantInteger(7);
  PRSI_PlantInteger(6);
  PRSI_Plant(GT);
  PRSI_Plant(DROP);

  /* LE: a < b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(7);
  PRSI_Plant(LE);
  PRSI_Plant(DROP);

  /* LE: a = b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(6);
  PRSI_Plant(LE);
  PRSI_Plant(DROP);

  /* LE: a > b */
  PRSI_PlantInteger(7);
  PRSI_PlantInteger(6);
  PRSI_Plant(LE);
  PRSI_Plant(DROP);

  /* GE: a < b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(7);
  PRSI_Plant(GE);
  PRSI_Plant(DROP);

  /* GE: a = b */
  PRSI_PlantInteger(6);
  PRSI_PlantInteger(6);
  PRSI_Plant(GE);
  PRSI_Plant(DROP);

  /* GE: a > b */
  PRSI_PlantInteger(7);
  PRSI_PlantInteger(6);
  PRSI_Plant(GE);
  PRSI_Plant(DROP);

  /* FLT: a < b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(7.0f);
  PRSI_Plant(FLT);
  PRSI_Plant(DROP);

  /* FLT: a = b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FLT);
  PRSI_Plant(DROP);

  /* FLT: a > b */
  PRSI_PlantFloat(7.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FLT);
  PRSI_Plant(DROP);

  /* FGT: a < b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(7.0f);
  PRSI_Plant(FGT);
  PRSI_Plant(DROP);

  /* FGT: a = b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FGT);
  PRSI_Plant(DROP);

  /* FGT: a > b */
  PRSI_PlantFloat(7.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FGT);
  PRSI_Plant(DROP);

  /* FLE: a < b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(7.0f);
  PRSI_Plant(FLE);
  PRSI_Plant(DROP);

  /* FLE: a = b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FLE);
  PRSI_Plant(DROP);

  /* FLE: a > b */
  PRSI_PlantFloat(7.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FLE);
  PRSI_Plant(DROP);

  /* FGE: a < b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(7.0f);
  PRSI_Plant(FGE);
  PRSI_Plant(DROP);

  /* FGE: a = b */
  PRSI_PlantFloat(6.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FGE);
  PRSI_Plant(DROP);

  /* FGE: a > b */
  PRSI_PlantFloat(7.0f);
  PRSI_PlantFloat(6.0f);
  PRSI_Plant(FGE);
  PRSI_Plant(DROP);

  PRSI_PlantInteger(2);
  PRSI_Plant(ZEQU);  /* --> 0 */
  PRSI_Plant(ZEQU);  /* --> 1 */
  PRSI_Plant(DROP);  /* --> */

  poBRA = PRSI_PlantBranch(BRA);
  PRSI_Plant(NOP);
  PRSI_Plant(NOP);
  PRSI_PlantSetBranch(PRSI_PlantPosition(),poBRA); /* branch to here */

  PRSI_PlantInteger(0);
  poBRA = PRSI_PlantBranch(ZBRA);
  PRSI_Plant(NOP);
  PRSI_Plant(NOP);
  PRSI_PlantSetBranch(PRSI_PlantPosition(),poBRA); /* branch to here */

  PRSI_PlantInteger(1);
  poBRA = PRSI_PlantBranch(ZBRA);
  PRSI_Plant(NOP);
  PRSI_Plant(NOP);
  PRSI_PlantSetBranch(PRSI_PlantPosition(),poBRA); /* branch to here */

  /* Set up a subroutine & branch past it. */
  /* This subroutine returns its argument*/
  poBRA = PRSI_PlantBranch(BRA);
  poSubAddr = PRSI_PlantPosition();
  PRSI_PlantInteger(8);  /* reserve 8 bytes for locals */
  PRSI_Plant(ENTER);

  PRSI_PlantInteger(13);
  PRSI_PlantInteger(0);
  PRSI_Plant(FPST);      /* Store LONG local 0 */

  PRSI_PlantInteger(15);
  PRSI_PlantInteger(4);
  PRSI_Plant(FPWST);      /* Store WORD local 1 */

  PRSI_PlantInteger(65);
  PRSI_PlantInteger(6);
  PRSI_Plant(FPCST);      /* Store BYTE local 2 */

  PRSI_PlantInteger(66); 
  PRSI_PlantInteger(7);
  PRSI_Plant(FPCST);      /* Store BYTE local 3 */

  PRSI_PlantInteger(0);
  PRSI_Plant(FPLD);       /* -->13 Fetch LONG local 0 */

  PRSI_PlantInteger(4);
  PRSI_Plant(FPWLD);      /* -->13,15 Fetch WORD local 1 */

  PRSI_PlantInteger(6);
  PRSI_Plant(FPCLD);      /* -->13,15,65 Fetch BYTE local 2 */

  PRSI_PlantInteger(7);
  PRSI_Plant(FPCLD);      /* -->13,15,65,66 Fetch BYTE local 3 */

  PRSI_Plant(ADD);        /* -->13,15,131 */
  PRSI_Plant(ADD);        /* -->13,146 */
  PRSI_Plant(ADD);        /* -->159 */

  PRSI_PlantInteger(-12); /* offset of parameter 0 */
  PRSI_Plant(FPLD);       /* -->159,42 */
  PRSI_Plant(ADD);        /* -->201 */

  PRSI_Plant(LEAVE);
  PRSI_PlantInteger(4);  /* bytes of parameters */
  PRSI_Plant(NRET);

  PRSI_PlantSetBranch(PRSI_PlantPosition(),poBRA); /* branch to here */

  PRSI_Plant(NOP);          /* target of branch to skip subroutine */

  /* Test TOR and FROMR before TOR is used to plant subroutine arg */
  PRSI_PlantInteger(11);
  PRSI_PlantInteger(22);
  PRSI_PlantInteger(33);
  PRSI_PlantInteger(44);
  PRSI_Plant(NOP);
  PRSI_Plant(TOR);           /* --> */
  PRSI_Plant(FROMR);         /* --> 33 */
  PRSI_Plant(DROP);
  PRSI_Plant(DROP);
  PRSI_Plant(DROP);
  PRSI_Plant(DROP);

  /* Call the subroutine */
  PRSI_PlantInteger(42);
  PRSI_Plant(TOR);
  PRSI_PlantAddr(poSubAddr);
  PRSI_Plant(CALL);
  PRSI_Plant(DROP);

  /* Call the 'C' test subroutine */
  PRSI_PlantAddr(testfn);
  PRSI_Plant(CCALL);


  /* Test load and store for long, word and char */
  PRSI_PlantInteger(55);
  PRSI_PlantInteger(56);
  PRSI_PlantInteger(57);     /* --> 55,56,57 */

  PRSI_PlantAddr(&lStore);
  PRSI_Plant(ST);            /* --> 55,56 */

  PRSI_PlantAddr(&wStore);
  PRSI_Plant(WST);           /* --> 55 */

  PRSI_PlantAddr(&cStore);
  PRSI_Plant(CST);           /* --> */

  PRSI_PlantAddr(CheckStore);
  PRSI_Plant(CCALL);

  PRSI_PlantAddr(&lStore);
  PRSI_Plant(LD);            /* --> 57 */
  
  PRSI_PlantAddr(&wStore);   
  PRSI_Plant(WLD);           /* --> 57,56 */
  
  PRSI_PlantAddr(&cStore);
  PRSI_Plant(CLD);           /* --> 57,56,55 */
  PRSI_Plant(DROP);
  PRSI_Plant(DROP);
  PRSI_Plant(DROP);          /* --> */

  PRSI_PlantInteger(0);
  PRSI_Plant(FPADD);       
  PRSI_PlantInteger(4);
  PRSI_Plant(FPADD);
  PRSI_Plant(DROP);
  PRSI_Plant(DROP);

  PRSI_PlantInteger(4);
  PRSI_PlantInteger(0);
  PRSI_PlantInteger(10);     /* --> 4,0,10 */
  PRSI_Plant(BND);           /* --> 4 */
  PRSI_Plant(DROP);

  PRSI_PlantInteger(0);
  PRSI_PlantInteger(0);
  PRSI_PlantInteger(10);     /* --> 0,0,10 */
  PRSI_Plant(BND);           /* --> 0 */
  PRSI_Plant(DROP);

  PRSI_PlantInteger(10);
  PRSI_PlantInteger(0);
  PRSI_PlantInteger(10);     /* --> 10,0,10 */
  PRSI_Plant(BND);           /* --> 10 */
  PRSI_Plant(DROP);

  PRSI_PlantInteger(16);
  PRSI_Plant(ALLOC);         /* -->addr */
  PRSI_PlantAddr(alStore);   /* -->addr,alStore */
  PRSI_Plant(SWAP);          /* -->alStore,addr */
  PRSI_PlantInteger(16);     /* -->alStore,addr,16 */
  PRSI_Plant(MOV);           /* --> Return stack should now have 4 longs*/
  PRSI_Plant(FROMR);         /* --> 75 */
  PRSI_Plant(DROP);
  PRSI_Plant(FROMR);         /* --> 74 */
  PRSI_Plant(DROP);
  PRSI_Plant(FROMR);         /* --> 73 */
  PRSI_Plant(DROP);
  PRSI_Plant(FROMR);         /* --> 72 */
  PRSI_Plant(DROP);

  PRSI_PlantAddr(pString);
  PRSI_Plant(SREF);
  PRSI_Plant(SDREF);

  PRSI_Plant(HALT);

}


static void PRSI_PlantFault(void)
{
  PRSI_PlantInteger(77);
  PRSI_Plant(FAULT);
  PRSI_Plant(HALT);
}

static void PRSI_PlantBNDFailLower(void)
{
  PRSI_PlantInteger(9);
  PRSI_PlantInteger(10);
  PRSI_PlantInteger(20);
  PRSI_Plant(BND);
  PRSI_Plant(HALT);
}

static void PRSI_PlantBNDFailUpper(void)
{
  PRSI_PlantInteger(21);
  PRSI_PlantInteger(10);
  PRSI_PlantInteger(20);
  PRSI_Plant(BND);
  PRSI_Plant(HALT);
}

int main(void)
{
  PRS_InitialiseInterpreter();

  pString = PRS_CreateString("Hello World");

  PRSI_PlantNewSegment();
  PRSI_PlantTestData();
  PRS_RunVM();

  PRSI_PlantNewSegment();
  PRSI_PlantFault();
  PRS_RunVM();

  PRSI_PlantNewSegment();
  PRSI_PlantBNDFailLower();
  PRS_RunVM();

  PRSI_PlantNewSegment();
  PRSI_PlantBNDFailUpper();
  PRS_RunVM();

  PRS_TerminateInterpreter();
  return 0;
}
