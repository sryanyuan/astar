#ifndef _INC_MEMLEAK_DETECT_FILELINE_
#define _INC_MEMLEAK_DETECT_FILELINE_

#ifdef _DEBUG
#define New new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new New
#endif

#endif