#include "xlcl.h"

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