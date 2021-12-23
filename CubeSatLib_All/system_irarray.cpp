#include <vector>
#include "system_irarray.h"
#include <cmath>

const std::vector<char> pixel={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const std::string grayRamp = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,""^`\'. ";

CIRArray::CIRArray(){  
  Name("IRARRAY");  
}


void CIRArray::init(){
  CSystemObject::init();
  setForever();
  setInterval(10000);
}

 void CIRArray::config( char addr, TwoWire *pWire) {
   
    _pWire=pWire;    
    _address=addr;
            
  }

void CIRArray::setup()
{
  init();
  for(int retries=0;retries<5;retries++){
  if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, _pWire)) {
    writeconsoleln("MLX90640 not found!");
    setState("ERROR");
    delay(300);
  }
  else{
    writeconsoleln("Found Adafruit MLX90640");
  
    //writeconsole("Serial number: ");
    //writeconsole(mlx.serialNumber[0], HEX);
    //writeconsole(mlx.serialNumber[1], HEX);
    //writeconsoleln(mlx.serialNumber[2], HEX);
    
    mlx.setMode(MLX90640_INTERLEAVED);
    //mlx.setMode(MLX90640_CHESS);
    //writeconsole("Current mode: ");
    if (mlx.getMode() == MLX90640_CHESS) {
      //writeconsoleln("Chess");
    } else {
      //writeconsoleln("Interleave");    
    }
  
    mlx.setResolution(MLX90640_ADC_18BIT);
    //writeconsole("Current resolution: ");
    mlx90640_resolution_t res = mlx.getResolution();
    switch (res) {
      case MLX90640_ADC_16BIT: writeconsoleln("16 bit"); break;
      case MLX90640_ADC_17BIT: writeconsoleln("17 bit"); break;
      case MLX90640_ADC_18BIT: writeconsoleln("18 bit"); break;
      case MLX90640_ADC_19BIT: writeconsoleln("19 bit"); break;
    }
  
    mlx.setRefreshRate(MLX90640_2_HZ);
    //writeconsole("Current frame rate: ");
    mlx90640_refreshrate_t rate = mlx.getRefreshRate();
    switch (rate) {
      case MLX90640_0_5_HZ: writeconsoleln("0.5 Hz"); break;
      case MLX90640_1_HZ: writeconsoleln("1 Hz"); break; 
      case MLX90640_2_HZ: writeconsoleln("2 Hz"); break;
      case MLX90640_4_HZ: writeconsoleln("4 Hz"); break;
      case MLX90640_8_HZ: writeconsoleln("8 Hz"); break;
      case MLX90640_16_HZ: writeconsoleln("16 Hz"); break;
      case MLX90640_32_HZ: writeconsoleln("32 Hz"); break;
      case MLX90640_64_HZ: writeconsoleln("64 Hz"); break;
    }
  
    setState("PLAY");
    return;
  }
}

}

void CIRArray::loop(){
CMsg m;
//runOnce(m);

}

void CIRArray::consoleOut(){
for (uint8_t h=0; h<24; h++) {
  for (uint8_t w=0; w<32; w++) {
    float t = frame[h*32 + w];
    #ifdef PRINT_TEMPERATURES
          writeconsole(t, 1);
          writeconsole(", ");
    #endif

    #ifdef PRINT_ASCIIART
    char c = '&';
    if (t < 20) c = ' ';
    else if (t < 23) c = '.';
    else if (t < 25) c = '-';
    else if (t < 27) c = '*';
    else if (t < 29) c = '+';
    else if (t < 31) c = 'x';
    else if (t < 33) c = '%';
    else if (t < 35) c = '#';
    else if (t < 37) c = 'X';
    writeconsole(c);
    #endif
  }
  writeconsoleln("  ");
}
writeconsoleln("   ");
}

void CIRArray::fillPixelTable(){
  float y;
  int offset;
  pixelTable[768]=NULL;
  pixelTable[769]=NULL;
   
  for (int x = 0 ; x < 768 ; x++){  
    y=frame[x];
    if (y!=NULL){
      offset= (int) map(y*10, imin, imax, 0, pixel.size()-1);
      if ((offset<0)||(offset>=pixel.size())){
         pixelTable[x]='A';       
      }
       else pixelTable[x]=pixel[offset];
    }
    else pixelTable[x]='A';
  }
}

void CIRArray::fillGreyTable(){
  float y;
  int offset;
  greyTable[768]=NULL;
  greyTable[769]=NULL;
   
  for (int x = 0 ; x < 768 ; x++){  
    y=frame[x];
    if (y!=NULL){
      offset= (int) map(y*10, imin, imax, 0, pixel.size()-1);
      if ((offset<0)||(offset>=pixel.size())){
         greyTable[x]='A';       
      }
       else greyTable[x]=pixel[offset];
    }
    else greyTable[x]='A';
  }
}


void CIRArray::runOnce(CMsg &msg)
{
  std::string sOut=msg.getParameter("OUTPUT","");

  if (mlx.getFrame(frame) != 0) {
    writeconsoleln("Failed");
    
  }

  float y,lasty;

  int offset=rand()%700 +5;   //random between 7 and 705

  y=frame[offset];

  if(y<-100.0) y=0.0;
  if(y>200.0) y=0.0;

  fmin=y;
  fmax=y;
  lasty=y;
   
  for (int x = 0 ; x < 768 ; x++)    //Find min max
  {
    y=frame[x];
    if (isnan(y)){
      y=lasty;
      frame[x]=y;
    }
    
    if(y>300.0){
      y=300;
      frame[x]=y;
    }
    if(y<-200.0){
      y=-200;
      frame[x]=y;
    }
    if(y<fmin) fmin=y;
    if(y>fmax) fmax=y;

  }

  imin=(int) floor(fmin*10);
  imax=(int) ceil(fmax*10);

}




void CIRArray::Output(CMsg &msg){  //Easier to send as long   convert to decimal when receiv
  runOnce(msg);
  //fillPixelTable();
  fillGreyTable();
  
  CMsg m;
  m.setParameter("table","irarray");
  m.setParameter("panel",Name());
  m.setParameter("data",greyTable);
          
  addTransmitList(m); 
}


void  CIRArray::callCustomFunctions(CMsg &msg){
  std::string act=msg.getACT();
  
  writeconsoleln(act);
  
  if(act=="IRRUN") runOnce(msg);
 
}
