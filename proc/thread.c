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
#include <lsene/mem.h>
#include <string.h>
#include "thread.h"
extern __attribute__ ((__noreturn__)) void _do_into(void *);
struct thread *currentp;
struct proc *current;
struct proc *tid_table[NR_THREAD];
void init_thread(void)
{
    memset(tid_table, 0, NR_THREAD * 4);
}
unsigned long gen_tid(struct proc *p)
{
    for (size_t i = 0; i < NR_THREAD; i++)
    {
        if (!tid_table[i])
            tid_table[i] = p;
    }
    //todo:Panic
    return -1UL;
}
unsigned long thread_create(void *exec, void *arg, unsigned long flag) //only support kernel thread now!
{
    if (!(flag & THRB_KERN)) //if not kern,exit
        return;
    struct proc *p = get_proc(current->pid);
    struct thread *t;
    for (size_t i = 0; i < NR_THREAD_PERPROC / 8; i++)
    {
        if (p->tstack[i])
            for (size_t j = 0; j < 8; j++)
            {
                if (!((struct thread *)&(p->tstack[i]))[j].state) //if empty
                {
                    //setting thread
                    t = &(((struct thread *)&(p->tstack[i]))[j]);
                    t->tid = gen_tid(t);
                    t->state = THR_RUNNING;
                    t->flag = flag;
                    t->ebp = (unsigned char *)kpalloc(4) + 4 * 4096; //16kb stack space
                    t->pid = current->pid;
                    //push back arg
                    *(unsigned long *)(t->ebp) = arg;
                    return t->tid;
                }
            }
    }
    //all used pages are full
    for (size_t i = 0; i < NR_THREAD_PERPROC / 8; i++)
    {
        if (!(p->tstack[i]))
        {
            p->tstack[i] = kpalloc(1);
            memset(p->tstack[i], 0, PAGE_SIZE);
            //setting thread
            t = p->tstack[i];
            t->tid = gen_tid(t);
            t->state = THR_RUNNING;
            t->flag = flag;
            t->pid = current->pid;
            t->ebp = (unsigned char *)kpalloc(4) + 4 * 4096; //16kb stack space
            //push back arg
            *(unsigned long *)(t->ebp) = arg;
            return t->tid;
        }
    }
}
void thread_exit()
{
    if (!(currentp->flag & THRB_KERN)) //if not kern,exit
        return;
    //free stack
    kpfree(currentp->ebp);
    current->state = 0; //exit directly
}
void into_thread(struct thread *p)
{
    currentp = p;
    _do_into(currentp);
}
void schedule(void)
{
}