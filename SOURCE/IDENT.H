/* 
IDENT.H

Prototypes for IDENT.C

*/

#ifndef _IDENT_H

/* Constants to determine whether a identifier is linked into
*  the main list or not.
*/
#define LINK 1
#define NOLINK 0


/* MarkT structure which is used to record the state of the identifier
// list an memory allocation before the system starts to process
// a compilable unit. If the processing fails then the system can
// roll-back the processing to where it was before.
*/ 
typedef struct s_mark
{
  IdentifierT **tail;
  void *MemMark;
}MarkT;

extern TypeT IntegerTypeDecl;
extern TypeT BooleanTypeDecl;
extern TypeT RealTypeDecl;
extern TypeT StringTypeDecl;
extern TypeT ElementTypeDecl;

IdentifierT *PRSI_FindRecordField(IdentifierT *parent, const char *name);

extern IdentifierT *PRSI_NewIdentifier(const char *name,int link);
extern TypeT *PRSI_NewType(void);

extern IdentifierT *PRSI_FindIdentifier(const char *name);
extern int PRSI_CheckDuplicateIdentifier(const char *name);

extern void PRSI_StartLocalScope(IdentifierT *FnOrProc);
extern void PRSI_EndLocalScope(void);
extern int PRSI_InLocalScope(void);
extern IdentifierT* PRSI_GetLocalList(void);

extern void PRSI_InitialiseIdentifiers(void);
extern void PRSI_TerminateIdentifiers(void);

extern void *PRSI_AllocateGlobalStore(unsigned int size);

extern void PRSI_StartTransaction(MarkT *mark);
extern void PRSI_RollBackTransaction(const MarkT *mark);

#define _IDENT_H 1
#endif

