#include "i8042.h"

.global _kbc_code
.global _kbd_int_handler_a
.data

_kbc_code:
.byte  0x03

.text
_kbd_int_handler_a:

inb $(OUT_BUF)
mov  %al, _kbc_code
ret
