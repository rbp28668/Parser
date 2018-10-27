#include <assert.h>
#include "prsif.h"

void PRS_Version(int* lpiMajor, int* lpiMinor)
{
  assert(lpiMajor);
  assert(lpiMinor);
  *lpiMajor = 1;
  *lpiMinor = 2;
}
