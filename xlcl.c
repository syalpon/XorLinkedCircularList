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
}XorLinkedCircularList_Root;


/*----------------------------------*/
/* ������̃|�C���^��XOR���ĕԂ��֐� */
/*----------------------------------*/
static void *AddrXor(void *p,void *q)
{
    return (void *)(((unsigned int)p) ^ ((unsigned int)q));
}


/*----------------------------------*/
/*XorLinkedCircularList�̎g�p�J�n�֐�*/
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
/*XLCL����擪�̃f�[�^�����o���֐�   */
/*----------------------------------*/
__declspec(dllexport) int __cdecl xlcl_pop(void *vp)
{
    XorLinkedCircularList_Root *Root_addr = (XorLinkedCircularList_Root *)vp;
    int ret = 0;
    if(Root_addr != NULL)
    {
        if(Root_addr->Tail != NULL) /*1�ȏ�̃m�[�h������ꍇ*/
        {
            ret = Root_addr->Tail->data;

            if(Root_addr->Tail->prev_xor_next != NULL) /*3�ȏ�̃m�[�h������ꍇ*/
            {
                XorLinkedCircularList *head_next = AddrXor(Root_addr->Head->prev_xor_next,Root_addr->Tail);
                XorLinkedCircularList *tail_prev = AddrXor(Root_addr->Tail->prev_xor_next,Root_addr->Head);
                XorLinkedCircularList *tail_prev_prev = AddrXor(tail_prev->prev_xor_next,Root_addr->Tail);
                XorLinkedCircularList *free_addr = Root_addr->Tail;

                /*�Ō�̃m�[�h�̃��������*/
                Root_addr->Tail = tail_prev;
                Root_addr->Tail->prev_xor_next = AddrXor(tail_prev_prev,Root_addr->Head);
                Root_addr->Head->prev_xor_next = AddrXor(Root_addr->Tail,head_next);
                free(free_addr);
                Root_addr->AllocationSize -= sizeof(XorLinkedCircularList);
            }
            else
            {
                if( Root_addr->Head != Root_addr->Tail ) /*2�̃m�[�h�̏ꍇ*/
                {
                    /*�Ō�̃m�[�h�̃��������*/
                    free(Root_addr->Tail);
                    Root_addr->AllocationSize -= sizeof(XorLinkedCircularList);
                    Root_addr->Tail = Root_addr->Head;
                }
                else /*1�̃m�[�h�̏ꍇ*/
                {
                    /*�Ō�̃m�[�h�̃��������*/
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
/*XLCL�̃f�[�^��W������֐�          */
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
/*XLCL�̎g�p���I������֐�            */
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