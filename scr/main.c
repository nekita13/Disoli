#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h> 
#include <psp2/ime_dialog.h> 
#include <vita2d.h>
#include "ime_dialog.h"
#include <psp2/types.h>

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
//struct lol op[]  = { "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20", "wasa" , 1 , "i the God" ,"20:19" , "sidor", 2 , "hahahahaah", "20:19","wasa", 1 , "Fuck you" ,"20:20"} ;

vita2d_pgf *pgf;
vita2d_pvf *pvf;
vita2d_texture *img[5] = {NULL, NULL, NULL, NULL, NULL};
vita2d_texture *phone ;
SceCtrlData ctrl;
int touchs[2];
int massuge;
int can = 0;
int serverid = 0;
int iserv = 960;
// новая, Правильная эра
struct servers{
	char name[50];
	int id;
	char path[50];

};
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
struct masseges{
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
	int scrollServ;
	int drawMode;
	int RMeny;
	int ls;

} ;
struct mark mark;
struct chel men[10] = {
	{"nikita" , 1 , "ux0:data/VitaPad/kot32.png"},
	{"andrei" , 2 , "ux0:data/VitaPad/kot64.png"},
	{"degroid228" , 3 ,  "ux0:data/VitaPad/kot64.png"},
	{"simpl" , 4 , "ux0:data/VitaPad/kot64.png"},
	{"sucks" , 4 , "ux0:data/VitaPad/kot34.png"}
};

