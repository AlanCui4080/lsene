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
.section .mbt2std

tag_start:
.long 0xE85250D6
.long 0
.long tag_end-tag_start
.long -1*(tag_end-tag_start + 0 +  0xE85250D6)

.vbe:
.short 5
.short 0
.long 20
.long 1024
.long 768
.long 32
.vbe_e:
.long 0
tag_end:
.long 0xffffffffffffffff

/* OverExcutingProtection*/
hlt
hlt
hlt
hlt
hlt
/* OverExcutingProtection*/

.section .text
.globl _kstart
_kstart:
    movl $_fmem_start,%ebp
    movl %ebp,%esp
    jmp kmain
    hlt