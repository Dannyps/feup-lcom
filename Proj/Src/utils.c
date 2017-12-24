#include "utils.h"
#include "keyboard.h"
#include "timer.h"
#include "month_pixmap.h"
#include "read_xpm.h"
#include "video.h"
#include <stdio.h>
#include <stdlib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include "view.h"

static View cal = {2017, 1, 2};

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

	int ipc_status;
	int r;
	message msg;
	int stop=0;
	while(!stop) { /*	You may want to use a different condition*/
		/*Get a request message.*/
		 if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			 printf("driver_receive failed with: %d", r);
			 continue;
		 }
		 if (is_ipc_notify(ipc_status)) {
			 int irq_set=0; irq_set |= BIT(0); // 0 as in the kbc_hookIDs[0].
			 int irq_timer0=0; irq_timer0 |= BIT(1); // 1 as in the timer0_hookIDs[0].
			 switch (_ENDPOINT_P(msg.m_source)) {
				 case HARDWARE:
					if (msg.NOTIFY_ARG & irq_timer0) {
						timer0_int_handler();
					}
					if (msg.NOTIFY_ARG & irq_set) {
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

	drawMonth(cal, 400, 90);
	int k=0;

}
