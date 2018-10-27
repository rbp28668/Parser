
#ifndef TOKEN_H
#include "lex.h"

/* All the (non predefined) tokens for the parser. Keywords
*  have a TK prefix (Token Keyword). Punctuation has a TP
*  prefix (Token Punctuation)
*/

enum e_Keywords
	{
  TKDiv = TokFirstUser,	
	TKMod,
  TKNil,
  TKIn,
  TKOr,
  TKAnd,
  TKNot,
	TKIf,
	TKThen,
	TKElse,
	TKCase,
	TKOf,
	TKRepeat,
	TKUntil,
	TKWhile,
	TKDo,
	TKFor,
	TKTo,
	TKDownto,
	TKBegin,
	TKEnd,
  TKWith,
  TKGoto,
  TKConst ,
	TKVar,
	TKType,
	TKArray,
	TKRecord,
	TKSet,
  TKFile,
	TKFunction,
	TKProcedure,
  TKLabel,
  TKPacked,
  TKProgram,

	TPPlus,
	TPMinus,
	TPTimes,
	TPDivide,
	TPEquals,
	TPNotEquals,
	TPLessThan,
	TPGreaterThan,
	TPLessEquals,
	TPGreaterEquals,
	TPLeftParen,
	TPRightParen,
	TPLeftBracket,
	TPRightBracket,
	TPAssignment,
	TPPeriod,
	TPComma,
	TPColon,
	TPSemicolon,
  TPDotDot,
  TPPointer,

	TPQuote,

  TSNegate,    /* special for unary minus */
  TSReturn,    /* special for function return */

  TS_LastToken
	};

#define TOKEN_H
#endif

