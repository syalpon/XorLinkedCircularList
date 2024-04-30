#ifndef _XLCL_H_
#define _XLCL_H_

    #ifdef __cplusplus
    extern "C" {
    #endif

    __declspec(dllimport) void *__cdecl xlcl_start   (void);
    __declspec(dllimport) void *__cdecl xlcl_add_node(void *vp,int additional_data);
    __declspec(dllimport) int   __cdecl xlcl_pop     (void *vp,unsigned int index,int *ret);
    __declspec(dllimport) int   __cdecl xlcl_get_head(void *vp,int *ret);
    __declspec(dllimport) int   __cdecl xlcl_get_tail(void *vp,int *ret);
    __declspec(dllimport) void  __cdecl xlcl_all_show(void *vp);
    __declspec(dllimport) void *__cdecl xlcl_end     (void *vp);
    
    #ifdef __cplusplus
    }
    #endif

#endif

