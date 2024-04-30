#include "xlcl.h"
#include <stdio.h>
int main(void)
{
    void *Root = xlcl_start();
    int buffer ;
    xlcl_all_show(Root);
    xlcl_add_node(Root,1);
    xlcl_add_node(Root,2);
    xlcl_add_node(Root,3);
    xlcl_add_node(Root,4);
    xlcl_add_node(Root,5);
    xlcl_all_show(Root);

    xlcl_pop(Root,3,&buffer);
    printf("getData[3] = %d\n\n",buffer);
    xlcl_all_show(Root);

    xlcl_get_tail(Root,&buffer);
    printf("getData[Tail] = %d\n\n",buffer);
    xlcl_all_show(Root);

    xlcl_get_head(Root,&buffer);
    printf("getData[Head] = %d\n\n",buffer);
    xlcl_all_show(Root);

    xlcl_get_tail(Root,&buffer);
    printf("getData[Tail] = %d\n\n",buffer);
    xlcl_all_show(Root);

    xlcl_get_tail(Root,&buffer);
    printf("getData[Tail] = %d\n\n",buffer);
    xlcl_all_show(Root);

    Root = xlcl_end(Root); 

    return 0;
}