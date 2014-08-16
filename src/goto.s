        .equ	PIO_BASE,       0xFFFFF400	/* PIO Base Address */
        .equ	PIO_PER,	0x00000000	/* offset */
        .equ	PIO_PDR,	0x00000004	/* offset */
        .equ	PIO_OER,	0x00000010	/* offset */
        .equ	PIO_ODR,	0x00000014	/* offset */
        .equ	PIO_ODSR,	0x00000038	/* offset */
        .equ	PIO_PUER,	0x00000060	/* offset */
        .equ	PIO_PUDR,	0x00000064	/* offset */
        .equ	PIO_ASR,	0x00000070	/* offset */
        .equ	PIO_BSR,	0x00000074	/* offset */
        .equ	PIO_OWER,	0x000000A0	/* offset */
        .equ	PIO_OWDR,	0x000000A4	/* offset */

        .text
        .arm
        .global	GOTO
	.global	pinFreqTest

@---------------------------------
GOTO:
    mov		pc, r0
@---------------------------------
pinFreqTest:
    ldr		r0, =PIO_BASE
    
    ldr		r1, =0xFFFFFFBF
    ldr		r2, =0xFFFFFFFF
    str		r1, [r0, #PIO_PER]
    str		r1, [r0, #PIO_OER]
    str		r2, [r0, #PIO_PUDR]
    str		r1, [r0, #PIO_ASR]
    str		r1, [r0, #PIO_OWER]
    mvn		r1, r1
    mvn		r2, r2
    str		r1, [r0, #PIO_PDR]
    str		r1, [r0, #PIO_ODR]
    str		r2, [r0, #PIO_PUER]
    str		r1, [r0, #PIO_BSR]
    str		r1, [r0, #PIO_OWDR]

    mov		r1, #0
    mov		r2, #0xF
pinFreqTestLoop:
    str		r1, [r0, #PIO_ODSR]	@0
    str		r2, [r0, #PIO_ODSR]	@1
    str		r1, [r0, #PIO_ODSR]	@0
    str		r2, [r0, #PIO_ODSR]	@1
    str		r1, [r0, #PIO_ODSR]	@0
    str		r2, [r0, #PIO_ODSR]	@1
    str		r1, [r0, #PIO_ODSR]	@0
    str		r2, [r0, #PIO_ODSR]	@1
    b		pinFreqTestLoop
@---------------------------------
