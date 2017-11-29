#ifndef __KEYBOARD_H
#define __KEYBOARD_H

int kbc_hookIDs[2];
char stop;

#ifdef PROJ
unsigned int sysinbcount=0;
int sys_inb_cnt(port_t port, unsigned long* byte){
	sysinbcount++;
	return sys_inb(port, byte);
}
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

int kbd_subscribe_int(void );
int kbd_unsubscribe_int();
void kbd_int_handler();
int kbd_test_scan();

#endif
