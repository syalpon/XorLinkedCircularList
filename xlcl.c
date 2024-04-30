#include <stdio.h>
#include <stdlib.h>

/*���ȎQ�ƍ\����:XorLinkedCircularList*/
typedef struct XorLinkedCircularList_T
{
    int data;
    struct XorLinkedCircularList_T *prev_xor_next;
}XorLinkedCircularList;

/*XorLinkedCircularList�̈�ԏ��߂̃A�h���X���i�[����\����*/
typedef struct XorLinkedCircularList_Root_T
{
    XorLinkedCircularList *Head;
    XorLinkedCircularList *Tail;
    size_t AllocationSize;
    unsigned int datanum;
}XorLinkedCircularList_Root;



/*----------------------------------*/
/* ������̃|�C���^��XOR���ĕԂ��֐� */
/*----------------------------------*/
static void *AddrXor(void *p,void *q)
{
    return (void *)(((unsigned int)p) ^ ((unsigned int)q));
}



/*----------------------------------*/
/* �w��m�[�h�A�h���X��Ԃ��֐�        */
/*----------------------------------*/
static void *GetAddrXLCL(XorLinkedCircularList_Root *root,unsigned int index)
{
    XorLinkedCircularList *addr1 = root->Tail;
    XorLinkedCircularList *addr2 = root->Head;
    XorLinkedCircularList *xlxladdr;

    /*index�Ԗڂ̃f�[�^��T��*/
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
/*XorLinkedCircularList�̎g�p�J�n�֐�*/
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
/*XLCL�Ƀm�[�h��ǉ�����֐�          */
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
            /*�m�ۂ����̈�Ƀf�[�^���i�[*/
            obtained_addr->data = additional_data;

            if( Root_addr->Head == NULL ) /*����̃m�[�h�ǉ��̍�*/
            {
                /*root�̐擪�A�h���X���X�V*/
                Root_addr->Head = obtained_addr;

                /*����̃m�[�h�͎������g�̃A�h���X��Xor���s����0*/
                obtained_addr->prev_xor_next = NULL;
            }
            else
            {
                /*1�����m�[�h�����݂��Ă��鎞*/
                if(Root_addr->Head == Root_addr->Tail)
                {
                    obtained_addr->prev_xor_next = NULL;
                }
                /*2�ȏ�̃m�[�h�����݂��Ă��鎞*/
                else
                {
                    /*2�����m�[�h�����݂��Ă��鎞*/
                    if(Root_addr->Head->prev_xor_next == NULL)
                    {
                        Root_addr->Head->prev_xor_next = AddrXor(obtained_addr,Root_addr->Tail);
                        Root_addr->Tail->prev_xor_next = AddrXor(Root_addr->Head,obtained_addr);
                        obtained_addr->prev_xor_next   = AddrXor(Root_addr->Head,Root_addr->Tail);
                    }
                    /*3�ȏ�m�[�h�����݂��Ă���Ƃ�*/
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

            /*Rood�ɕۑ����閖���A�h���X���X�V*/
            Root_addr->Tail = obtained_addr;
        }
    }
    return obtained_addr;
}



/*----------------------------------*/
/*XLCL����f�[�^�����o���֐�        */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_pop(void *vp,unsigned int index,int *retdata)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    int retvalue = 0;

    if(Root_addr != NULL) /*NULL�`�F�b�N*/
    {
        if( Root_addr->datanum != 0 )  /*1�ȏ�̃m�[�h������ꍇ*/
        {
            XorLinkedCircularList *index_node_addr;
            XorLinkedCircularList *index_node_nextaddr;
            XorLinkedCircularList *index_node_prevaddr;

            /* index�Ԗڂ̃f�[�^��T�� */
            index_node_addr = GetAddrXLCL(Root_addr,index);
            if( index_node_addr != NULL )
            {
                if( retdata != NULL )
                {
                    *retdata = index_node_addr->data;
                }
                else
                {
                    /*�f�[�^�͎擾�ł��邪�A�Ԓl�p�o�b�t�@���Ȃ��ꍇ*/
                    /* Do Nothing */
                }
                retvalue = 1;

                /* �폜���� */
                if( Root_addr->datanum == 1 ) /*�m�[�h��1�̂Ƃ�*/
                {
                    Root_addr->Head = NULL;
                    Root_addr->Tail = NULL;
                }
                else /*�m�[�h��2�ȏ�̂Ƃ�*/
                {
                    /*�X�V�p�|�C���^�F���������K�v�Ȃ���*/
                    void *Root_Addr_Head_temp = Root_addr->Head;
                    void *Root_Addr_Tail_temp = Root_addr->Tail;

                    /*�폜����m�[�h�̑O�̃m�[�h�A�h���X���擾*/
                    if( index_node_addr == Root_addr->Head ) /*�w��m�[�h��HEAD*/
                    {
                        index_node_prevaddr = Root_addr->Tail;
                        Root_Addr_Head_temp = AddrXor(Root_addr->Head->prev_xor_next,index_node_prevaddr);
                    }
                    else
                    {
                        index_node_prevaddr = GetAddrXLCL(Root_addr,index-1);
                    }

                    /*�폜����m�[�h�̎��̃m�[�h�A�h���X���擾*/
                    if( index_node_addr == Root_addr->Tail ) /*�w��m�[�h��TAIL*/
                    {
                        index_node_nextaddr = Root_addr->Head;
                        Root_Addr_Tail_temp = AddrXor(Root_addr->Tail->prev_xor_next,index_node_nextaddr);
                    }
                    else
                    {
                        index_node_nextaddr = GetAddrXLCL(Root_addr,index+1);
                    }
                    
                    /*�X�V*/
                    Root_addr->Head = Root_Addr_Head_temp;
                    Root_addr->Tail = Root_Addr_Tail_temp;

                    /*XorAddr�̎Z�o*/
                    if( Root_addr->datanum >= 3 ) /*�m�[�h��3�ȏ�̂Ƃ�*/
                    {
                        /*�X�V�p�|�C���^�F���������K�v�Ȃ���*/
                        void *N_i_XorAddr;
                        void *N_j_XorAddr;
                        
                        /* �@N_{i+1}->XorAddr = N_{i+2} ^ N_{i-1} */ 
                        /* �AN_{i+2} = N_{i+1}->XorAddr ^ N_{i}   */
                        /*  N_{i+1}->XorAddr = N_{i+1}->XorAddr ^ N_{i} ^ N_{i-1} */
                        N_i_XorAddr = AddrXor(AddrXor(index_node_nextaddr->prev_xor_next,index_node_addr),index_node_prevaddr);
                        /* �@N_{i-1}->XorAddr = N_{i-2} ^ N_{i+1} */ 
                        /* �AN_{i-2} = N_{i-1}->XorAddr ^ N_{i}   */
                        /*  N_{i-1}->XorAddr = N_{i-1}->XorAddr ^ N_{i} ^ N_{i+1} */
                        N_j_XorAddr = AddrXor(AddrXor(index_node_prevaddr->prev_xor_next,index_node_addr),index_node_nextaddr);

                        /*�X�V*/
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
/*XLCL�̐擪����f�[�^�����o���֐�   */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_get_head(void *vp,int *ret)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    return xlcl_pop(Root_addr,0,ret);
}



/*----------------------------------*/
/*XLCL�̖�������f�[�^�����o���֐�   */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_get_tail(void *vp,int *ret)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    return xlcl_pop(Root_addr,Root_addr->datanum-1,ret);
}



/*----------------------------------*/
/*XLCL�̃f�[�^��W������֐�          */
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
/*XLCL�̎g�p���I������֐�            */
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