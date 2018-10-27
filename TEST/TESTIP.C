/* 
// File:        TESTIP.C
// 
// Prefix:
// 
// Description: Test harness for IPROC
//
*/

#include <stdio.h>
#include "plant.h"
#include "iproc.h"

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
  printf("testfn called\n");
  return 0;
}

/* Function to validate values have been stored correctly */
static int CheckStore(void)
{
  printf("Stored values are (long)%ld, (word)%d, (char)%d\n",
    lStore,wStore,(int)cStore);
  return 0;
}

static void PlantTestData(void)
{

  Plant(NOP);

  PlantInteger(6);
  PlantInteger(7);
  Plant(ADD);   /* --> 13 */

  PlantInteger(5);
  Plant(SUB);   /* --> 8 */

  PlantInteger(3);
  Plant(MULT);  /* --> 24 */

  PlantInteger(7);
  Plant(DIV);   /* --> 3 */

  PlantInteger(2);
  Plant(MOD);   /* --> 1 */

  PlantInteger(5);
  Plant(AND);   /* --> 1 */

  PlantInteger(6);
  Plant(OR);    /* --> 7 */

  Plant(NOT);   /* --> -8 */
  Plant(DROP);

  PlantFloat(6.0);
  PlantFloat(7.0);
  Plant(FADD);  /* --> 13.0 */

  PlantFloat(5.0);
  Plant(FSUB);  /* --> 8.0 */

  PlantFloat(3.0);
  Plant(FMULT); /* --> 15.0 */

  PlantFloat(7.0);
  Plant(FDIV);  /*  --> 7.12 */
  Plant(DROP);

  PlantInteger(6);
  Plant(ITOF);  /* --> 6.0 */

  Plant(FTOI);  /* --> 6 */
  Plant(DROP);

  PlantInteger(6);
  PlantInteger(6);
  Plant(EQU);
  Plant(DROP);

  PlantInteger(6);
  PlantInteger(7);
  Plant(NEQU);
  Plant(DROP);

  /* LT: a < b */
  PlantInteger(6);
  PlantInteger(7);
  Plant(LT);
  Plant(DROP);

  /* LT: a = b */
  PlantInteger(6);
  PlantInteger(6);
  Plant(LT);
  Plant(DROP);

  /* LT: a > b */
  PlantInteger(7);
  PlantInteger(6);
  Plant(LT);
  Plant(DROP);

  /* GT: a < b */  
  PlantInteger(6);
  PlantInteger(7);
  Plant(GT);
  Plant(DROP);

  /* GT: a = b */  
  PlantInteger(6);
  PlantInteger(6);
  Plant(GT);
  Plant(DROP);

  /* GT: a > b */  
  PlantInteger(7);
  PlantInteger(6);
  Plant(GT);
  Plant(DROP);

  /* LE: a < b */
  PlantInteger(6);
  PlantInteger(7);
  Plant(LE);
  Plant(DROP);

  /* LE: a = b */
  PlantInteger(6);
  PlantInteger(6);
  Plant(LE);
  Plant(DROP);

  /* LE: a > b */
  PlantInteger(7);
  PlantInteger(6);
  Plant(LE);
  Plant(DROP);

  /* GE: a < b */
  PlantInteger(6);
  PlantInteger(7);
  Plant(GE);
  Plant(DROP);

  /* GE: a = b */
  PlantInteger(6);
  PlantInteger(6);
  Plant(GE);
  Plant(DROP);

  /* GE: a > b */
  PlantInteger(7);
  PlantInteger(6);
  Plant(GE);
  Plant(DROP);
  
  /* FLT: a < b */
  PlantFloat(6.0f);
  PlantFloat(7.0f);
  Plant(FLT);
  Plant(DROP);

  /* FLT: a = b */
  PlantFloat(6.0f);
  PlantFloat(6.0f);
  Plant(FLT);
  Plant(DROP);

  /* FLT: a > b */
  PlantFloat(7.0f);
  PlantFloat(6.0f);
  Plant(FLT);
  Plant(DROP);

  /* FGT: a < b */
  PlantFloat(6.0f);
  PlantFloat(7.0f);
  Plant(FGT);
  Plant(DROP);
  
  /* FGT: a = b */
  PlantFloat(6.0f);
  PlantFloat(6.0f);
  Plant(FGT);
  Plant(DROP);

  /* FGT: a > b */
  PlantFloat(7.0f);
  PlantFloat(6.0f);
  Plant(FGT);
  Plant(DROP);

  /* FLE: a < b */
  PlantFloat(6.0f);
  PlantFloat(7.0f);
  Plant(FLE);
  Plant(DROP);

  /* FLE: a = b */
  PlantFloat(6.0f);
  PlantFloat(6.0f);
  Plant(FLE);
  Plant(DROP);

  /* FLE: a > b */
  PlantFloat(7.0f);
  PlantFloat(6.0f);
  Plant(FLE);
  Plant(DROP);

  /* FGE: a < b */
  PlantFloat(6.0f);
  PlantFloat(7.0f);
  Plant(FGE);
  Plant(DROP);

  /* FGE: a = b */
  PlantFloat(6.0f);
  PlantFloat(6.0f);
  Plant(FGE);
  Plant(DROP);

  /* FGE: a > b */
  PlantFloat(7.0f);
  PlantFloat(6.0f);
  Plant(FGE);
  Plant(DROP);

  PlantInteger(2);
  Plant(ZEQU);  /* --> 0 */
  Plant(ZEQU);  /* --> 1 */

  poBRA = PlantBranch(BRA);
  Plant(NOP);
  Plant(NOP);
  PlantSetBranch(PlantPosition(),poBRA); /* branch to here */

  PlantInteger(0);
  poBRA = PlantBranch(ZBRA);
  Plant(NOP);
  Plant(NOP);
  PlantSetBranch(PlantPosition(),poBRA); /* branch to here */

  PlantInteger(0);
  poBRA = PlantBranch(ZBRA);
  Plant(NOP);
  Plant(NOP);
  PlantSetBranch(PlantPosition(),poBRA); /* branch to here */

  /* Set up a subroutine & branch past it. */
  /* This subroutine returns its argument*/
  poBRA = PlantBranch(BRA);
  poSubAddr = PlantPosition();
  PlantInteger(8);  /* reserve 8 bytes for locals */
  Plant(ENTER);

  PlantInteger(13);
  PlantInteger(0);
  Plant(FPST);      /* Store LONG local 0 */

  PlantInteger(15);
  PlantInteger(4);
  Plant(FPWST);      /* Store WORD local 1 */

  PlantInteger(65);
  PlantInteger(6);
  Plant(FPCST);      /* Store BYTE local 2 */

  PlantInteger(66); 
  PlantInteger(7);
  Plant(FPCST);      /* Store BYTE local 3 */


  PlantInteger(0);
  Plant(FPLD);       /* -->13 Fetch LONG local 0 */

  PlantInteger(4);
  Plant(FPWLD);      /* -->13,15 Fetch WORD local 1 */

  PlantInteger(6);
  Plant(FPCLD);      /* -->13,15,65 Fetch BYTE local 2 */

  PlantInteger(7);
  Plant(FPCLD);      /* -->13,15,65,66 Fetch BYTE local 3 */

  Plant(ADD);        /* -->13,15,131 */
  Plant(ADD);        /* -->13,146 */
  Plant(ADD);        /* -->159 */

  PlantInteger(-12); /* offset of parameter 0 */
  Plant(FLD);        /* -->159,42 */
  Plant(ADD);        /* -->201 */

  Plant(LEAVE);
  PlantInteger(4);  /* bytes of parameters */
  Plant(NRET);

  PlantSetBranch(PlantPosition(),poBRA); /* branch to here */

  PlantInteger(42);
  Plant(TOR);
  PlantAddr(poSubAddr);
  Plant(CALL);
  Plant(DROP);

  PlantAddr(testfn);
  Plant(CCALL);

  PlantInteger(55);     /* --> 55 */
  Plant(DUP);           /* --> 55,55 */
  Plant(DROP);          /* --> 55 */

  PlantInteger(56);     /* --> 55,56 */
  Plant(OVER);          /* --> 55,56,55 */

  PlantInteger(2);      /* --> 55,56,55,2 */
  Plant(PICK);          /* --> 55,56,55,56 */
  Plant(SWAP);          /* --> 55,56,56,55 */
  Plant(REPL);          /* --> 55,56,55 */
  PlantInteger(2);
  Plant(ADD);           /* --> 55,56,57 */

  PlantAddr(&lStore);
  Plant(ST);            /* --> 55,56 */

  PlantAddr(&wStore);
  Plant(WST);           /* --> 55 */

  PlantAddr(&cStore);
  Plant(CST);           /* --> */

  PlantAddr(CheckStore);
  Plant(CCALL);

  PlantAddr(&lStore);
  Plant(LD);            /* --> 57 */
  
  PlantAddr(&wStore);   
  Plant(WLD);           /* --> 57,56 */
  
  PlantAddr(&cStore);
  Plant(CLD);           /* --> 57,56,55 */

  PlantInteger(33);
  Plant(TOR);           /* --> */
  Plant(FROMR);         /* --> 33 */
  Plant(DROP);

  PlantInteger(0);
  Plant(FPADD);       
  PlantInteger(4);
  Plant(FPADD);
  Plant(DROP);
  Plant(DROP);

  PlantInteger(4);
  PlantInteger(0);
  PlantInteger(10);     /* --> 4,0,10 */
  Plant(BND);           /* --> 4 */
  Plant(DROP);

  PlantInteger(0);
  PlantInteger(0);
  PlantInteger(10);     /* --> 0,0,10 */
  Plant(BND);           /* --> 0 */
  Plant(DROP);

  PlantInteger(10);
  PlantInteger(0);
  PlantInteger(10);     /* --> 10,0,10 */
  Plant(BND);           /* --> 10 */
  Plant(DROP);

  PlantInteger(16);
  Plant(ALLOC);         /* -->addr */
  PlantAddr(alStore);   /* -->addr,alStore */
  Plant(SWAP);          /* -->alStore,addr */
  PlantInteger(16);     /* -->alStore,addr,16 */
  Plant(MOV);           /* --> Return stack should now have 4 longs*/
  Plant(FROMR)          /* --> 75 */
  Plant(DROP);
  Plant(FROMR)          /* --> 74 */
  Plant(DROP);
  Plant(FROMR)          /* --> 73 */
  Plant(DROP);
  Plant(FROMR)          /* --> 72 */
  Plant(DROP);

  PlantAddr(pString);
  Plant(SREF);
  Plant(SDREF);

  Plant(HALT);

}


static void PlantFault(void)
{
  PlantInteger(77);
  Plant(FAULT);
  Plant(HALT);
}

static void PlantBNDFailLower(void)
{
  PlantInteger(9);
  PlantInteger(10);
  PlantInteger(20);
  Plant(BND);
  Plant(HALT);
}

static void PlantBNDFailUpper(void)
{
  PlantInteger(21);
  PlantInteger(10);
  PlantInteger(20);
  Plant(BND);
  Plant(HALT);
}

int main(void)
{
  PlantInitialise();

  pString = PRS_CreateString("Hello World");

  PlantNewSegment();
  PlantTestData();
  PlantRunSegment();

  PlantNewSegment();
  PlantFault();
  PlantRunSegment();

  PlantNewSegment();
  PlantBNDFailLower();
  PlantRunSegment();

  PlantNewSegment();
  PlantBNDFailUpper();
  PlantRunSegment();

  PlantTerminate();
  return 0;
}
