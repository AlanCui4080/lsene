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
#ifndef _KTHREAD_H_
#define _KTHREAD_H_
#include <limits.h>
#define NR_THREAD 8192UL
struct proc//SIZE:128
{
    unsigned long pid;
    unsigned long uid;
     //signal slot
    unsigned long signal;
    //thread status
    unsigned long state;
    unsigned long flag;
    //thread stack point,point to page including thread
    void *tstack[16];
    //number of threads in singal proc
    #define NR_THREAD_PERPROC 128
    unsigned long rev1[107];
}__attribute__ ((packed));
#define THRB_KERN 1

#define THR_RUNNING 1
#define THR_IN_CPU 2
#define THR_INTERRUPTABLLE 3
#define THR_UNINTERRUPTABLE 4
#define THR_TRACED 5
#define THR_DEAD 6
#define THR_ZOMBIE 7
//describe a thread
struct thread//SIZE:512
{
    unsigned long eip;
    //thread owner pid
    unsigned long pid;
    //register
    unsigned long eax,ebx,ecx,edx;
    //register
    unsigned long esi,edi;
    //register
    unsigned long esp,ebp;
    //register
    unsigned long eflags;
    //thread id
    unsigned long tid;
    //signal slot
    unsigned long signal;
    //thread status
    unsigned long state;
    unsigned long flag;

    unsigned long rev0;

    unsigned char floats[448];

    unsigned long rev1[12];

}__attribute__ ((packed));

#endif

