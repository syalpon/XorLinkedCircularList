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
    unsigned int datanum;
}XorLinkedCircularList_Root;



/*----------------------------------*/
/* 引数二つのポインタをXORして返す関数 */
/*----------------------------------*/
static void *AddrXor(void *p,void *q)
{
    return (void *)(((unsigned int)p) ^ ((unsigned int)q));
}



/*----------------------------------*/
/* 指定ノードアドレスを返す関数        */
/*----------------------------------*/
static void *GetAddrXLCL(XorLinkedCircularList_Root *root,unsigned int index)
{
    XorLinkedCircularList *addr1 = root->Tail;
    XorLinkedCircularList *addr2 = root->Head;
    XorLinkedCircularList *xlxladdr;

    /*index番目のデータを探索*/
    if(root->datanum > index)
    {
        while( index-- )
        {
            xlxladdr = AddrXor(addr1,addr2->prev_xor_next);
            addr1 = addr2;
            addr2 = xlxladdr;
        }
    }
    else
    {
        addr2 = NULL;
    }
    return addr2;
}



/*----------------------------------*/
/*XorLinkedCircularListの使用開始関数*/
/*----------------------------------*/
__declspec(dllexport) void *__cdecl xlcl_start(void)
{
    XorLinkedCircularList_Root *xlclr_addr = malloc(sizeof(XorLinkedCircularList_Root));
    xlclr_addr->AllocationSize = sizeof(XorLinkedCircularList_Root);
    xlclr_addr->datanum = 0; 

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
        (Root_addr->datanum)++;

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
/*XLCLからデータを取り出す関数        */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_pop(void *vp,unsigned int index,int *retdata)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    int retvalue = 0;

    if(Root_addr != NULL) /*NULLチェック*/
    {
        if( Root_addr->datanum != 0 )  /*1つ以上のノードがある場合*/
        {
            XorLinkedCircularList *index_node_addr;
            XorLinkedCircularList *index_node_nextaddr;
            XorLinkedCircularList *index_node_prevaddr;

            /* index番目のデータを探索 */
            index_node_addr = GetAddrXLCL(Root_addr,index);
            if( index_node_addr != NULL )
            {
                if( retdata != NULL )
                {
                    *retdata = index_node_addr->data;
                }
                else
                {
                    /*データは取得できるが、返値用バッファがない場合*/
                    /* Do Nothing */
                }
                retvalue = 1;

                /* 削除処理 */
                if( Root_addr->datanum == 1 ) /*ノードが1つのとき*/
                {
                    Root_addr->Head = NULL;
                    Root_addr->Tail = NULL;
                }
                else /*ノードが2つ以上のとき*/
                {
                    /*更新用ポインタ：同時性が必要なため*/
                    void *Root_Addr_Head_temp = Root_addr->Head;
                    void *Root_Addr_Tail_temp = Root_addr->Tail;

                    /*削除するノードの前のノードアドレスを取得*/
                    if( index_node_addr == Root_addr->Head ) /*指定ノードがHEAD*/
                    {
                        index_node_prevaddr = Root_addr->Tail;
                        Root_Addr_Head_temp = AddrXor(Root_addr->Head->prev_xor_next,index_node_prevaddr);
                    }
                    else
                    {
                        index_node_prevaddr = GetAddrXLCL(Root_addr,index-1);
                    }

                    /*削除するノードの次のノードアドレスを取得*/
                    if( index_node_addr == Root_addr->Tail ) /*指定ノードがTAIL*/
                    {
                        index_node_nextaddr = Root_addr->Head;
                        Root_Addr_Tail_temp = AddrXor(Root_addr->Tail->prev_xor_next,index_node_nextaddr);
                    }
                    else
                    {
                        index_node_nextaddr = GetAddrXLCL(Root_addr,index+1);
                    }
                    
                    /*更新*/
                    Root_addr->Head = Root_Addr_Head_temp;
                    Root_addr->Tail = Root_Addr_Tail_temp;

                    /*XorAddrの算出*/
                    if( Root_addr->datanum >= 3 ) /*ノードが3つ以上のとき*/
                    {
                        /*更新用ポインタ：同時性が必要なため*/
                        void *N_i_XorAddr;
                        void *N_j_XorAddr;
                        
                        /* ①N_{i+1}->XorAddr = N_{i+2} ^ N_{i-1} */ 
                        /* ②N_{i+2} = N_{i+1}->XorAddr ^ N_{i}   */
                        /*  N_{i+1}->XorAddr = N_{i+1}->XorAddr ^ N_{i} ^ N_{i-1} */
                        N_i_XorAddr = AddrXor(AddrXor(index_node_nextaddr->prev_xor_next,index_node_addr),index_node_prevaddr);
                        /* ①N_{i-1}->XorAddr = N_{i-2} ^ N_{i+1} */ 
                        /* ②N_{i-2} = N_{i-1}->XorAddr ^ N_{i}   */
                        /*  N_{i-1}->XorAddr = N_{i-1}->XorAddr ^ N_{i} ^ N_{i+1} */
                        N_j_XorAddr = AddrXor(AddrXor(index_node_prevaddr->prev_xor_next,index_node_addr),index_node_nextaddr);

                        /*更新*/
                        index_node_nextaddr->prev_xor_next = N_i_XorAddr;
                        index_node_prevaddr->prev_xor_next = N_j_XorAddr;
                    }
                }

                free(index_node_addr);
                (Root_addr->datanum)--;
                Root_addr->AllocationSize -= sizeof(XorLinkedCircularList);
            }
            else /* Node is Nothing */
            {
                retvalue = 0;
            }
        }
        else /* Node is Nothing */
        {   
            retdata = NULL;
            retvalue = 0;
        }
    }
    else /* NULL Access */
    {  
        retvalue = -1;
    }

    return retvalue; 
}



