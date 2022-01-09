#include <stdio.h>

__declspec(dllimport) void *__cdecl xlcl_start(void);
__declspec(dllimport) void *__cdecl xlcl_add_node(void *vp,int additional_data);
__declspec(dllimport) int   __cdecl xlcl_pop(void *vp);
__declspec(dllimport) void  __cdecl xlcl_all_show(void *vp);
__declspec(dllimport) void *__cdecl xlcl_end(void *vp);

int main(void)
{
    void *Root = xlcl_start();

    xlcl_add_node(Root,1);
    xlcl_add_node(Root,2);
    xlcl_add_node(Root,3);
    xlcl_add_node(Root,4);
    xlcl_all_show(Root);
    xlcl_pop(Root);
    xlcl_all_show(Root);
    xlcl_pop(Root);
    xlcl_all_show(Root);
    xlcl_pop(Root);
    xlcl_all_show(Root);
    xlcl_pop(Root);
    xlcl_all_show(Root);
    Root = xlcl_end(Root); 

    return 0;
}