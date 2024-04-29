#include <stdio.h>
#include <stdlib.h>

/*自己参照構造体:XorLinkedCircularList*/
typedef struct XorLinkedCircularList_T
{
    int data;
    struct XorLinkedCircularList_T *prev_xor_next;
}XorLinkedCircularList;

/*XorLinkedCircularListの一番初めのアドレスを格納する構造体*/
typedef struct XorLinkedCircularList_Root_T
{
    XorLinkedCircularList *Head;
    XorLinkedCircularList *Tail;
    size_t AllocationSize;
}XorLinkedCircularList_Root;


/*----------------------------------*/
/* 引数二つのポインタをXORして返す関数 */
/*----------------------------------*/
static void *AddrXor(void *p,void *q)
{
    return (void *)(((unsigned int)p) ^ ((unsigned int)q));
}


/*----------------------------------*/
/*XorLinkedCircularListの使用開始関数*/
/*----------------------------------*/
__declspec(dllexport) void *__cdecl xlcl_start(void)
{
    XorLinkedCircularList_Root *xlclr_addr = malloc(sizeof(XorLinkedCircularList_Root));
    xlclr_addr->AllocationSize = sizeof(XorLinkedCircularList_Root);
    if( xlclr_addr != NULL )
    {
        xlclr_addr->Head = NULL;
        xlclr_addr->Tail = NULL;
    }
    return xlclr_addr;
}



/*----------------------------------*/
/*XLCLにノードを追加する関数          */
/*----------------------------------*/
__declspec(dllexport) void *__cdecl xlcl_add_node(void *vp,int additional_data)
{
    XorLinkedCircularList *obtained_addr = NULL;
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;

    if( Root_addr != NULL )
    {
        obtained_addr = malloc(sizeof(XorLinkedCircularList));
        Root_addr->AllocationSize += sizeof(XorLinkedCircularList);
        if(obtained_addr != NULL)
        {
            /*確保した領域にデータを格納*/
            obtained_addr->data = additional_data;

            if( Root_addr->Head == NULL ) /*初回のノード追加の際*/
            {
                /*rootの先頭アドレスを更新*/
                Root_addr->Head = obtained_addr;

                /*初回のノードは自分自身のアドレスのXorを行う為0*/
                obtained_addr->prev_xor_next = NULL;
            }
            else
            {
                /*1つだけノードが存在している時*/
                if(Root_addr->Head == Root_addr->Tail)
                {
                    obtained_addr->prev_xor_next = NULL;
                }
                /*2つ以上のノードが存在している時*/
                else
                {
                    /*2つだけノードが存在している時*/
                    if(Root_addr->Head->prev_xor_next == NULL)
                    {
                        Root_addr->Head->prev_xor_next = AddrXor(obtained_addr,Root_addr->Tail);
                        Root_addr->Tail->prev_xor_next = AddrXor(Root_addr->Head,obtained_addr);
                        obtained_addr->prev_xor_next   = AddrXor(Root_addr->Head,Root_addr->Tail);
                    }
                    /*3つ以上ノードが存在しているとき*/
                    else
                    {
                        void *head_next = AddrXor(Root_addr->Head->prev_xor_next,Root_addr->Tail);
                        void *tail_prev = AddrXor(Root_addr->Head,Root_addr->Tail->prev_xor_next);
                        Root_addr->Head->prev_xor_next = AddrXor(obtained_addr,head_next);
                        Root_addr->Tail->prev_xor_next = AddrXor(tail_prev,obtained_addr);
                        obtained_addr->prev_xor_next   = AddrXor(Root_addr->Head,Root_addr->Tail);
                    }
                }
            }

            /*Roodに保存する末尾アドレスを更新*/
            Root_addr->Tail = obtained_addr;
        }
    }
    return obtained_addr;
}



/*----------------------------------*/
/*XLCLから先頭のデータを取り出す関数   */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_pop(void *vp)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    int ret = 0;
    if(Root_addr != NULL)
    {
        if(Root_addr->Tail != NULL) /*1つ以上のノードがある場合*/
        {
            ret = Root_addr->Tail->data;

            if(Root_addr->Tail->prev_xor_next != NULL) /*3つ以上のノードがある場合*/
            {
                XorLinkedCircularList *head_next = AddrXor(Root_addr->Head->prev_xor_next,Root_addr->Tail);
                XorLinkedCircularList *tail_prev = AddrXor(Root_addr->Tail->prev_xor_next,Root_addr->Head);
                XorLinkedCircularList *tail_prev_prev = AddrXor(tail_prev->prev_xor_next,Root_addr->Tail);
                XorLinkedCircularList *free_addr = Root_addr->Tail;

                /*最後のノードのメモリ解放*/
                Root_addr->Tail = tail_prev;
                Root_addr->Tail->prev_xor_next = AddrXor(tail_prev_prev,Root_addr->Head);
                Root_addr->Head->prev_xor_next = AddrXor(Root_addr->Tail,head_next);
                free(free_addr);
                Root_addr->AllocationSize -= sizeof(XorLinkedCircularList);
            }
            else
            {
                if( Root_addr->Head != Root_addr->Tail ) /*2つのノードの場合*/
                {
                    /*最後のノードのメモリ解放*/
                    free(Root_addr->Tail);
                    Root_addr->AllocationSize -= sizeof(XorLinkedCircularList);
                    Root_addr->Tail = Root_addr->Head;
                }
                else /*1つのノードの場合*/
                {
                    /*最後のノードのメモリ解放*/
                    free(Root_addr->Tail);
                    Root_addr->AllocationSize -= sizeof(XorLinkedCircularList);
                    Root_addr->Tail = Root_addr->Head = NULL;
                }
            }
        }
    }
    return ret;
}



/*----------------------------------*/
/*XLCLのデータを標示する関数          */
/*----------------------------------*/
__declspec(dllexport) void __cdecl xlcl_all_show(void *vp)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    if(Root_addr!=NULL)
    {
        printf("AllocationSize:%d\n",Root_addr->AllocationSize);
        if(Root_addr->Head!=NULL)
        {
            XorLinkedCircularList *index = Root_addr->Head;
            XorLinkedCircularList *index_prev = Root_addr->Tail;

            printf("%p:%d\n",index,index->data);
            while(index != Root_addr->Tail)
            {
                XorLinkedCircularList *temp = index;
                index = AddrXor(index_prev,index->prev_xor_next);
                index_prev = temp;
                printf("%p:%d\n",index,index->data);
            }
        }
    }
}



/*----------------------------------*/
/*XLCLの使用を終了する関数            */
/*----------------------------------*/
__declspec(dllexport) void *__cdecl xlcl_end(void *vp)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    if(Root_addr!=NULL)
    {
        while(Root_addr->Head!=NULL) xlcl_pop(Root_addr);
        free(Root_addr);
    }
    return NULL;
}