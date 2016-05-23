/*********************************************************************
   PicoTCP.
   See LICENSE and COPYING for usage.
   
   List Functions.
    
   Authors: Claus Herrmann
 *********************************************************************/
 
#include "pico_config.h"
#include "pico_frame.h"
#include "pico_device.h"
#include "pico_protocol.h"
#include "pico_stack.h"
#include "pico_addressing.h"
#include "pico_list.h"

void pico_addhead(pList list, pNode node)
{
   // list o-o newnode o-o node
   node->ln_Succ = list->lh_Head;
   node->ln_Pred = (pNode)&list->lh_Head;
   list->lh_Head->ln_Pred = node;
   list->lh_Head = node;
}

void pico_addtail(pList list, pNode node)
{
   //  list o-o node o-o node o-o node o-o newnode
   node->ln_Succ = (pNode)&list->lh_Tail;
   node->ln_Pred = list->lh_TailPred;
   list->lh_TailPred->ln_Succ = node;
   list->lh_TailPred          = node;
}

void pico_insert(pList list, pNode node, pNode pred)
{
	if (!pred)
	{
		pico_addhead(list,node);
		return;
	}
	if (!pred->ln_Succ)
	{
	    pico_addtail(list, node);
	} else
	{
		node->ln_Succ = pred->ln_Succ;
		node->ln_Pred = pred;
		pred->ln_Succ->ln_Pred = node;
		pred->ln_Succ = node;
	}
}

void pico_newlist(pList list)
{
    list->lh_Tail       = NULL;
    list->lh_Head       = (pNode)&list->lh_Tail;
    list->lh_TailPred   = (pNode)&list->lh_Head;
}

void pico_remove(pNode node)
{
	node->ln_Succ->ln_Pred = node->ln_Pred;
	node->ln_Pred->ln_Succ = node->ln_Succ;
}

pNode pico_remtail(pList list)
{
	pNode node;
	if (IsListEmpty(list)) return NULL;

	node = list->lh_TailPred;

	node->ln_Succ->ln_Pred = node->ln_Pred;
	node->ln_Pred->ln_Succ = node->ln_Succ;

	return node;
}

pNode pico_remhead(pList list)
{
	pNode node;
	if (IsListEmpty(list)) return NULL;

	node = list->lh_Head;

	node->ln_Succ->ln_Pred = node->ln_Pred;
	node->ln_Pred->ln_Succ = node->ln_Succ;

    return node;
}

