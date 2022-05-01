
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
}

 void CIRArray::config( char addr, TwoWire *pWire) {
   
    _pWire=pWire;    
    _address=addr;    
  }

void CIRArray::setup()
{
  CMsg m;
  init();          

  for(int retries=0;retries<5;retries++){
  if (! mlx.begin(_address, _pWire)) {
    
    m.setSYS(Name());
    m.setINFO("ERROR");
    m.setCOMMENT("mlx.begin ERROR   IRARRAY Setup");
    writeconsoleln(m.serializeout());
    addTransmitList(m);    
    setState("ERROR");
    delay(300);
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

}

void CIRArray::test(CMsg &msg){  
  CSystemObject::test(msg);
  CMsg m;
  runOnce(m);
  Output(m);
  
}


void CIRArray::loop(){
  if(subscribers(Name())){
    CMsg m;
    runOnce(m);
    Output(m);
    //writeconsoleln("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX   SUBSCRIBED IRARRAY XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  }
}

void CIRArray::consoleOutTemp(){
writeconsoleln("   ");
for (uint8_t h=0; h<24; h++) {
  for (uint8_t w=0; w<32; w++) {
    float t = frame[h*32 + w];

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
for (uint8_t h=0; h<24; h++) {
  for (uint8_t w=0; w<32; w++) {
     c= imageTable[h*32 + w];
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
  if (State()!="PLAY")
    setup();
  if (State()!="PLAY")
    return;    
  std::string sOut=msg.getParameter("OUTPUT","");
  CMsg m;
  if (mlx.getFrame(frame) != 0) {
    m.setSYS(Name());
    m.setINFO("ERROR");
    m.setCOMMENT("mlx.begin ERROR   IRARRAY runOnce");
    addTransmitList(m);
    writeconsoleln(m.serializeout());    
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
  runOnce(msg);
  std::string sType=msg.getParameter("TYPE","A");
  std::string sDisplay=msg.getParameter("CONSOLE","1");

  
  if((sType=="P")||(subscribers(Name()+"P"))) fillPixel();
  if((sType=="G")||(subscribers(Name()+"G"))) fillGrey();
  if((sType=="A")||(subscribers(Name()+"A"))) fillAscii();
  
  if(sDisplay=="1"){
    consoleOutTemp();
    consoleOut();
  }
  
  CMsg m;
  m.setTABLE("IR");
  m.setPANEL(Name());
  m.setDATA(imageTable);
          
  addDataList(m); 
}


void  CIRArray::callCustomFunctions(CMsg &msg){
  std::string act=msg.getACT();
    
  if(act=="IRRUN") runOnce(msg);
}
