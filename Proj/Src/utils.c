#include "utils.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "month_pixmap.h"
#include "read_xpm.h"
#include "video.h"
#include <stdio.h>
#include <stdlib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include "view.h"

static View cal = {2017, 1, 2};
unsigned cursorX=50, cursorY=50;


void start_listening(){

	/* Subscribes to timer */
	int timer0_ret = timer0_subscribe_int();
	if(timer0_ret!=0){
		fprintf(stderr, "Couldn't subscribe to IRQ_0!\n");exit(-3);
	}

	/* Subscribes to kbd */
	int kbd_ret = kbd_subscribe_int();
	if(kbd_ret!=0){
		fprintf(stderr, "Could not subscribe interruptions for the kbc!\n");exit(-4);
	}

	/* Subscribes to mouse */
	int mouse_ret = kbd_mouse_subscribe_int();
	if(mouse_ret!=0){
		fprintf(stderr, "Could not subscribe interruptions for the kbc!\n");exit(-1);
	}

	int ipc_status;
	int r;
	message msg;
	int stop=0;
	printf("Entering the loop.\n");
	while(!stop) { /*	You may want to use a different condition*/
		/*Get a request message.*/
		if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			int irq_kbdset=0; irq_kbdset |= BIT(0); // 0 as in the kbc_hookIDs[0].
			int irq_timer0=0; irq_timer0 |= BIT(1); // 1 as in the timer0_hookIDs[0].
			int irq_mouseset=0; irq_mouseset |= BIT(2); // 2 as in the mouse_hookIDs[0].
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_timer0) {
					//printf("handling timer0\n");
					timer0_int_handler();
				}
				if (msg.NOTIFY_ARG & irq_kbdset) {
					printf("handling keyboard\n");
					KEY_PRESS* kp;
					kp=kbd_int_handler();
					if(kp==NULL)
						continue;
					if(kp->code==0x81){
						stop=1; continue;
					}
					if(kp->code==0x4d){ //right
						nextMonth(&cal);
						rfill_screen();
						draw_main_page();
						continue;
					}
					if(kp->code==0x4b){ //left
						prevMonth(&cal);
						rfill_screen();
						draw_main_page();
						continue;
					}
					free(kp);
				}
				if (msg.NOTIFY_ARG & irq_mouseset) {
					//printf("handling mouse\n");
					MOUSE_ACTION* ma;
					ma = kbd_mouse_int_handler();
					if(ma==NULL){ //device is not synced or this wasn't the final byte.
						continue;
					}
					if(ma->lmb == 1 || ma->z > 0) {
						nextMonth(&cal);
						rfill_screen();
						draw_main_page();
						free(ma);
						continue;
					}
					free(ma);
				}
				break;
			default:
				break;
				//no other notifications expected: do nothing
			}
		} else { /*received a standard message, not a notification*/
			/*no standard messages expected: do nothing*/
		}
	}

	if(timer0_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n"); exit(-5);
	}

	if(kbd_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n");	exit(-6);
	}

	if(kbd_mouse_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n");	exit(-6);
	}
	sys_outb(0x64, 0x20);
	sys_outb(0x60, 0x47);

	printf("\nunsubscribed successfully.\n");

	return;
}

/**
 * Make sure you clear the screen b4 calling this.
 */
void draw_main_page(){
	video_info_t vi;
	vi = get_vi();
	int i;
	int j;
	for(i = 0;i<vi.x;i++){
		for(j=0;j<100;j++){
			setP(i,j,white_c);
		}
	}

	draw_xpm(lcom_nome, 0, 0);
	draw_xpm(lcom_tcti, 724, 0);

	drawMonthName(cal, 300, 90);

	draw_xpm(Weekdays, 300, 140);

	// serves to see where the first day of the month falls on the week
	int weekday = calculateFirstWeekDay(cal);
	drawMonth(/*weekday*/ 1, 300, 190);

	int k=0;

}


