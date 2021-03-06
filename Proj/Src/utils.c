#include "utils.h"
#include "rtc.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "read_xpm.h"
#include "video.h"
#include "view.h"
#include <stdio.h>
#include <stdlib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <unistd.h>

unsigned cursorX=50, cursorY=50;

View cal;

int stop=0;
int landing = 1;
int search=0;
char rtcStr[64];
char srchStr[64];

xpm_t XPM_landingPage, XPM_exitCross, XPM_weekdays, XPM_controlBar, XPM_searchButton, XPM_searchBox, XPM_name, XPM_time;
xpm_t XPM_january, XPM_february, XPM_march, XPM_april, XPM_may, XPM_june; // camelCase please!
xpm_t XPM_july, XPM_august, XPM_september, XPM_october, XPM_november, XPM_december;
xpm_t XPM_monday, XPM_tuesday, XPM_wednesday, XPM_thursday, XPM_friday, XPM_saturday, XPM_sunday;

void buildPath(char* tpath, char* cat){
	strcat(tpath, cat);
}
void load_xpms(char* folder){
	strcpy(rtcStr, "waiting for RTC interrupt");
	strcpy(srchStr, "please press the RESET FIELD button");

	unsigned int i;
	int pos=-1;
	char path[512];
	char tPath[512];
	strcpy(path, folder);

	for(i=0;i<strlen(path);i++){
		if(path[i]=='/')
			pos=i;
	}

	if(pos==-1){
		puts("the impossible happened!\n"); exit(-59);
	}else{
		path[pos]='\0';
	}
	strcat(path, "/xpms");

	printf("reading xpms.\n");
	int c=0;

	strcpy(tPath, path); buildPath(tPath, "/minCAL.xpm");	 				XPM_name=read_xpm_from_file(tPath);			c++;
	strcpy(tPath, path); buildPath(tPath, "/tempo.xpm"); 					XPM_time=read_xpm_from_file(tPath);			c++;
	strcpy(tPath, path); buildPath(tPath, "/search_box.xpm"); 				XPM_searchBox=read_xpm_from_file(tPath);	c++;
	strcpy(tPath, path); buildPath(tPath, "/search_button.xpm");			XPM_searchButton=read_xpm_from_file(tPath);	c++;
	strcpy(tPath, path); buildPath(tPath, "/control_bar.xpm"); 				XPM_controlBar=read_xpm_from_file(tPath);	c++;
	strcpy(tPath, path); buildPath(tPath, "/exit_cross.xpm"); 				XPM_exitCross=read_xpm_from_file(tPath);	c++;
	strcpy(tPath, path); buildPath(tPath, "/landing_page.xpm"); 			XPM_landingPage=read_xpm_from_file(tPath);	c++;
	strcpy(tPath, path); buildPath(tPath, "/weekdays.xpm"); 				XPM_weekdays=read_xpm_from_file(tPath);		c++;

	strcpy(tPath, path); buildPath(tPath, "/months/january.xpm"); 			XPM_january=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/months/february.xpm"); 			XPM_february=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/months/march.xpm"); 			XPM_march=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/months/april.xpm"); 			XPM_april=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/months/may.xpm"); 				XPM_may=read_xpm_from_file(tPath);			c++;
	strcpy(tPath, path); buildPath(tPath, "/months/june.xpm"); 				XPM_june=read_xpm_from_file(tPath);			c++;
	strcpy(tPath, path); buildPath(tPath, "/months/july.xpm");	 			XPM_july=read_xpm_from_file(tPath);			c++;
	strcpy(tPath, path); buildPath(tPath, "/months/august.xpm"); 			XPM_august=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/months/september.xpm"); 		XPM_september=read_xpm_from_file(tPath);	c++;
	strcpy(tPath, path); buildPath(tPath, "/months/october.xpm"); 			XPM_october=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/months/november.xpm"); 			XPM_november=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/months/december.xpm"); 			XPM_december=read_xpm_from_file(tPath);		c++;

	strcpy(tPath, path); buildPath(tPath, "/monthviews/monday.xpm"); 		XPM_monday=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/monthviews/tuesday.xpm"); 		XPM_tuesday=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/monthviews/wednesday.xpm"); 	XPM_wednesday=read_xpm_from_file(tPath);	c++;
	strcpy(tPath, path); buildPath(tPath, "/monthviews/thursday.xpm"); 		XPM_thursday=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/monthviews/friday.xpm"); 		XPM_friday=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/monthviews/saturday.xpm"); 		XPM_saturday=read_xpm_from_file(tPath);		c++;
	strcpy(tPath, path); buildPath(tPath, "/monthviews/sunday.xpm"); 		XPM_sunday=read_xpm_from_file(tPath);		c++;


	printf("%d xpms read.\n", c);
	//init call

	cal.year=2018;
	cal.month=1;
	cal.daysInTheMonth=31;
	cal.xpm=&XPM_january;
	printf("returned 0x%x\n", XPM_controlBar.pointer);
}

