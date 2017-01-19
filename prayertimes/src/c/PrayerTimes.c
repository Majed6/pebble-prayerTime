#include <pebble.h>
#include <math.h>
#include <stdio.h>
static Window *s_main_window;
static TextLayer *s_prayers_tLayer;
double SAC[2];// contains D[0] and Eqt[1]  
int Fajr[4],Dhuhr[4],Asr[4],Maghrib[4],Isha[4];
char text[] = "Prayer times are:\nFajr:00:00\nDhuhr:00:00\nAsr:00:00\nMaghrib:00:00\nIsha:00:00";//text to be printed in screen


void calcPrayers(double TimeZone ,double L,double Lng );
static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init();
static void deinit();




int main(void) {
  init();
  app_event_loop();
  deinit();
}




//---------------------- Essentials ----------------------

static void main_window_load(Window *window) {
    // Create time TextLayer GRect(x,y,width,height)
  s_prayers_tLayer = text_layer_create(GRect(0, 55, 144, 100));
  text_layer_set_background_color(s_prayers_tLayer, GColorClear);
  text_layer_set_text_color(s_prayers_tLayer, GColorBlack);
  
  calcPrayers(3,24.6333,46.7167);
  text_layer_set_text(s_prayers_tLayer, text); //text to be printed 
    // Improve the layout to be more like a watchface
  text_layer_set_font(s_prayers_tLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_prayers_tLayer, GTextAlignmentCenter);
   // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_prayers_tLayer));

}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_prayers_tLayer);
}




static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}



//---------------------- Sun's angular coordinates ----------------------

void SunsAngularCoordinates(){
  double d,g,q,L,e,RA;
  //------------------ Start of Calc JD ------------------
  int JD,a,y,m,JDN;
    // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  a=(14-tick_time->tm_mon)/12;
  y=tick_time->tm_year+4800-a;
  m=tick_time->tm_mon+(12 * a)-3;
  JDN=tick_time->tm_mday+(((153 * m)+2)/5)+(365 * y)+(y/4)-32083;
  JD=JDN+((tick_time->tm_hour-12)/24)+(tick_time->tm_min/1440)+(tick_time->tm_sec/86400);
  //------------------ End of Calc JD ------------------

  
  d = JD - 2451545.0;   

  g = 357.529 + 0.98560028 * d;
  q = 280.459 + 0.98564736 * d;
  L = q + 1.915 * sin(g) + 0.020 * sin(2 * g);
  
  //R = 1.00014 - 0.01671* cos(g) - 0.00014* cos(2*g); //The distance of the Sun from the Earth, R, in astronomical units (AU)
  e = 23.439 - 0.00000036 * d;
  RA = atan2(cos(e) * sin(L), cos(L))/ 15;

  //SAC[0] = asin(sin(e) * sin(L)); // declination of the Sun
  SAC[1] = q/15 - RA;  // equation of time
}


//---------------------- Calculating Prayer Times ----------------------
void calcPrayers(double TimeZone ,double L,double Lng ){
  //SunsAngularCoordinates();
  //----------------- Dhuhr -----------------------
  double DP=12+TimeZone-Lng/15-SAC[1];
  Dhuhr[0]=DP/10;
  Dhuhr[1]=((int)DP)%10;
  Dhuhr[2]=(((((int)(DP * 100))/10)/100)*60)/10;
  Dhuhr[3]=(((((int)(DP * 100))%10)/100)*60)%10;
    
    
  //----------------- To add Prayer Use: -----------------------
  text[35]=Dhuhr[0]+48;
  text[36]=Dhuhr[1]+48;
  text[38]=Dhuhr[2]+48;
  text[39]=Dhuhr[3]+48;
  

  //----------------- Prayer added -----------------------------
  
  
}





