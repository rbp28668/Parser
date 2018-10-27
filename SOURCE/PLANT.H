/* 
// File:          PLANT.H
// 
// Prefix:
// 
// Description:   Include file for opcode planting
//
*/

#ifndef _PLANT_H
#define _PLANT_H
/* PLANT.C */

#include "iproc.h"

extern void PRSI_Plant(Opcode op);
extern Opcode* PRSI_PlantBranch(Opcode op);
extern void PRSI_PlantSetBranch(Opcode* Target,Opcode* BrInst);
extern Opcode* PRSI_PlantPosition(void);

extern void PRSI_PlantAddr(void* addr);
extern void PRSI_PlantInteger(long lval);
extern void PRSI_PlantFloat(float fval);

extern void PRSI_PlantString(PSTRING pstr);

extern void PRSI_PlantNewSegment(void);
extern Opcode* PRSI_PlantGetSegment(void);
extern Opcode* PRSI_PlantCurrentSegment(void);
extern void PRSI_PlantSetRunnable(void);

extern int PRSI_SaveCode(void);
extern int PRSI_RestoreCode(void);

extern int PRSI_PlantInitialise(void);
extern void PRSI_PlantTerminate(void);

/* PLANTEXP.C */
#ifdef _EXPR_H
extern int PRSI_IsSimpleType(TypeT* type);
extern int PRSI_PlantAPList(ExprT* expr);
extern int PRSI_PlantExpression(ExprT* expr, int blValue);
#endif

#endif
