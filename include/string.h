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
#ifndef _C_STRING_H_
#define _C_STRING_H_

#ifndef _SIZE_T_
#define _SIZE_T_
//fastest unsigned
typedef unsigned int size_t;
#include <limits.h>
#define SIZE_T_MAX UINT_MAX
#endif

#ifndef _RESTRICT_
#define _RESTRICT_
#define restrict
#endif

extern size_t strlen(const char *s);
extern size_t strnlen(const char *s, size_t maxlen);
extern void *memchr(const void *s, int c, size_t n);
extern void *memrchr(const void *s, int c, size_t n);
extern void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
extern char *strcpy(char *restrict s1, const char *restrict s2);
extern char *strncpy(char *restrict s1, const char *restrict s2, size_t n);
extern char *strcat(char *restrict s1, const char *restrict s2);
extern char *strncat(char *restrict s1, const char *restrict s2, size_t n);
extern int strcmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern char *strchr(const char *s, int c);
extern char *strrchr(const char *s, int c);
extern void *memset(void *s, int c, size_t n);

#endif