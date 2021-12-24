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
#ifndef _MEM_H_
#define _MEM_H_
//page size
#define PAGE_SIZE 4096

//kernel pre-loaded free mem base 
extern unsigned long _fmem_start;
//kernel pre-loaded free mem end
extern unsigned long _fmem_end;

//page page-group
#define PGB_CLEAN 1UL
#define PGB_SYS (1UL << 1)
#define PGB_KERN (1UL << 1)
//page-group only
#define PGB_FULL (1UL << 31)

//sigle-page control struct (aligned)
struct page
{
    //base address of this page
    void* base;
    //type of this page
    unsigned short alloc;
    //flags of this page
    unsigned short flag;
    //reversed
    unsigned long rev0;
    unsigned long rev1;
}
__attribute__ ((packed))//this struct MUST be aligned manually
;
#define SIZEOF_STRUCT_PAGE 16
//a page group,which contains a group page ,1 control, n normal data,(n+1) total (aligned)
//as size as a page
struct page_group
{
    //chain list
    struct page_group *next;
    struct page_group *prev;
    //owner of this page
    unsigned long pid,uid;
    //flags of this page
    unsigned long flag;
    //max consecutive page
    unsigned long consecu; 
    unsigned long rev1;
    unsigned long rev2;

#define NR_PAGES_INGROUP 254
    struct page pages[254];

}
__attribute__ ((packed))//this struct MUST be aligned manually
;

#define KPB_KERNEL 1
#endif