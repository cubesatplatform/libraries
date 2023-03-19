
#include <vector>
#include "system_irarray.h"
#include <cmath>
#include <string>




const std::vector<char> pixel={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const std::string greyRamp = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+=<>i!lI;,""^`\'. ";

CIRArray::CIRArray(){    
}


void CIRArray::init(){
  CSystemObject::init();
  setInterval(40);
  setErrorThreshold(10); 
}

 void CIRArray::config( char addr, TwoWire *pWire) {
   
    _pWire=pWire;    
    _address=addr;    
  }


void CIRArray::config(CMsg &msg){
  std::string straddress=msg.get(_ADDRESS);
  std::string strwire=msg.get(_WIRE);

  TwoWire *pWire;

  if(strwire==_WIRE) pWire=&Wire;  
  if(strwire==_WIRE1) pWire=&Wire1;  
  if(strwire==_WIRE2) pWire=&Wire2;  

  if(straddress.size()>0){
    config(straddress[0],pWire);
  }
}

void CIRArray::setup()
{
  CMsg m;
  init();          

  for(int retries=0;retries<10;retries++){
  if (! mlx.begin(_address, _pWire)) {
     incErrorCount();      
     
     writeconsole("NOT Found MLX90640: ");writeconsoleln(_address);
     delay(500);
    }
  else{
    writeconsoleln("Found MLX90640");
    delay(500);
  
    
    mlx.setMode(MLX90640_INTERLEAVED);    
    
    if (mlx.getMode() == MLX90640_CHESS) {    
    } else {    
    }
  
    mlx.setResolution(MLX90640_ADC_18BIT);
    
    mlx90640_resolution_t res = mlx.getResolution();
    switch (res) {
      case MLX90640_ADC_16BIT: m.set(_INFO,"16 bit"); break;
      case MLX90640_ADC_17BIT: m.set(_INFO,"17 bit"); break;
      case MLX90640_ADC_18BIT: m.set(_INFO,"18 bit"); break;
      case MLX90640_ADC_19BIT: m.set(_INFO,"19 bit"); break;
      }
  
    mlx.setRefreshRate(MLX90640_4_HZ);   //2
    
    mlx90640_refreshrate_t rate = mlx.getRefreshRate();
    switch (rate) {
      case MLX90640_0_5_HZ: m.set(_COMMENT,"0.5 Hz"); break;
      case MLX90640_1_HZ: m.set(_COMMENT,"1 Hz"); break; 
      case MLX90640_2_HZ: m.set(_COMMENT,"2 Hz"); break;
      case MLX90640_4_HZ: m.set(_COMMENT,"4 Hz"); break;
      case MLX90640_8_HZ: m.set(_COMMENT,"8 Hz"); break;
      case MLX90640_16_HZ: m.set(_COMMENT,"16 Hz"); break;
      case MLX90640_32_HZ: m.set(_COMMENT,"32 Hz"); break;
      case MLX90640_64_HZ: m.set(_COMMENT,"64 Hz"); break;
      }
    
    setState(_PLAY);
    return;
    }
  }
  sendError();
}




void CIRArray::loop(){  
    CMsg m;
    m.set(_CONSOLE,"1");
    runOnce(m);       //PUT BACK  TESTING ONLY
    
    output(m);
   // setState("COMPLETE");
}



float CIRArray::getTemp(int x, int y, int incx, int incy){
  unsigned int offset=0;
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


CMsg CIRArray::variance(float a[], int n) 
{   
    CMsg m;
    // Compute mean (average of elements) 
    float sum = 0; 
    
    for (int i = 0; i < n; i++) sum += a[i];    
    float mean = (float)sum / (float)n; 
    // Compute sum squared differences with mean. 
    float sqDiff = 0; 
    for (int i = 0; i < n; i++) 
        sqDiff += (a[i] - mean) * (a[i] - mean); 
   // return (float)sqDiff / n; 

    m.set("MEAN",mean);
    m.set("VARIANCE",(float)sqDiff / n);
    m.set("STDDEV",sqrt((float)sqDiff / n));
    m.set("COUNT", n);

    return m;
} 



std::tuple<int, int> CIRArray::getHotSpot(){
  float avgframe[IR_X*IR_Y];
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



void CIRArray::consoleOut(char *imageTable){

for (uint8_t h=0; h<IR_Y; h++) {
  for (uint8_t w=0; w<IR_X; w++) {
    char c = imageTable[h*IR_X + w];

    writeconsole(c);
    writeconsole(", ");
  }
  writeconsoleln("  ");
}
writeconsoleln("   ");
}





void CIRArray::fillPixel(char *imageTable){

  float y;
  unsigned int offset;
  
   
  for (int x = 0 ; x < IRARRAYSIZE ; x++){  
    imageTable[x]=' ';
    y=frame[x];
    if (y!=0){
      offset= (int) map(y*10, imin, imax, 0, pixel.size()-1);
      imageTable[x]=pixel[offset];
    }  
  }
}

void CIRArray::fillGrey(char *imageTable){

  float y;
  unsigned int offset;
 


  std::string greyRamp1=greyRamp;
 

  for (int x = 0 ; x < IRARRAYSIZE ; x++){  
    imageTable[x]=' ';
    y=frame[x];
    if (y!=0){
      offset= (int) map(y*10, imin, imax, 0, greyRamp1.size()-1);
      imageTable[x]=greyRamp1[offset];
    }
    
  }
}

void CIRArray::fillAscii(char *imageTable){
  
  float t;
  char c;
   
  for (int x = 0 ; x < IRARRAYSIZE ; x++){  
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


void CIRArray::fillTemp(char *imageTable){
  
  float t;
  unsigned char c;

  float fmean=0.0;
  float fstddev=0.0;

  
  CMsg m= variance(frame, IRARRAYSIZE);
  m.writetoconsole();


  fmean=m.get("MEAN",0.0);   
  
  fstddev=m.get("STDDEV",0.0);  //Everything is within 4 standard deviations of the mean  (basically everything)


   
  for (int x = 0 ; x < IRARRAYSIZE ; x++){  
    t=frame[x];
    /*
    writeconsole(t);writeconsole(" ");
    t+=offset;
    if (t<0.0)  t=0.0;
    if (t>254.0) t=254.0;   
    c=char(round(t));
    */

   //int <newvalue> = map(<value>, <original_min>, <original_max>, <new_min>, <new_max>);
    c=map(t, fmean-5.0*fstddev, fmean+5.0*fstddev, 0, 255);
    writeconsole(c);writeconsole(" ");
    imageTable[x]=c;
  }
}



void CIRArray::runOnce(CMsg &msg)
{    
  CMsg m;
  
  if (mlx.getFrame(frame) != 0) {
    incErrorCount();    
    return;
  }
  
  
  float y,lasty;

  unsigned int offset=rand()%700 +5;   //random between 7 and 705

  
  y=frame[offset];

  if(y<-100.0) y=0.0;
  if(y>200.0) y=0.0;

  fmin=y;
  fmax=y;
  lasty=y;
   
  for (int x = 0 ; x < IRARRAYSIZE ; x++)    //Find min max
  {
    y=frame[x];
    frame[x]=y*9.0/5.0+32.0;  //convert to fahrenheit
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


void CIRArray::output(CMsg &msg){  //Easier to send as long   convert to decimal when receivE

  writeconsoleln("IRArray output............................");
  char imageTable[IRARRAYSIZE];  //need it null terminated  

  memset(imageTable, 7, IRARRAYSIZE);
  
  std::string sType=msg.get(_VALUE,"A");
  std::string sDisplay=msg.get(_CONSOLE,"0");

/*
  if(sType==_FILLPIXEL) fillPixel(imageTable);
  if(sType==_FILLGREY) fillGrey(imageTable);
  if(sType==_FILLASCII) fillAscii(imageTable);
  
  */
  
  fillTemp(imageTable);
  if(sDisplay=="1"){
    consoleOut(imageTable);    
  }
  
  

  CMsg m= variance(frame, IRARRAYSIZE);

  m.set(_NAME,name());
  m.set(_TIME,getTime());
  
  std::tuple<int, int> XY=getHotSpot();
  int x=std::get<0>(XY);
  int y=std::get<1>(XY);
  m.set(_PIXELX,x);
  m.set(_PIXELY,y);
  m.set(_IR_MAX,fmax);
  m.set(_IR_MIN,fmin);
  m.writetoconsole();
  //addTransmitList(m);
  addDataMap(m);

  std::string datastr;

  for(int i=0;i<IRARRAYSIZE;i++){
    datastr+=imageTable[i];
  }



  
  CMsg mm;
  mm.set(_TIME,getTime()%1000);

  std::string s=mm.get(_TIME);

  std::string strfn=name();
  strfn+='-';
  strfn+=s;               //Makes the name unique
  strfn+='_';
  
  
  char c='a';
  for(int count=0;count<IRARRAYSIZE;count+=MAXPARAMSIZE){    
    CMsg mPart;
    std::string str;
    
  
    str=datastr.substr(count,MAXPARAMSIZE);

  writeconsoleln("DATASTRING");
  writeconsoleln(str);
  writeconsoleln("^^DATASTRING");


    std::string strfilename=strfn;
    strfilename+=c;
    strfilename+=".jpg";
    
    //mPart.set(_API,_INSERTMULTI);
    writeconsoleln(str);
    //str="Hello Kitty";
    mPart.set(_NAME,name());
    mPart.set(strfilename,str);
    mPart.writetoconsole();
    
    c++;    
    addTransmitList(mPart);
  }
}



void CIRArray::callCustomFunctions(CMsg &msg){   //Calls a specific function directly
 
  std::string act=msg.get(_ACT);  

 
  
  /*
  mapcustom(fillPixel)
  mapcustom(fillGrey)
  mapcustom(fillAscii)

  if (act==FILLPIXEL) fillPixel();
  if (act==FILLGREY) fillGrey();
  if (act==FILLASCII)  fillAscii();
  */
   CSystemObject::callCustomFunctions(msg);

}