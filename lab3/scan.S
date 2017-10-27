#include "i8042.h"

.global _kbc_code
.global _kbd_int_handler_a
.data

_kbc_code:
.byte  0x03

.text
_kbd_int_handler_a:

outb %al, $(STATUS_REG)

shl $7, %al
shr $7, %al
cmp $1, %al
jne a

inb $(OUT_BUF)
mov  %al, _kbc_code
ret
a:
ret
