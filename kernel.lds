ENTRY(_kstart)
SECTIONS
{

    . = 1M;
    .text ALIGN(8):
    {
        *(.mbt2std)
        *(.text)
        *(.kprotect_data)
    }
    .rodata :
    {
        *(.rodata)
    }
    .bss :
    {
        *(.bss)
    }
    .___void_aligner0 ALIGN(4k):{}
    _fmem_start = .;
    . = 64M;
    _fmem_end = .;


}