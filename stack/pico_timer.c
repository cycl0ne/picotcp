/*********************************************************************
   PicoTCP.
   See LICENSE and COPYING for usage.
   
   Timer Functions.
    
   Authors: Claus Herrmann
 *********************************************************************/
#include "pico_config.h"
#include "pico_frame.h"
#include "pico_device.h"
#include "pico_protocol.h"
#include "pico_stack.h"
#include "pico_addressing.h"
#include "pico_dns_client.h"

#include "pico_ethernet.h"
#include "pico_olsr.h"
#include "pico_aodv.h"
#include "pico_eth.h"
#include "pico_arp.h"
#include "pico_ipv4.h"
#include "pico_ipv6.h"
#include "pico_icmp4.h"
#include "pico_icmp6.h"
#include "pico_igmp.h"
#include "pico_udp.h"
#include "pico_tcp.h"
#include "pico_socket.h"
#include "heap.h"
#include "pico_list.h"

volatile pico_time pico_tick;

static uint32_t tmr_id = 0u;
typedef struct pico_timer_ref pico_timer_ref;

struct pico_timer
{
    void *arg;
    void (*timer)(pico_time timestamp, void *arg);
};
 
#if 1

List_t pico_timer_list;

struct pico_timer_ref
{
    Node_t      ptr_node;
    pico_time   expire;
    uint32_t    id;
    uint32_t    hash;
    struct pico_timer *tmr;
};

int32_t pico_init_timers(void)
{
	pico_newlist(&pico_timer_list);
	return 0;
}

void pico_check_timers(void)
{
	struct pico_timer	*t;
	struct pico_timer_ref *ptr1, *ptr2;
	pico_tick    = PICO_TIME_MS();
	
	ForeachNodeSafe(&pico_timer_list, ptr1, ptr2)
	{
		if (ptr1->expire > pico_tick) break;
		pico_remove((pNode)ptr1);
		t = ptr1->tmr;
        if (t && t->timer) t->timer(pico_tick, t->arg);
        if (t) PICO_FREE(t);
        PICO_FREE(ptr1);
	}
}

void MOCKABLE pico_timer_cancel(uint32_t id)
{
	struct pico_timer_ref *tref, *tref2;
	if (id == 0u) return;

	ForeachNodeSafe(&pico_timer_list, tref, tref2)
	{
		if (tref->id == id)
		{
			pico_remove((pNode)tref);
			if (tref->tmr) PICO_FREE(tref->tmr);
			PICO_FREE(tref);
			break;
		}
	}
}

void pico_timer_cancel_hashed(uint32_t hash)
{
	struct pico_timer_ref *tref, *tref2;
	if (hash == 0u) return;

	ForeachNodeSafe(&pico_timer_list, tref, tref2)
	{
		if (tref->hash == hash)
		{
			pico_remove((pNode)tref);
			if (tref->tmr) PICO_FREE(tref->tmr);
			PICO_FREE(tref);
			break;
		}
	}
}

static struct pico_timer *pico_timer_create(void (*timer)(pico_time, void *), void *arg)
{
	struct pico_timer *t = PICO_ZALLOC(sizeof(struct pico_timer));
	
	if (!t) 
	{
		pico_err = PICO_ERR_ENOMEM;
		return NULL;
	}
	
	t->arg = arg;
	t->timer = timer;
	return t;
}

static void pico_insert_request(struct pico_timer_ref *tref)
{
	struct pico_timer_ref *node;
	ForeachNode(&pico_timer_list, node)
	{
		if (tref->expire <= node->expire)
		{
			pico_insert(&pico_timer_list, &tref->ptr_node, node->ptr_node.ln_Pred);
			return;
		}
	}
	pico_addtail(&pico_timer_list, (pNode)tref);
}

static uint32_t pico_timer_ref_add(pico_time expire, struct pico_timer *t, uint32_t id, uint32_t hash)
{
	struct pico_timer_ref *tref = PICO_ZALLOC(sizeof(struct pico_timer_ref));
	
    tref->expire = PICO_TIME_MS() + expire;
    tref->tmr = t;
    tref->id = id;
    tref->hash = hash;
    
    pico_insert_request(tref);
    return tref->id;
}