struct masseges mes[50] = {
	{ "men[0].&name ", "s" , 1 , 0, "32" , "10:23"}, 
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

struct servers serv[]= {
	{"личка" , 0  , "ux0:data/VitaPad/kot32.png"}, 
	{"lokalhost" , 0 ,"ux0:data/VitaPad/12.png" },
	{"seweranka" , 1 , "ux0:data/VitaPad/11.png"},
	{"какойто сервер" ,1,"ux0:data/VitaPad/8.png"},
	{"Steamquail", 1,"ux0:data/VitaPad/7.png" },
	{"llllooolllliii", 1,"ux0:data/VitaPad/9.png"},
	{"godot" , 1,"ux0:data/VitaPad/7.png" }

};

static int ime_dialog_running = 0;

static uint16_t ime_title_utf16[SCE_IME_DIALOG_MAX_TITLE_LENGTH];
static uint16_t ime_initial_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
static uint16_t ime_input_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
static uint8_t ime_input_text_utf8[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
char *output = "2";
int imeStatus;

void utf16_to_utf8(uint16_t *src, uint8_t *dst) {
    int i;
    for (i = 0; src[i]; i++) {
        if ((src[i] & 0xFF80) == 0) {
            *(dst++) = src[i] & 0xFF;
        } else if((src[i] & 0xF800) == 0) {
            *(dst++) = ((src[i] >> 6) & 0xFF) | 0xC0;
            *(dst++) = (src[i] & 0x3F) | 0x80;
        } else if((src[i] & 0xFC00) == 0xD800 && (src[i + 1] & 0xFC00) == 0xDC00) {
            *(dst++) = (((src[i] + 64) >> 8) & 0x3) | 0xF0;
            *(dst++) = (((src[i] >> 2) + 16) & 0x3F) | 0x80;
            *(dst++) = ((src[i] >> 4) & 0x30) | 0x80 | ((src[i + 1] << 2) & 0xF);
            *(dst++) = (src[i + 1] & 0x3F) | 0x80;
            i += 1;
        } else {
            *(dst++) = ((src[i] >> 12) & 0xF) | 0xE0;
            *(dst++) = ((src[i] >> 6) & 0x3F) | 0x80;
            *(dst++) = (src[i] & 0x3F) | 0x80;
        }
    }

    *dst = '\0';
}

void utf8_to_utf16(uint8_t *src, uint16_t *dst) {
    int i;
    for (i = 0; src[i];) {
        if ((src[i] & 0xE0) == 0xE0) {
            *(dst++) = ((src[i] & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
            i += 3;
        } else if ((src[i] & 0xC0) == 0xC0) {
            *(dst++) = ((src[i] & 0x1F) << 6) | (src[i + 1] & 0x3F);
            i += 2;
        } else {
            *(dst++) = src[i];
            i += 1;
        }
    }

    *dst = '\0';
}

int initImeDialog(char *title, char *initial_text, int max_text_length, int password) {
    if (ime_dialog_running)
        return -1;

    utf8_to_utf16((uint8_t *)title, ime_title_utf16);
    utf8_to_utf16((uint8_t *)initial_text, ime_initial_text_utf16);

    SceImeDialogParam param;
    sceImeDialogParamInit(&param);

    param.supportedLanguages = 0x0001FFFF;
    param.languagesForced = SCE_TRUE;
    param.type = SCE_IME_TYPE_BASIC_LATIN;
    param.title = ime_title_utf16;
    param.maxTextLength = max_text_length;
    param.initialText = ime_initial_text_utf16;
    param.inputTextBuffer = ime_input_text_utf16;
    param.textBoxMode = password;

    int res = sceImeDialogInit(&param);
    if (res >= 0)
        ime_dialog_running = 1;

    return res;
}

int isImeDialogRunning() {
    return ime_dialog_running;	
}

uint16_t *getImeDialogInputTextUTF16() {
    return ime_input_text_utf16;
}

uint8_t *getImeDialogInputTextUTF8() {
    return ime_input_text_utf8;
}

int updateImeDialog() {
    if (!ime_dialog_running)
        return IME_DIALOG_RESULT_NONE;

    SceCommonDialogStatus status = sceImeDialogGetStatus();
    if (status == IME_DIALOG_RESULT_FINISHED) {
        SceImeDialogResult result;
        memset(&result, 0, sizeof(SceImeDialogResult));
        sceImeDialogGetResult(&result);

        if (result.button == SCE_IME_DIALOG_BUTTON_CLOSE) {
            status = IME_DIALOG_RESULT_CANCELED;
        } else {
            utf16_to_utf8(ime_input_text_utf16, ime_input_text_utf8);
        }

        sceImeDialogTerm();

        ime_dialog_running = 0;
    }

    return status;
}
// тута начинаются функции
int  getTach(){// опрашиваем сенсор и пишым значение в touchs
	
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
int dir;
int cannalBlok( int cnl){ // это блок который находится слева
	int i=0;
	int endDraw= 100;
	vita2d_draw_rectangle(0, 0, 300, 900, RGBA8(43, 43, 43, 150));
	vita2d_draw_texture(img[1] , 7, 10);

	while (i <5){	
		
		if (cnl == i ){
				vita2d_pgf_draw_textf(pgf, 25, endDraw, WHITE, 1.0f , "# %s    %d", channel[i], endDraw );
		}else vita2d_pgf_draw_textf(pgf, 20, endDraw, RGBA8(178, 178, 178, 225), 1.0f , "# %s    %d", channel[i], endDraw );
		
		if (touchs[1] > endDraw -17 && touchs[1] < endDraw +33 && touchs[0] < 400 && touchs[0]> 0 && mark.tap == 0){// обработка нажатий и изменение канала
			massuge =i;
			mark.tap = 1;
			mark.scrollX = 0;
		}
		endDraw += 50;
		i++;
		//if (canals[i] == 0) break;
	}
	dir = serverid-1;
	vita2d_pgf_draw_textf(pgf, 75, 32, WHITE, 1.3f , "%s", server_name[dir ]);
	return 1 ;

}

int lsBlock(int cnl){
	int i=0;
	int endDraw= 100;
	vita2d_draw_rectangle(0, 0, 300, 900, RGBA8(43, 43, 43, 150));

	while (i <4 ){	
		vita2d_draw_texture(img[i] , 7, endDraw - 17);

		if (cnl == i ){
			vita2d_pgf_draw_textf(pgf, 76, endDraw, WHITE, 1.0f , "%s", men[i].name);
		}else vita2d_pgf_draw_textf(pgf, 70, endDraw, RGBA8(178, 178, 178, 225), 1.0f , "%s", men[i].name );

		if (touchs[1] > endDraw -17 && touchs[1] < endDraw +33 && touchs[0] < 300 && touchs[0]> 0 && mark.tap == 0){// обработка нажатий и изменение канала
			massuge =i;
			mark.tap = 1;
			mark.scrollX = 0;
		}
		endDraw += 50;
		i++;
		//if (canals[i] == 0) break;
	}

	vita2d_pgf_draw_textf(pgf, 0, 32, WHITE, 1.1f , "личные сообщения");
	return 1 ;
}

int drawInput(){ // ввод сообщений, после отправки прировнять output к "", шоб очистить окно
	vita2d_draw_rectangle(300, 480, 900, 100, RGBA8(43, 43, 43, 230));
	vita2d_pgf_draw_textf(pgf, 360, 544 - 20, WHITE, 1.3f , " %s", output );	

	vita2d_draw_rectangle(870, 480, 200, 250, RGBA8(43, 43, 43, 250));

	if (touchs[1] > 444 && touchs[1] < 544 && touchs[0] < 860 && touchs[0]> 300 && mark.tap == 0 && mark.RMeny != 1){// открытие окна ввода
		initImeDialog("input you massege" , output , 212, 0); 
	}
	imeStatus = updateImeDialog();
	if(imeStatus == IME_DIALOG_RESULT_FINISHED){
		output = (char *)getImeDialogInputTextUTF8();			
	}
	if (touchs[1] > 480 && touchs[1] < 544 && touchs[0] < 960 && touchs[0]> 860 && mark.tap == 0 && mark.RMeny != 1){
		// отправить сообщение
	}
	return 1 ;
}

int viewServer(int draw){// менюшка которая вылазт справа и показывает сервера
	int endDraw = 50 ;
	int i = 0;
	int n = sizeof(serv)/sizeof(serv[0]);
	vita2d_draw_rectangle( draw , 0 ,700 , 600, RGBA8(43, 43, 43, 200));

	for(struct servers *p=serv; p < serv+n; p++){
		vita2d_draw_texture(img[p->id], draw + 10 , endDraw + mark.scrollServ - 17 );
		vita2d_pgf_draw_textf(pgf, draw + 80, endDraw + mark.scrollServ + 5, RGBA8(178, 178, 178, 225), 1.3f , " %s", p->name );	
		if (i == serverid) vita2d_pgf_draw_textf(pgf, draw + 80, endDraw + mark.scrollServ +5 , WHITE, 1.3f , " %s", p->name );	

		if (touchs[1] > endDraw + mark.scrollServ -17 && touchs[1] < endDraw + mark.scrollServ +40 && touchs[0] > 300 && touchs[0]< 960 && mark.tap == 0){
			serverid =i;
			mark.tap = 1;
			mark.scrollX = 0;
			if (serverid == 0) {
				mark.ls = 1 ; 
			}else mark.ls = 0;

		}
		 endDraw += 80;
		i++;
	}
	vita2d_draw_rectangle( draw , 0 ,700 , 40, RGBA8(43, 43, 43, 255));
	vita2d_pgf_draw_textf(pgf, draw + 20, 30, WHITE, 1.3f , "Servers");
	return 1;
}

int viewMassage(int cnl){// основная вызываемая функция, отоброжает сообщения и каналы
	int i = 0;
	endDraw = 360;
	int n = sizeof(mes)/sizeof(mes[0]);
	
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
	
	if (mark.ls == 1 )lsBlock(cnl); else cannalBlok(cnl);
	
	drawInput();
	if (mark.RMeny == 1 ) {// анимация выезда менюшки
		if (iserv > 300) iserv-= 10;
		if (iserv > 300) iserv-= 10;
		if (iserv > 300) iserv-= 10;
		if (iserv > 300) iserv-= 10;
		if (iserv > 300) iserv-= 10;
		if (iserv > 300) iserv-= 10;
		if (iserv > 300) iserv-= 10;
		if (iserv > 300) iserv-= 10;

		viewServer(iserv);
	}
	
	return 1;
}

/* int viewCanall(int id){ 
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
*/

int init(){
	int i = 0;
	int n = sizeof(men)/sizeof(men[0]);
	for(struct chel *p=men; p < men+n; p++){
	strcpy( mes[i].name , p->name);
		i++;
	}

	return 1;
}


int mode = 20;
int control(){// обновление буфера нажатий и проверка кнопок

	sceCtrlPeekBufferPositive(0, &ctrl, 1);
	if (ctrl.buttons == 0) mark.buttonTap = 0;
	if (ctrl.buttons == (SCE_CTRL_CIRCLE ) && mark.RMeny == 1 ) {
		if (mark.buttonTap == 0) mark.RMeny = 0;
	  	mark.buttonTap = 1 ;
	} // это выходы из подменюшек
	if (ctrl.ly <110 || ctrl.ly > 140){
		if (mark.RMeny == 1 ) mark.scrollServ += lerp((ctrl.ly - 115), 255- 115 , mode) ;
		else mark.scrollX += lerp((ctrl.ly - 115), 255- 115 , mode) ;

	} 
	if (ctrl.buttons == (SCE_CTRL_RTRIGGER ) && mark.buttonTap == 0  ) {
		if (mark.RMeny ==0 ) mark.RMeny = 1 ;else{mark.RMeny=0 ; iserv = 960;}	
		mark.buttonTap = 1; 
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
	
	while (1) {
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		vita2d_draw_texture(phone  , 0,0 ); // рисуем фон
		getTach();
		control();
		endDraw=550;
		
		switch (mark.drawMode){ //тута мы смотрим что рисовать и рисуем
			case 0: // список серверов
				endDraw = viewServer(iserv);

			break;
			
			case 1 :// список каналов хотя возможно стоит бьединить последнии два пункта
				//mark.drawMode = viewCanall(can); // в принципе она нахер не нужна СОГЛАСЕН

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
		

		
        //vita2d_pvf_draw_textf(pvf , 80, 130, RED , 1.0f, "while working  %s touch[1] %d touch[0] %d" , output, touchs[1], touchs[0]);
		vita2d_end_drawing();
		vita2d_common_dialog_update();
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