const unsigned char letters[95][13] = {
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00 }, // space :32
		{ 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18,
				0x18, 0x18 }, // ! :33
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36,
				0x36, 0x36 }, { 0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66,
				0xff, 0x66, 0x66, 0x00, 0x00 }, { 0x00, 0x00, 0x18, 0x7e, 0xff,
				0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18 },
		{ 0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb,
				0xd8, 0x70 }, { 0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70,
				0xd8, 0xcc, 0xcc, 0x6c, 0x38 }, { 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e },
		{ 0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
				0x18, 0x0c }, { 0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c,
				0x0c, 0x0c, 0x0c, 0x18, 0x30 }, { 0x00, 0x00, 0x00, 0x00, 0x99,
				0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18,
				0x00, 0x00 }, { 0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c,
				0x0c, 0x06, 0x06, 0x03, 0x03 }, { 0x00, 0x00, 0x3c, 0x66, 0xc3,
				0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c },
		{ 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78,
				0x38, 0x18 }, { 0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18,
				0x0c, 0x06, 0x03, 0xe7, 0x7e }, { 0x00, 0x00, 0x7e, 0xe7, 0x03,
				0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e },
		{ 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c,
				0x1c, 0x0c }, { 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe,
				0xc0, 0xc0, 0xc0, 0xc0, 0xff }, { 0x00, 0x00, 0x7e, 0xe7, 0xc3,
				0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e },
		{ 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03,
				0x03, 0xff }, { 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e,
				0xe7, 0xc3, 0xc3, 0xe7, 0x7e }, { 0x00, 0x00, 0x7e, 0xe7, 0x03,
				0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e },
		{ 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00,
				0x1c, 0x1c, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x06, 0x0c, 0x18,
				0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06 },
		{ 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03,
				0x06, 0x0c, 0x18, 0x30, 0x60 }, { 0x00, 0x00, 0x18, 0x00, 0x00,
				0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e },
		{ 0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3,
				0xc3, 0xc3, 0x66, 0x3c, 0x18 }, { 0x00, 0x00, 0xfe, 0xc7, 0xc3,
				0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe },
		{ 0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
				0xe7, 0x7e }, { 0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3,
				0xc3, 0xc3, 0xc7, 0xce, 0xfc }, { 0x00, 0x00, 0xff, 0xc0, 0xc0,
				0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff },
		{ 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0,
				0xc0, 0xff }, { 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0,
				0xc0, 0xc0, 0xc0, 0xe7, 0x7e }, { 0x00, 0x00, 0xc3, 0xc3, 0xc3,
				0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 },
		{ 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
				0x18, 0x7e }, { 0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06,
				0x06, 0x06, 0x06, 0x06, 0x06 }, { 0x00, 0x00, 0xc3, 0xc6, 0xcc,
				0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3 },
		{ 0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
				0xc0, 0xc0 }, { 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
				0xdb, 0xff, 0xff, 0xe7, 0xc3 }, { 0x00, 0x00, 0xc7, 0xc7, 0xcf,
				0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3 },
		{ 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
				0xe7, 0x7e }, { 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe,
				0xc7, 0xc3, 0xc3, 0xc7, 0xfe }, { 0x00, 0x00, 0x3f, 0x6e, 0xdf,
				0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c },
		{ 0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3,
				0xc7, 0xfe }, { 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e,
				0xe0, 0xc0, 0xc0, 0xe7, 0x7e }, { 0x00, 0x00, 0x18, 0x18, 0x18,
				0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff },
		{ 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
				0xc3, 0xc3 }, { 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3,
				0xc3, 0xc3, 0xc3, 0xc3, 0xc3 }, { 0x00, 0x00, 0xc3, 0xe7, 0xff,
				0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 },
		{ 0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66,
				0x66, 0xc3 }, { 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
				0x3c, 0x3c, 0x66, 0x66, 0xc3 }, { 0x00, 0x00, 0xff, 0xc0, 0xc0,
				0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff },
		{ 0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
				0x30, 0x3c }, { 0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18,
				0x18, 0x30, 0x30, 0x60, 0x60 }, { 0x00, 0x00, 0x3c, 0x0c, 0x0c,
				0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66,
				0x3c, 0x18 }, { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70 },
		{ 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe,
				0xc0, 0xc0, 0xc0, 0xc0, 0xc0 }, { 0x00, 0x00, 0x7e, 0xc3, 0xc0,
				0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03,
				0x03, 0x03 }, { 0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3,
				0x7e, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x30, 0x30, 0x30,
				0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e },
		{ 0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
				0xfe, 0xc0, 0xc0, 0xc0, 0xc0 }, { 0x00, 0x00, 0x18, 0x18, 0x18,
				0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00 },
		{ 0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00,
				0x0c, 0x00 }, { 0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc,
				0xc6, 0xc0, 0xc0, 0xc0, 0xc0 }, { 0x00, 0x00, 0x7e, 0x18, 0x18,
				0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78 },
		{ 0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
				0xfc, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x7c, 0xc6, 0xc6,
				0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00 },
		{ 0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00,
				0x00, 0x00 }, { 0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3,
				0x7f, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0xc0, 0xc0, 0xc0,
				0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30,
				0xfc, 0x30, 0x30, 0x30, 0x00 }, { 0x00, 0x00, 0x7e, 0xc6, 0xc6,
				0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3,
				0xc3, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0xc3, 0x66, 0x3c,
				0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00 },
		{ 0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00,
				0x00, 0x00 }, { 0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06,
				0xff, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x0f, 0x18, 0x18,
				0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f },
		{ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
				0x18, 0x18 }, { 0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f,
				0x1c, 0x18, 0x18, 0x18, 0xf0 }, { 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00 } };  // :126