MOCKABLE uint32_t pico_timer_add(pico_time expire, void (*timer)(pico_time, void *), void *arg)
{
	struct pico_timer *t = pico_timer_create(timer, arg);
	/* zero is guard for timers */
    if (tmr_id == 0u) tmr_id++;
    if (!t) return 0;
    
    return pico_timer_ref_add(expire, t, tmr_id++, 0);
}

uint32_t pico_timer_add_hashed(pico_time expire, void (*timer)(pico_time, void *), void *arg, uint32_t hash)
{
	struct pico_timer *t = pico_timer_create(timer, arg);
	/* zero is guard for timers */
	if (tmr_id == 0u) tmr_id++;
	if (!t) return 0;
	
	return pico_timer_ref_add(expire, t, tmr_id++, hash);
} /* Static path count: 4 */

#else

struct pico_timer_ref
{
    pico_time expire;
    uint32_t id;
    uint32_t hash;
    struct pico_timer *tmr;
};

DECLARE_HEAP(pico_timer_ref, expire);
static heap_pico_timer_ref *Timers;

int32_t pico_init_timers(void)
{
	/* Initialize timer heap */
	Timers = heap_init();
	if (!Timers) return -1;
	return 0;
}

void pico_check_timers(void)
{
    struct pico_timer *t;
    struct pico_timer_ref tref_unused, *tref = heap_first(Timers);
    pico_tick = PICO_TIME_MS();
    while((tref) && (tref->expire < pico_tick)) {
        t = tref->tmr;
        if (t && t->timer)
            t->timer(pico_tick, t->arg);

        if (t)
        {
            PICO_FREE(t);
        }

        heap_peek(Timers, &tref_unused);
        tref = heap_first(Timers);
    }
}

void MOCKABLE pico_timer_cancel(uint32_t id)
{
    uint32_t i;
    struct pico_timer_ref *tref = Timers->top;
    if (id == 0u)
        return;

    for (i = 1; i <= Timers->n; i++) {
        if (tref[i].id == id) {
            if (Timers->top[i].tmr)
            {
                PICO_FREE(Timers->top[i].tmr);
                Timers->top[i].tmr = NULL;
                tref[i].id = 0;
            }
            break;
        }
    }
}

void pico_timer_cancel_hashed(uint32_t hash)
{
    uint32_t i;
    struct pico_timer_ref *tref = Timers->top;
    if (hash == 0u)
        return;

    for (i = 1; i <= Timers->n; i++) {
        if (tref[i].hash == hash) {
            if (Timers->top[i].tmr)
            {
                PICO_FREE(Timers->top[i].tmr);
                Timers->top[i].tmr = NULL;
                tref[i].id = 0;
            }
        }
    }
}

static uint32_t pico_timer_ref_add(pico_time expire, struct pico_timer *t, uint32_t id, uint32_t hash)
{
    struct pico_timer_ref tref;

    tref.expire = PICO_TIME_MS() + expire;
    tref.tmr = t;
    tref.id = id;
    tref.hash = hash;

    heap_insert(Timers, &tref);
    if (Timers->n > PICO_MAX_TIMERS) {
        dbg("Warning: I have %d timers\n", (int)Timers->n);
    }

    return tref.id;
}

static struct pico_timer *
pico_timer_create(void (*timer)(pico_time, void *), void *arg)
{
    struct pico_timer *t = PICO_ZALLOC(sizeof(struct pico_timer));

    if (!t) {
        pico_err = PICO_ERR_ENOMEM;
        return NULL;
    }

    t->arg = arg;
    t->timer = timer;

    return t;
}

MOCKABLE uint32_t pico_timer_add(pico_time expire, void (*timer)(pico_time, void *), void *arg)
{
    struct pico_timer *t = pico_timer_create(timer, arg);

    /* zero is guard for timers */
    if (tmr_id == 0u) {
        tmr_id++;
    }

    if (!t)
        return 0;

    return pico_timer_ref_add(expire, t, tmr_id++, 0);
}

uint32_t pico_timer_add_hashed(pico_time expire, void (*timer)(pico_time, void *), void *arg, uint32_t hash)
{
    struct pico_timer *t = pico_timer_create(timer, arg);

    /* zero is guard for timers */
    if (tmr_id == 0u) {
        tmr_id++;
    }

    if (!t)
        return 0;

    return pico_timer_ref_add(expire, t, tmr_id++, hash);
} /* Static path count: 4 */

#endif