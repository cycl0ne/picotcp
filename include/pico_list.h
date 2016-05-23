/*********************************************************************
   PicoTCP.
   See LICENSE and COPYING for usage.
   
   .
    
   Authors: Claus Herrmann
 *********************************************************************/

#ifndef INCLUDE_PICO_LIST
#define INCLUDE_PICO_LIST

typedef struct Node
{
    struct Node *ln_Succ;
    struct Node *ln_Pred;
} Node_t, *pNode;

typedef struct List
{
    pNode   lh_Head;
    pNode   lh_Tail;
    pNode   lh_TailPred;
} List_t, *pList;

#define IsListEmpty(l)\
        ( (((pList)l)->lh_TailPred) == (pNode)(l) )

#define SafeIsListEmpty(l)\
        ((l)&&((((pList)(l))->lh_Head) == ((pNode)&(((pList)(l))->lh_Tail))))

#define SafeGetHead(l)\
		(l) ? (((((pList)(l))->lh_Head) == ((pNode)&(((pList)(l))->lh_Tail))) ? (NULL) : ((((pList)(l))->lh_Head))) : NULL

#define SafeGetTail(l)\
		(l) ? (((((pList)(l))->lh_Head) == ((pNode)&(((pList)(l))->lh_Tail))) ? (NULL) : ((((pList)(l))->lh_TailPred))) : NULL

#define ForeachNode(l,n) \
   for (n=(void *)(((pList)(l))->lh_Head); \
       ((pNode)(n))->ln_Succ; \
       n=(void *)(((pNode)(n))->ln_Succ))

#define ForeachNodeSafe(l,n,n2)         \
for                                     \
(                                       \
    n=(void *)(((pList)(l))->lh_Head);  \
    (n2=(void *)((pNode)(n))->ln_Succ); \
    n=(void *)n2                        \
)

#   define GetHead(l)       (void *)(((pList)l)->lh_Head->ln_Succ \
				? ((pList)l)->lh_Head \
				: (pNode)0)
#   define GetTail(l)       (void *)(((pList)l)->lh_TailPred->ln_Pred \
				? ((pList)l)->lh_TailPred \
				: (pNode)0)
#   define GetSucc(n)       (void *)(((pNode)n)->ln_Succ->ln_Succ \
				? ((pNode)n)->ln_Succ \
				: (pNode)0)
#   define GetPred(n)       (void *)(((pNode)n)->ln_Pred->ln_Pred \
				? ((pNode)n)->ln_Pred \
				: (pNode)0)

/* List functions */
void pico_addhead(pList list, pNode node);
pNode pico_remhead(pList list);
pNode pico_remtail(pList list);
void pico_remove(pNode node);
void pico_newlist(pList list);
void pico_insert(pList list, pNode node, pNode pred);
void pico_addtail(pList list, pNode node);

#endif
 