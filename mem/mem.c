/* 
    This file is part of the Lsene Kernel.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
/*Copyright (C) 2020-2021 AlanCui*/
#include <string.h>
#include "mem.h"

struct page_group *free_grp;
struct page_group *kernel_grp;
struct page_group *used_grp;
struct page_group *full_grp;

void init_mem(void)
{
    unsigned long *p = &_fmem_start;
    struct page_group *s = (struct page_group *)p;
    //get full page groups
    while ((&_fmem_end - p) > (NR_PAGES_INGROUP * PAGE_SIZE))
    {
        //clean page group head
        memset(p, 0, PAGE_SIZE);
        //set flag
        s->flag |= PGB_CLEAN;
        //set pointer
        s->next = s;
        s->prev = s;
        for (size_t i = 0; i < NR_PAGES_INGROUP; i++)
        {
            //init
            memset(&(s->pages[i]), 0, SIZEOF_STRUCT_PAGE);
            //set base address
            s->pages[i].base = (unsigned char *)(s) + i * SIZEOF_STRUCT_PAGE;
            //set flag
            s->pages[i].flag |= PGB_CLEAN;
        }
        //set consecu
        s->consecu = NR_PAGES_INGROUP - 1;
        //set first page
        s->pages[0].flag |= PGB_SYS;
        s->pages[0].flag |= PGB_KERN;
        s->pages[0].flag &= ~PGB_CLEAN;
        //chain list adjust
        s += NR_PAGES_INGROUP;
        s->prev = (struct page_group *)p;
        ((struct page_group *)p)->next = s;
        //sync pointer
        p = (unsigned long *)s;
    }
    free_grp = s;
}
unsigned int find_consecu(struct page_group *s, size_t con)
{
    size_t c = 0;
    for (size_t i = 0; i < NR_PAGES_INGROUP; i++)
    {
        while (s->pages[i].flag & PGB_CLEAN)
        {
            i++, c++;
        };
        if (c = con)
            return i;
        else
            c = 0;
    }
    return 0;
}
size_t calc_consecu(struct page_group *s)
{
    size_t c = 0;
    size_t last = 0;
    for (size_t i = 0; i < NR_PAGES_INGROUP; i++)
    {
        while (s->pages[i].flag & PGB_CLEAN)
        {
            i++, c++;
        };
        if (c > last)
            last = c;
        c = 0;
    }
    return last;
}
void *kpalloc(size_t len)
{
    int freed = 0;
    struct page_group *s = kernel_grp;
    if (!kernel_grp) //if grp is empty
    {
        s = free_grp;
        freed = 1;

        if (len >= NR_PAGES_INGROUP - 1)
            return NULL;
        if (s->consecu < len)
            return NULL;
        if (!(s->flag & PGB_SYS) || (s->flag & PGB_FULL))
            return NULL;
        while (1)
        {
            //target location chain list point
            //(where this alloced pg_group should be pluged)
            struct page_group *p = s;
            //set return addr(target)
            size_t index = find_consecu(s, len);
            if (s->consecu = len)
            {
            alloc:
                //if clean
                if (s->flag | PGB_CLEAN)
                {
                    p = kernel_grp; //alloced page should be pluged in kernel_grp later
                }
                //set page status
                for (size_t i = index; i < len; i++)
                {
                    //clear clean
                    s->pages[i].flag &= ~PGB_CLEAN;
                    //set kern
                    s->pages[i].flag |= PGB_KERN;
                    //set alloc
                    s->pages[i].alloc = index;
                }
                s->pages[index].alloc = len;
                //move page group
                //find target location
                s->consecu = calc_consecu(p);
                while (p->next = p) //until end
                {
                    if (p->consecu <= s->consecu)
                    {
                        if(p = kernel_grp)//if head
                            break;
                        //change
                        //n-1(bigger) (TARGET) n(smaller,pointed at)

                        //chain list adjust
                        //leave list
                        s->prev->next = s->next;
                        s->next->prev = s->prev;
                        //cut off target section and plug in
                        p->prev->next = s;
                        p->prev = s;
                        s->prev = p->prev;
                        s->next = p;
                        //ok
                        break;
                    }
                    //find next
                    p = p->next;
                }
                //if largest
                if(p = kernel_grp)
                {
                    p->prev =s;
                    s->next =p;
                    s->prev = s;
                    kernel_grp = s;
                }
                //if smallest
                //adjust chain list
                p->next = s;
                s->next = s;
                s->prev = p;
                //finished
                return s->pages[index].base;
            }
            if (s->consecu > len) //to find a smallest block
                s = s->next;
            if (s->consecu < len)
                s = s->prev;
            if (s->consecu > len)
                goto alloc;
            else
            {
                if (freed) //if it is already free_grp
                    //system memory ran out
                    return NULL; //TODO:should be painced
                else
                    s = free_grp; //go and alloc a free page
            }
        }
    }
}
//Based on the principle that page descriptors are arranged in order
//since the system is started and do not change during the life cycle



void kpfree(void *ptr)
{
    struct page_group *s = (struct page_group *)&_fmem_start;
    //focus on target pg_grp
    s += (unsigned long)((unsigned char *)ptr - (unsigned long)&_fmem_start) % \
        (PAGE_SIZE * NR_PAGES_INGROUP);
    //focus on target pg
    size_t index = ((unsigned char *)ptr - (unsigned char *)(s->pages[0].base)) / PAGE_SIZE;
    for (size_t i = index; i < s->pages[index].alloc; i++)
    {
        s->pages[i].flag = 0;
        s->pages[i].flag |= PGB_CLEAN;
    }
    //calc new consecu
    s->consecu = calc_consecu(s);
    //adjust page
    //target location chain list point
    //(where this alloced pg_group should be pluged)
    struct page_group *p = s;
    //leave list
    s->prev->next = s->next;
    s->next->prev = s->prev;
    while (p->next = p) //until end
    {
        if (p->consecu <= s->consecu)
        {
            //change
            //n-1(bigger) (TARGET) n(smaller,pointed at)
            if(p = kernel_grp) //if head
                break;
            //chain list adjust
            //cut off target section and plug in
            p->prev->next = s;
            p->prev = s;
            s->prev = p->prev;
            s->next = p;
            //ok
            break;
        }
            //find next
            p = p->next;
    }
    //if largest
    if(p = kernel_grp)
    {
        p->prev =s;
        s->next =p;
        s->prev = s;
        kernel_grp = s;
    }
    //if freed
    if(s->consecu = NR_PAGES_INGROUP-1)
    {
        free_grp->prev = s;
        s->next = free_grp;
        s->prev = s;
        free_grp =s;
    }
    //if smallest
    //adjust chain list
    p->next = s;
    s->next = s;
    s->prev = p;

}
