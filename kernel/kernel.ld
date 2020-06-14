OUTPUT_FORMAT("binary");

MEMORY
{
	dummy(r) : ORIGIN = 0x123456789, LENGTH = 16m
	ram(rwx) : ORIGIN = 0x100000000, LENGTH = 16m
}

SECTIONS
{
	.header : {
		QUAD(__bss_start)
		QUAD(__bss_end - __bss_start)
	} > dummy

	.body : {
		*(.text)

		*(.rodata)

		*(.data)

		__bss_start = .;
		*(.bss)
		__bss_end = .;

		*(.eh_frame)
	} > ram AT> dummy
}