void start_listening(){
	printf("a");
	/* Subscribes to timer */
	int timer0_ret = timer0_subscribe_int();
	if(timer0_ret!=0){
		fprintf(stderr, "Couldn't subscribe to IRQ_0!\n");exit(-3);
	}

	/* Subscribes to rtc */
	int rtc_ret = rtc_subscribe_int();
	if(rtc_ret!=0){
		fprintf(stderr, "Could not subscribe interruptions for the kbc!\n");exit(-1);
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

	char CTRL_status=0;

	printf("Entering the loop.\n");
	while(!stop) {
		/*Get a request message.*/
		if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			int irq_kbdset=0; 		irq_kbdset |= BIT(0); 		// 0 as in the kbc_hookIDs[0].
			int irq_timer0=0; 		irq_timer0 |= BIT(1); 		// 1 as in the timer0_hookIDs[0].
			int irq_mouseset=0; 	irq_mouseset |= BIT(2); 	// 2 as in the mouse_hookIDs[0].
			int irq_rtcset=0; 		irq_rtcset |= BIT(4) ; 		// 4 as in the rtc_hookIDs[0].

			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_timer0) {
					//printf("handling timer0\n");
					timer0_int_handler();
				}

				if (msg.NOTIFY_ARG & irq_kbdset) {
					//printf("handling keyboard\n");
					landing=0;
					KEY_PRESS* kp;
					kp=kbd_int_handler();
					if(kp==NULL)
						continue;
					if(search){
						if(kp->code==0x81){//s
							search=0; continue;
						}else if(kp->code==0x1c){//return/enter
							search=interpretSrchStr();
						}else{
							textInput(kp);
						}
					}else{
						if(kp->code==0x81){
							stop=1; continue;
						}
						if(kp->code==0x4d){ //right
							if(CTRL_status)
								nextYear(&cal);
							else
								nextMonth(&cal);
						}
						if(kp->code==0x4b){ //left
							if(CTRL_status)
								prevYear(&cal);
							else
								prevMonth(&cal);
						}
						if(kp->code==0x1f){ //s
							search=1;
						}
						if(kp->code==0x1d || kp->code==0x9d){ //CTRL
							if(kp->mk){
								//makecode
								CTRL_status=1;
							}else{
								CTRL_status=0;
							}
						}
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
					if(ma->lmb == 1) {
						landing=0;
						free(ma);
						continue;
					}
					if(ma->z > 0){
						if(CTRL_status)
							nextYear(&cal);
						else
							nextMonth(&cal);
					}else if(ma->z < 0){
						if(CTRL_status)
							prevYear(&cal);
						else
							prevMonth(&cal);
					}
					free(ma);
				}

				if (msg.NOTIFY_ARG & irq_rtcset) {
					rtc_time_t* rtc;
					rtc = rtc_int_handler();
					sprintf(rtcStr, "%02d:%02d:%02d - %02d/%02d/20%02d", rtc->hour, rtc->min, rtc->sec, rtc->day, rtc->month, rtc->year);
					free(rtc);
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
		fprintf(stderr, "Could not unsubscribe from IRQ_1.\n");	exit(-6);
	}

	if(kbd_mouse_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_12.\n");exit(-7);
	}

//	//clear mouse buffer
//	sys_outb(0x64, 0x20);
//	sys_outb(0x60, 0x47);

	if(rtc_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_8.\n");	exit(-8);
	}

	printf("\nunsubscribed successfully.\n");

	return;
}


void textInput(KEY_PRESS* kp){
	if(kp->bk)
		return;
	if(kp->code>=2 && kp->code<=12){
		unsigned len = strlen(srchStr);
		srchStr[len]=kbCodes[(int) kp->code];
		srchStr[len+1]='\0';
	}else if(kp->code==0x0e){ // backspace
		unsigned len = strlen(srchStr);
		if(len!=0)
			srchStr[len-1]='\0';
	}else if(kp->code==0x35){ // slash. quick fix because of pt keyboard.
		unsigned len = strlen(srchStr);
		srchStr[len]='-';
		srchStr[len+1]='\0';
	}else{
		//ignore
	}
}

int min(int a, int b){
	if(a<b){return a;}
	return b;
}

int max(int a, int b){
	if(a<b){return b;}
	return a;
}

/**
 * @note Make sure you clear the screen before calling this.
 */
void draw_main_page(){
	//printf("drawing main page\n");
	video_info_t vi;
	vi = get_vi();
	int i;
	int j;
	for(i = 0;i<vi.x;i++){
		for(j=0;j<100;j++){
			setP(i,j,white_c);
		}
	}
	draw_xpm_from_memory(XPM_name, 0, 0);
	draw_xpm_from_memory(XPM_time, 724, 0);
	draw_xpm_from_memory(XPM_exitCross, 650, 20);
	drawMonthName(&cal, 300, 90);
	draw_xpm_from_memory(XPM_weekdays, 300, 140);
	draw_xpm_from_memory(XPM_controlBar, 300, 480);
	draw_xpm_from_memory(XPM_searchButton, 425, 570);
	drawMonth(&cal, 300, 190);
	unsigned short len=get_string_width(rtcStr);
	draw_string(rtcStr, 737+274/2-len/2, 58, red_c);

}

int interpretSrchStr(){
	int year, month;
	if(sscanf(srchStr, "%d-%d", &month, &year)==2 && month>0 && month<=12 && year>0){
		// success
		cal.year=year-1;
		cal.month=month;
		nextYear(&cal);
		return 0;
	}else{
		return -1;
	}
}

void draw_landing_page(){
	draw_xpm_from_memory(XPM_landingPage, 0, 0);
	return;
}

void draw_search_box(){
	draw_xpm_from_memory(XPM_searchBox, 120, 220);
	draw_xpm_from_memory(XPM_exitCross, 812, 228);
	draw_string(srchStr, 180, 308, red_c);
	if(search==-1){
		draw_string("The string could not be recognized!", 180, 408, red_c);
	}
	return;
}

const unsigned char kbCodes[13]={'.', '.', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-'};

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

