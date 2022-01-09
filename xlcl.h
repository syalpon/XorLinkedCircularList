#ifndef _XLCL_H_
#define _XLCL_H_

__declspec(dllimport) void *__cdecl xlcl_start(void);
__declspec(dllimport) void *__cdecl xlcl_add_node(void *vp,int additional_data);
__declspec(dllimport) int   __cdecl xlcl_pop(void *vp);
__declspec(dllimport) void  __cdecl xlcl_all_show(void *vp);
__declspec(dllimport) void *__cdecl xlcl_end(void *vp);

#endif

