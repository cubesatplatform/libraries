
#include <vector>
#include "system_irarray.h"
#include <cmath>

const std::vector<char> pixel={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const std::string greyRamp = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+=<>i!lI;,""^`\'. ";

CIRArray::CIRArray(){  
  Name("IRARRAY");  
}


void CIRArray::init(){
  CSystemObject::init();
  setInterval(10000);
  setErrorThreshold(3); 
}

 void CIRArray::config( char addr, TwoWire *pWire) {
   
    _pWire=pWire;    
    _address=addr;    
  }


void CIRArray::config(CMsg &msg){
  std::string straddress=msg.getParameter("ADDRESS");
  std::string strwire=msg.getParameter("WIRE");

  TwoWire *pWire;

  if(strwire=="Wire") pWire=&Wire;  
  if(strwire=="Wire1") pWire=&Wire1;  
  if(strwire=="Wire2") pWire=&Wire2;  

  if(straddress.size()>0){
    config(straddress[0],pWire);
  }
}

void CIRArray::setup()
{
  CMsg m;
  init();          

  for(int retries=0;retries<5;retries++){
  if (! mlx.begin(_address, _pWire)) {
     incErrorCount();      
     delay(30);
    }
  else{
    writeconsoleln("Found MLX90640");
  
    
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
      case MLX90640_ADC_16BIT: m.setINFO("16 bit"); break;
      case MLX90640_ADC_17BIT: m.setINFO("17 bit"); break;
      case MLX90640_ADC_18BIT: m.setINFO("18 bit"); break;
      case MLX90640_ADC_19BIT: m.setINFO("19 bit"); break;
      }
  
    mlx.setRefreshRate(MLX90640_2_HZ);
    //writeconsole("Current frame rate: ");
    mlx90640_refreshrate_t rate = mlx.getRefreshRate();
    switch (rate) {
      case MLX90640_0_5_HZ: m.setCOMMENT("0.5 Hz"); break;
      case MLX90640_1_HZ: m.setCOMMENT("1 Hz"); break; 
      case MLX90640_2_HZ: m.setCOMMENT("2 Hz"); break;
      case MLX90640_4_HZ: m.setCOMMENT("4 Hz"); break;
      case MLX90640_8_HZ: m.setCOMMENT("8 Hz"); break;
      case MLX90640_16_HZ: m.setCOMMENT("16 Hz"); break;
      case MLX90640_32_HZ: m.setCOMMENT("32 Hz"); break;
      case MLX90640_64_HZ: m.setCOMMENT("64 Hz"); break;
      }
    writeconsoleln(m.serializeout());
    setState("PLAY");
    return;
    }
  }
  sendError();
}

void CIRArray::test(CMsg &msg){    
  Run(250);
  if (State()!="ERROR"){
    CMsg m;
    Output(m);
  }
}


void CIRArray::loop(){  
    CMsg m;
    runOnce(m);    
}



float CIRArray::getTemp(int x, int y, int incx, int incy){
  int offset=0;
  float t = NAN;
  
  int posX=x+incx;
  int posY=y+incy;

  if( (posX<0)||(posY<0))
    return t;

  if( (posX>=IR_X)||(posY>IR_Y))
    return t;  
  
  offset=posX+posY*IR_X;
  t=frame[offset];
  return t;
}



std::tuple<int, int> CIRArray::getHotSpot(){
  int maxX=-1,maxY=-1;
  float favg=NAN;
  for (int y=0; y<IR_Y; y++) {
    for (int x=0; x<IR_X; x++) {
      float tot=0;
      int count=0;
      favg=NAN;
      float f1=getTemp(x,y,0,0);
      float f2=getTemp(x,y,0,1);
      float f3=getTemp(x,y,0,-1);
      float f4=getTemp(x,y,1,0);
      float f5=getTemp(x,y,1,1);
      float f6=getTemp(x,y,1,-1);
      float f7=getTemp(x,y,-1,0);
      float f8=getTemp(x,y,-1,1);
      float f9=getTemp(x,y,-1,-1);
      
      

      if(!isnan(f1)){
        tot+=f1;
        count++;
      }
      

      if(!isnan(f2)){
        tot+=f2;
        count++;
      }

      if(!isnan(f3)){
        tot+=f3;
        count++;
      }

      if(!isnan(f4)){
        tot+=f4;
        count++;
      }

      if(!isnan(f5)){
        tot+=f5;
        count++;
      }

      if(!isnan(f6)){
        tot+=f6;
        count++;
      }

      if(!isnan(f7)){
        tot+=f7;
        count++;
      }

      if(!isnan(f8)){
        tot+=f8;
        count++;
      }

      if(!isnan(f9)){
        tot+=f9;
        count++;
      }


      if(count>0){
        favg=tot/(float)count;
        //writeconsole(favg); writeconsole(" ");writeconsoleln(x+y*IR_X);
        
        avgframe[x+y*IR_X]=favg;
      }
    }
    
  }

  float fmaxAVG=-1000.0;
  for (int y=0; y<IR_Y; y++) {
    for (int x=0; x<IR_X; x++) {
      favg=avgframe[x+y*IR_X];
      if(!isnan(favg)){
        if(favg>fmaxAVG){
          maxX=x;
          maxY=y;
          fmaxAVG=favg;
        }
      }
    }
  }

  return std::tuple<int, int>{maxX, maxY};
}



void CIRArray::consoleOutTemp(){
writeconsoleln("   ");
for (uint8_t h=0; h<IR_Y; h++) {
  for (uint8_t w=0; w<IR_X; w++) {
    float t = frame[h*IR_X + w];

    writeconsole(t);
    writeconsole(", ");
  }
  writeconsoleln("  ");
}
writeconsoleln("   ");
}


void CIRArray::consoleOut(){
writeconsoleln("   ");
char c;
for (uint8_t h=0; h<IR_Y; h++) {
  for (uint8_t w=0; w<IR_X; w++) {
     c= imageTable[h*IR_X + w];
    writeconsole(c);    
  }
  writeconsoleln("  ");
}
writeconsoleln("   ");
}


void CIRArray::fillPixel(){
  float y;
  int offset;
  imageTable[768]=0;
  imageTable[769]=0;
   
  for (int x = 0 ; x < 768 ; x++){  
    imageTable[x]=' ';
    y=frame[x];
    if (y!=0){
      offset= (int) map(y*10, imin, imax, 0, pixel.size()-1);
      if ((offset<0)||(offset>=pixel.size())){
         imageTable[x]='A';       
      }
       else imageTable[x]=pixel[offset];
    }  
  }
}

void CIRArray::fillGrey(){
  float y;
  int offset;
  imageTable[768]=0;
  imageTable[769]=0;


  std::string greyRamp1=greyRamp;
/*
  std::string greyRamp1;
  for(auto c:greyRamp){
    greyRamp1=c+greyRamp1;
  }
 */ 

  for (int x = 0 ; x < 768 ; x++){  
    imageTable[x]=' ';
    y=frame[x];
    if (y!=0){
      offset= (int) map(y*10, imin, imax, 0, greyRamp1.size()-1);
      if ((offset<0)||(offset>=greyRamp1.size())){
         imageTable[x]=' ';       
      }
       else imageTable[x]=greyRamp1[offset];
    }
    
  }
}

void CIRArray::fillAscii(){
  float t;
  char c;
  imageTable[768]=0;
  imageTable[769]=0;
   
  for (int x = 0 ; x < 768 ; x++){  
    t=frame[x];
    c  = '&';
    if (t!=0){
      
      if (t < 20) c = ' ';
      else if (t < 23) c = '.';
      else if (t < 25) c = '-';
      else if (t < 27) c = '*';
      else if (t < 29) c = '+';
      else if (t < 31) c = 'x';
      else if (t < 33) c = '%';
      else if (t < 35) c = '#';
      else if (t < 37) c = 'X';

    }
    imageTable[x]=c;
  }
}


void CIRArray::runOnce(CMsg &msg)
{    
  CMsg m;
  if (mlx.getFrame(frame) != 0) {
    incErrorCount();    
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

  imin=(int) 10*floor(fmin);
  imax=(int) 10*ceil(fmax);
}


void CIRArray::Output(CMsg &msg){  //Easier to send as long   convert to decimal when receiv
  std::string sType=msg.getParameter("TYPE","A");
  std::string sDisplay=msg.getParameter("CONSOLE","1");

  
  if(sType=="P") fillPixel();
  if(sType=="G") fillGrey();
  if(sType=="A") fillAscii();
  
  if(sDisplay=="1"){
    consoleOutTemp();
    consoleOut();
  }
  
  CMsg m;
  m.setTABLE("IR");
  m.setPANEL(Name());
  m.setDATA(imageTable);   ///  CHECK!!#!@#!@#

  std::tuple<int, int> XY=getHotSpot();
  
  m.setParameter("X",std::get<0>(XY));
  m.setParameter("Y",std::get<1>(XY));
  m.setParameter("MAX",fmax);
  m.setParameter("MIN",fmin);
          
  addTransmitList(m); 
}

