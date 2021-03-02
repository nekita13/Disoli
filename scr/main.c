#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h> 

#include <vita2d.h>

#define SCREEN_W 960
#define SCREEN_H 544

#define BLACK RGBA8(0, 0, 0, 255)
#define LIME RGBA8(0, 255, 20, 255)
#define RED RGBA8(255, 0, 0, 255)
#define BLUE RGBA8(0, 0, 255, 255)
#define WHITE RGBA8(255, 255, 255, 255)
#define lerp(value, from_max, to_max) ((((value*10) * (to_max*10))/(from_max*10))/10) // это из екземпла, для привильного соотношения нажатий и положения пикселей)) (надеюсь что я не забуду зачем это говно)
/*
 * Symbol of the image.png file
 */
int firstMovingX = 420;

int endDraw = 30;



char img_file[12][30] = {"ux0:data/VitaPad/12.png" , "ux0:data/VitaPad/11.png", "ux0:data/VitaPad/8.png","ux0:data/VitaPad/7.png" , "ux0:data/VitaPad/9.png","ux0:data/VitaPad/7.png","ux0:data/VitaPad/12.png","ux0:data/VitaPad/11.png", "ux0:data/VitaPad/10.png","ux0:data/VitaPad/11.png","ux0:data/VitaPad/12.png", };// сюда надо пихатть пути к пикчам серваков
char server_name[40][30] = {"lol" , "какойто сервер" , "Steamquail", "llllooolllliii", "godot" };//имена серверов
int serv_list[50]= { 0, 1, 2, 3, 4} ; // не помню зачем))
char channel[30][30] = {"чатик", "loli", "animated", "для русских", "фурри", "не лезь", "мемы" }; // названия аналов
struct lol{// структра с сообщениями
      char name[18];
      int id; 
	  char massage[99];
	  char time[20];
} ;
struct lol op[]  = { "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20"} ;

vita2d_pgf *pgf;
vita2d_pvf *pvf;
vita2d_texture *img[5] = {NULL, NULL, NULL, NULL, NULL};
vita2d_texture *phone ;
SceCtrlData ctrl;
int touchs[2];
int massuge;
int can = 0;

// новая, Правильная эра
struct chel{// это просто для удобства
	char name[30];
	int id;
	char pikch[30];
};
struct channels	
{	
	char name[30];
	int id;
	char type[30];
};
struct masseges
{
	char name[30];
	char nickn[30];
	int msgid;
	int usrid;
	char content[30];
	char time[30];
};
struct mark { // это маркеры сгрупированные для удобства
	int tap;
	int buttonTap;
	int scrollX;
	int scrollY;
	int drawMode;

} ;
struct mark mark;
struct chel men[10] = {
	{"nikita" , 1 , "ux0:data/VitaPad/kot32.png"},
	{"andrei" , 2 , "ux0:data/VitaPad/kot64.png"},
	{"degroid228" , 3 ,  "ux0:data/VitaPad/8.png"},
	{"simpl" , 4 , "ux0:data/VitaPad/7.png"}
};

struct masseges mes[] = {
	{"men[0].name" , "s" , 1 , 0, "32" , "10:23"}, 
	{"kot" , "s" , 1 , 1 , "64" , "10:23"},
	{"name" , "s" , 1 , 0, "hello men" , "10:23"}, 
	{"name" , "s" , 1 , 0, "hello men" , "10:23"}, 
	{"name" , "s" , 1 , 1 , "hello ginro" , "10:23"},
	{"name" , "s" , 1 , 0, "hello men" , "10:23"},
	{"name" , "s" , 1 , 0, "hello men" , "10:23"}, 
	{"name" , "s" , 1 , 1 , "hello ginro" , "10:23"},
	{"name" , "s" , 1 , 0, "hello men" , "10:23"},
	{"name" , "s" , 1 , 1 , "hello ginro" , "10:23"}
}; 



int  getTach(){
	
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_BACK);
	SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];
	sceTouchRead(0, &touch[0], 1);
	

	if (touch[0].reportNum > 0) {
			/* Front touchscreen: 1920x1088 */
			touchs[0] = lerp( touch[0].report[0].x, 1920, 960);
			touchs[1] = lerp( touch[0].report[0].y, 1088, 544);
		}
		else{
			touchs[0] = -500;
			touchs[1] = 0;
			mark.tap = 0;
		}
	return 1;
}

int cannalBlok( int cnl){ // это блок который находится слева
	int i=0;
	int endDraw= 100;
	vita2d_draw_rectangle(0, 0, 300, 900, RGBA8(43, 43, 43, 150));
	vita2d_draw_texture(img[1] , 7, 10);

	while (i <5){	
		vita2d_pgf_draw_textf(pgf, 20, endDraw, RGBA8(178, 178, 178, 225), 1.0f , "# %s    %d", channel[i], endDraw );
		if (cnl == i ){
				vita2d_pgf_draw_textf(pgf, 20, endDraw, WHITE, 1.0f , "# %s    %d", channel[i], endDraw );
		}
		
		if (touchs[1] > endDraw -17 && touchs[1] < endDraw +33 && touchs[0] < 400 && touchs[0]> 0 && mark.tap == 0){// обработка нажатий и изменение канала
			massuge =i;
			mark.tap = 1;
			mark.scrollX = 0;
		}
		endDraw += 50;
		i++;
		//if (canals[i] == 0) break;
	}
	vita2d_pgf_draw_textf(pgf, 75, 32, WHITE, 1.3f , "%s", server_name[can]);
	return 1 ;

}