/*----------------------------------*/
/*XLCLの先頭からデータを取り出す関数   */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_get_head(void *vp,int *ret)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    return xlcl_pop(Root_addr,0,ret);
}



/*----------------------------------*/
/*XLCLの末尾からデータを取り出す関数   */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_get_tail(void *vp,int *ret)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    return xlcl_pop(Root_addr,Root_addr->datanum-1,ret);
}



/*----------------------------------*/
/*XLCLのデータを標示する関数          */
/*----------------------------------*/
__declspec(dllexport) void __cdecl xlcl_all_show(void *vp)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    if(Root_addr!=NULL)
    {
        printf("--------infomation--------\n");
        printf("AllocationSize:%d\ndatanum:%u\n",Root_addr->AllocationSize,Root_addr->datanum);
        printf("HEAD:%p\nTAIL:%p\n",Root_addr->Head,Root_addr->Tail);
        if(Root_addr->Head!=NULL)
        {
            XorLinkedCircularList *index = Root_addr->Head;
            XorLinkedCircularList *index_prev = Root_addr->Tail;
            printf(" [Address]:[ Data ]:[XorAddress] \n");
            printf("0x%p:%7d :0x%p\n",index,index->data,index->prev_xor_next);

            while(index != Root_addr->Tail)
            {
                XorLinkedCircularList *temp = index;
                index = AddrXor(index_prev,index->prev_xor_next);
                index_prev = temp;
                printf("0x%p:%7d :0x%p\n",index,index->data,index->prev_xor_next);
            }
        }
    }
    printf("\n");
}



/*----------------------------------*/
/*XLCLの使用を終了する関数            */
/*----------------------------------*/
__declspec(dllexport) void *__cdecl xlcl_end(void *vp)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;

    if(Root_addr!=NULL)
    {
        while(Root_addr->Head!=NULL)
        {
            xlcl_get_tail(Root_addr,NULL);
        } 
        free(Root_addr);
    }
    return NULL;
}