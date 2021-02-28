#include <stdio.h>
#include <string.h>

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
int drawMode = 2;
struct lol op[]  = { "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20"} ;

vita2d_pgf *pgf;
vita2d_pvf *pvf;
vita2d_texture *img[5] = {NULL, NULL, NULL, NULL, NULL};
int scrol = 0;
int touchs[2];
int massuge;
int tap;


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
 
struct chel men[10] = {
	{"nikita" , 1 , "ux0:data/VitaPad/kot32.png"},
	{"andrei" , 2 , "ux0:data/VitaPad/kot64.png"},
	{"degroid228" , 3 ,  "ux0:data/VitaPad/8.png"},
	{"simpl" , 4 , "ux0:data/VitaPad/7.png"}
};

struct masseges mes[] = {
	{"kot" , "s" , 1 , 0, "32" , "10:23"}, 
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
			tap = 0;
		}
	return 1;
}

int viewMassage(int cnl){
	int i = 0;
	endDraw = 360;
	int n = sizeof(mes)/sizeof(mes[0]);
	vita2d_pgf_draw_textf(pgf, 700, 70, BLUE, 1.3f , " %s", channel[cnl]);	

	for(struct masseges *p=mes; p < mes+n; p++){

		vita2d_draw_texture(img[p->usrid], 313, endDraw + scrol);
		vita2d_pgf_draw_textf(pgf, 380, endDraw + scrol, WHITE, 1.0f , " %s", p->name );	
		vita2d_pgf_draw_textf(pgf, 380, endDraw + 26 + scrol, WHITE , 0.8f , " %s", p->content );	
		vita2d_pgf_draw_textf(pgf, 480, endDraw + scrol, WHITE, 0.5f , " %s", p->time );	
		endDraw -= 80;
		i++;
	}
	return 1;
}

int viewCanall(int id){
	int i =0 ;
	endDraw = 30 +30 + scrol;
	vita2d_draw_texture( img[id], 20, 40);
	vita2d_pgf_draw_textf(pgf, 40, endDraw, WHITE, 1.0f , "# %s", server_name[id] );	
	while (i <5){	
		vita2d_pgf_draw_textf(pgf, 170, endDraw, WHITE, 1.0f , "# %s    %d", channel[i], endDraw );
		
		if (touchs[1] > endDraw -17 && touchs[1] < endDraw +33 && touchs[0] < 400 && touchs[0]> 0 && tap == 0){
			massuge =i;
			tap = 1;
			scrol = 0;
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

	vita2d_draw_texture(img[1], 20, plase + scrol);
    vita2d_pgf_draw_textf(pgf, 150, plase+ 80 + scrol, WHITE, 1.5f, "%s   %d", server_name[id], plase);

	return plase;
}

int endFor = 5;

int main(){
	
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	pgf = vita2d_load_default_pgf();
	pvf = vita2d_load_default_pvf();
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData ctrl;

	int texturi = 5;
	while (texturi != 0)
	{	--texturi;
		char *  hil = men[texturi].pikch ;
		img[texturi] = vita2d_load_PNG_file(hil);
		
	}
	int can = 0;
	massuge = 1;
	int	 i=0;
	int buttonTap = 0;
	while (1) {
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		
    	endDraw=550;
		getTach();
		sceCtrlPeekBufferPositive(0, &ctrl, 1);
		if (ctrl.buttons == (SCE_CTRL_CIRCLE ) && drawMode > 0 ) {
			if (buttonTap == 0) drawMode -= 1;
		  	buttonTap = 1 ;
		}else buttonTap = 0;// это выходы из подменюшек
		if (ctrl.buttons == (SCE_CTRL_UP )  ) scrol += 1;// скрол, потом сдеалю на сосок
		if (ctrl.buttons == (SCE_CTRL_DOWN )  ) scrol -= 1;
		switch (drawMode){ //тута мы смотрим что рисовать и рисуем
			case 0: // список серверов
				i = 4;
				while (i > -1 ){
					endDraw = viewServer(i, endDraw);
					
					if (touchs[1] > endDraw && touchs[1] < endDraw +130 && touchs[0] < 400 && touchs[0]> 0 && tap == 0){
						can =i;
						drawMode = 1;
						tap =1;
						scrol = 0;
						break;
					}
					i--;
				}
			break;
			
			case 1 :// список каналов хотя возможно стоит бьединить последнии два пункта
				drawMode = viewCanall(can);
			break;

			case 2 :// сообщения
				viewMassage(massuge);
			break;
			case 228: // предупреждение о ошибке)) 
				vita2d_pgf_draw_textf(pgf, 0, 200, RED, 2.0f , "програмист гдето обосрался  (; ");	
				vita2d_pgf_draw_textf(pgf, 0, 300, LIME, 2.0f , "жмыхни по экрану шоб вернутся ");	
				if (touchs[0] >0) drawMode =0;
			break;
			default:
				drawMode = 228;
			break;
		}
		


        vita2d_pvf_draw_textf(pvf , 80, 130, RED , 1.0f, "while working надеюсь что это говно заработает %d %d %d %d %d btap %d" , touchs[0] , touchs[1], can , i, drawMode, buttonTap);
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
