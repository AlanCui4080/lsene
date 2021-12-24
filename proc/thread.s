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
_do_into:
    endbr32
    //get ebx-edi ready
    //eax do as an pointer
    movl (%esp),%eax

    movl 12(%eax),%ebx//ebx
    movl 16(%eax),%ecx//ecx
    movl 20(%eax),%edx//edx
    movl 24(%eax),%ebx//ebx
    movl 28(%eax),%ebx//esi
    movl 32(%eax),%ebx//edi
    pushl 44(%eax)//eflags
    popfl

    addl 64,%eax//floats
    frstor %eax
    movl 8(%eax),%eax //eax

    jmp (%esp)

    hlt

    