int viewMassage(int cnl){
	int i = 0;
	endDraw = 360;
	int n = sizeof(mes)/sizeof(mes[0]);
	cannalBlok(cnl);
	for(struct masseges *p=mes; p < mes+n; p++){

		vita2d_draw_texture(img[p->usrid], 310 , endDraw + mark.scrollX - 14);
		vita2d_pgf_draw_textf(pgf, 380, endDraw + mark.scrollX, WHITE, 1.3f , " %s", p->name );	
		vita2d_pgf_draw_textf(pgf, 380, endDraw + 26 + mark.scrollX, WHITE , 1.0f , " %s   %d", p->content, mark.scrollX );	
		vita2d_pgf_draw_textf(pgf, 480, endDraw + mark.scrollX, WHITE, 0.8f , " %s", p->time );	
		endDraw -= 80;
		i++;
	}
	if (mark.scrollX > 800 ) {
		vita2d_draw_rectangle(890, 400, 40, 40, RGBA8(43, 43, 43, 100));
		if (touchs[1] > 400 && touchs[1] < 440 && touchs[0] < 930 && touchs[0]> 890 && mark.tap == 0) {
			mark.tap= 1;
			mark.scrollX = 0; 
		}

	}
	
	return 1;
}

int viewCanall(int id){
	int i =0 ;
	endDraw = 30 +30 + mark.scrollX;
	vita2d_draw_texture( img[id], 20, 40);
	vita2d_pgf_draw_textf(pgf, 40, endDraw, WHITE, 1.0f , "# %s", server_name[id] );	
	while (i <5){	
		vita2d_pgf_draw_textf(pgf, 170, endDraw, WHITE, 1.0f , "# %s    %d", channel[i], endDraw );
		
		if (touchs[1] > endDraw -17 && touchs[1] < endDraw +33 && touchs[0] < 400 && touchs[0]> 0 && mark.tap == 0){
			massuge =i;
			mark.tap = 1;
			mark.scrollX = 0;
			return 2;

			break;
		}
		endDraw += 50;
		i++;
		//if (canals[i] == 0) break;
	}
	

	return 1;
}	

int viewServer(int id,  int plase ){
	
	
	plase -= 130 ;

	vita2d_draw_texture(img[1], 20, plase + mark.scrollX);
    vita2d_pgf_draw_textf(pgf, 150, plase+ 80 + mark.scrollX, WHITE, 1.5f, "%s   %d", server_name[id], plase);

	return plase;
}

int mode = 0;
int control(){

	sceCtrlPeekBufferPositive(0, &ctrl, 1);
	if (ctrl.buttons == (SCE_CTRL_CIRCLE ) && mark.drawMode > 0 ) {
		if (mark.buttonTap == 0) mark.drawMode -= 1;
	  	mark.buttonTap = 1 ;
	}else mark.buttonTap = 0;// это выходы из подменюшек
	if (ctrl.ly <110 || ctrl.ly > 140){
		mark.scrollX += lerp((ctrl.ly - 115), 255- 115 , mode) ;

	} 
	if (ctrl.buttons == (SCE_CTRL_UP )  ) mark.scrollX += 1;// скрол, потом сдеалю на сосок
	if (ctrl.buttons == (SCE_CTRL_DOWN )  ) mark.scrollX -= 1;
	if (ctrl.buttons == (SCE_CTRL_LEFT )  ) mode += 1;// скрол, потом сдеалю на сосок
	if (ctrl.buttons == (SCE_CTRL_RIGHT )  ) mode -= 1;

	return 1; 
} 

int main(){
	
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	pgf = vita2d_load_default_pgf();
	pvf = vita2d_load_default_pvf();
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	phone  = vita2d_load_PNG_file("ux0:data/VitaPad/phone.png"); //это импорт и загрузка фона в память 
	int texturi = 5;
	while (texturi != 0) // это загрузка текстур в память в переменную img
	{	--texturi;
		char *  hil = men[texturi].pikch ;
		img[texturi] = vita2d_load_PNG_file(hil);
		
	}
	
	mark.drawMode = 2;
	massuge = 1;
	int	 i=0;

	while (1) {
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		vita2d_draw_texture(phone  , 0,0 ); // рисуем фон
		getTach();
		control();
		endDraw=550;
		switch (mark.drawMode){ //тута мы смотрим что рисовать и рисуем
			case 0: // список серверов
				i = 4;
				while (i > -1 ){
					endDraw = viewServer(i, endDraw);
					
					if (touchs[1] > endDraw && touchs[1] < endDraw +130 && touchs[0] < 400 && touchs[0]> 0 && mark.tap == 0){
						can =i;
						mark.drawMode = 1;
						mark.tap =1;
						mark.scrollX = 0;
						break;
					}
					i--;
				}
			break;
			
			case 1 :// список каналов хотя возможно стоит бьединить последнии два пункта
				mark.drawMode = viewCanall(can); // в принципе она нахер не нужна
			break;

			case 2 :// сообщения
				viewMassage(massuge);
			break;
			case 228: // предупреждение о ошибке)) 
				vita2d_pgf_draw_textf(pgf, 0, 200, RED, 2.0f , "програмист гдето обосрался  (; ");	
				vita2d_pgf_draw_textf(pgf, 0, 300, LIME, 2.0f , "жмыхни по экрану шоб вернутся ");	
				if (touchs[0] >0) mark.drawMode =0;
			break;
			default:
				mark.drawMode = 228;
			break;
		}
		


        //vita2d_pvf_draw_textf(pvf , 80, 130, RED , 1.0f, "while working  %d %d clearInput %d %d %d btap %d" , mark.scrollX , mode, ctrl.ly , (ctrl.ly - 115), mark.drawMode, mark.buttonTap);
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	// конец рендера и выход из программы
	vita2d_fini();
	vita2d_free_texture(img[0]);
	vita2d_free_pgf(pgf);
	vita2d_free_pvf(pvf);

	sceKernelExitProcess(0);
	return 0;
}
