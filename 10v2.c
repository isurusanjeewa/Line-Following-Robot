#include "10v2.h"




#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
//#include <math.h>//pow on



/*

#define RF 31760  //Pin_c6
#define RR 31763  //Pin_c7
#define LF 31764  //Pin_c4
#define LR 31765  //Pin_c5

*/

#define RF 56  //Pin_c3
#define RR 59  //Pin_c0
#define LF 60  //Pin_c4
#define LR 61  //Pin_c5

int cons= 32;
float feed;
int i ;
int xL =0;
int xR = 0;
int binr;
float adc_val[8];
float line;
int l[8];
float ref[8];
float his = 0;

int16 a1 =2000;
int16 a2 = 1600;
int slowTurn = 99;
int turn = 88;

int slowFwd = 100;

int16 fwdcount = 35;
int blockConst = 45;

///////////////////////////////
   // define variables to change states
   int state = 0;
   int count = 0;
   
   ///////////////////////////////
   
   ////////////////////////////////
   ///////Variables to avoid obsitcals
   
   int currentDir = 2;
   int1 rotateDir = 1;
   int16 obCount =0;
   
   int1 intarupted=0;
   
   int p=0;
   
   int blockCount=0;
   int1 blocked = 0;
   
   
////////////////////////////////
///Variables to change speed 

//////////// R_2 -- L_2 
int mid =97;
int rew = 102; 


//////////////////////////////////
void fwd(){

      set_pwm1_duty(106);
      set_pwm2_duty(107);
      output_high(LF);
      output_low(LR);
      output_high(RF);
      output_low(RR);
}

void stop(){
     set_pwm1_duty(0);
     set_pwm2_duty(0);
}

void R_1(){

      set_pwm1_duty(0);
      set_pwm2_duty(108);
      output_high(RF);
      output_low(RR);
      output_low(LR);
      output_high(LF);
}


void L_1(){
      set_pwm1_duty(108);
      set_pwm2_duty(0);
      output_high(RF);
      output_low(RR);
      output_low(LR);
      output_high(LF);
}

/////////////////////////////////////////
void L_2(){

      set_pwm1_duty(rew-2);
      set_pwm2_duty(mid);
      output_low(LF);
      output_high(LR);
      output_low(RR);
      output_high(RF);
}

void R_2(){
      set_pwm1_duty(mid-2);
      set_pwm2_duty(rew);
      output_low(LR);
      output_high(LF);
      output_low(RF);
      output_high(RR);
}
/////////////////////////////////

void fwd_slow(){

      set_pwm1_duty(slowFwd-2);
      set_pwm2_duty(slowFwd);
      output_high(LF);
      output_low(LR);
      output_high(RF);
      output_low(RR);
}
void R_1_slow(){
      set_pwm1_duty(0);
      set_pwm2_duty(slowTurn);
      output_high(RF);
      output_low(RR);
      output_low(LR);
      output_high(LF);

}
void L_1_slow(){
      set_pwm1_duty(slowTurn-2);
      set_pwm2_duty(0);
      output_high(RF);
      output_low(RR);
      output_low(LR);
      output_high(LF);
}

void L_3_R(){

      set_pwm1_duty(turn);
      set_pwm2_duty(slowTurn);
      output_low(LF);
      output_high(LR);
      output_low(RR);
      output_high(RF);
}
void R_3_R(){

      set_pwm1_duty(slowTurn-2);
      set_pwm2_duty(turn);
      output_low(LR);
      output_high(LF);
      output_low(RF);
      output_high(RR);
}



void RR_1(){
      set_pwm1_duty(100);
      set_pwm2_duty(rew);
      output_high(RF);
      output_low(RR);
      output_low(LF);
      output_high(LR);
}


void LR_1(){

      set_pwm1_duty(rew);
      set_pwm2_duty(100);
      output_high(RR);
      output_low(RF);
      output_low(LR);
      output_high(LF);

}


void R_4(){

      set_pwm1_duty(94);
      set_pwm2_duty(107);
      output_low(LR);
      output_high(LF);
      output_low(RF);
      output_high(RR);;

}
void R_5(){

      set_pwm1_duty(87);
      set_pwm2_duty(110);
      output_low(LR);
      output_high(LF);
      output_low(RF);
      output_high(RR);;

}


void L_4(){

      set_pwm1_duty(107);
      set_pwm2_duty(94);
      output_low(LF);
      output_high(LR);
      output_low(RR);
      output_high(RF);
}

void L_5(){

      set_pwm1_duty(110);
      set_pwm2_duty(87);
      output_low(LF);
      output_high(LR);
      output_low(RR);
      output_high(RF);
}


#int_RDA
void  readIR() 
{
   p=getc();
   if(state==1 && intarupted==0){
      intarupted=1;
      if(blocked==0 && currentDir==2){
         rotateDir=p;
      }
   }
   //else - other ir interupts
}

void lineFallow(){

   if( xR == 1 && l[0] == 1){
            RR_1();
            delay_ms(570);
         }
   if(xL == 1 && l[7] == 1){
            LR_1();
            delay_ms(570);
   }
   
   xL = 0;
   xR=0;

      line =((25)*l[7]+(20)*l[6]+(5)*l[5]+(0)*l[4]+(0)*l[3]+(-10)*l[2]+(-15)*l[1]+ (-25)*l[0]);
   
   if (line > 0){
   
      if (line > his ){
         feed = line +7;
      }
      else{
         feed = line;
      }
   }
   else if(line <0){
      if (line < his ){
         feed = line -7;
      }
      else{
         feed = line;
      }
   }
   else{
   
      feed = line;  
   }
  
   his = line;  
   
   
   if(feed == 0){   
      fwd();
   }   
   else if( feed >=-15 && feed <0){
      L_1();
   }
   
   else if(feed <= 15 && feed > 0 ){
     R_1();   
   }
   else if (feed >= 45){
         R_1();
      }
   else if(feed < -15 && feed > -45){
      
      L_2();
   }
   else if(feed >15 && feed < 45){ 
      R_2();
     
   }
   
   else if(feed <= -45){
         L_1();
   }
   if(l[7]==1 && l[6]== 0 && ( l[3] ==1 || l[4]==1)){
         R_5();
         delay_ms(1000);
         xR=1;
         
   }
   if(l[0]==1 && l[1]== 0 && ( l[3] ==1 || l[4]==1)){
         L_5();
         delay_ms(1000);
         xL = 1;
         
   }
 
 //  delay_ms(3);
 
}

void arrowfallow(){

line =((25)*l[7]+(20)*l[6]+(15)*l[5]+(0)*l[4]+(0)*l[3]+(-15)*l[2]+(-20)*l[1]+ (-25)*l[0]);
   
   if (line > 0){
   
      if (line > his ){
         feed = line +7;
      }
      else{
         feed = line;
      }
   }
   else if(line <0){
      if (line < his ){
         feed = line -7;
      }
      else{
         feed = line;
      }
   }
   else{
   
      feed = line;  
   }
  
   his = line;  
   
  

 
 
 
   if(feed == 0){   
      fwd();
   
   }
   else if( feed >=-15 && feed <0){
      L_1();
   }
   
   else if(feed <= 15 && feed > 0 ){
     R_1();   
   }
   else if (feed >= 45){
         R_4();
      }
   else if(feed < -15 && feed > -45){
      
      L_4();
   }
   else if(feed >15 && feed < 45){ 
      R_4();
     
   }
   
   else if(feed <= -45){
         L_4();
   }   
   delay_ms(10);
}

void avoidObsticals(){

 if(intarupted==1){
 
  if(currentDir==2){
         if(rotateDir){
            L_3_R();
         }else{
            R_3_R();
         }
         currentDir=rotateDir;
         delay_ms(a2);
         fwd();
      }else{
      blocked = 1;
      obCount =0;
        
         if(rotateDir==1){

            output_b(9);
            R_3_R();
            delay_ms(a2);
            output_b(1);
            R_1_slow();
            delay_ms(a1);
            currentDir=0;
            rotateDir=0;
         }else{
         output_b(9);
            L_3_R();
            delay_ms(a2);
            L_1_slow();
            output_b(1);
            delay_ms(a1);
            currentDir=1;
            rotateDir=1;
            
         }
      }
      intarupted=0;
 }
 

 fwd_slow();
 delay_ms(15);
 if(currentDir!=2){
   obCount++;
   delay_ms(10);
   if(obCount>fwdCount){
      obCount = 0;
      stop();
      delay_ms(10);
         if(rotateDir){
            R_1_slow();
            delay_ms(a1); 
         }else{
            L_1_slow();
            delay_ms(a1); 
         }
         currentDir=2;     
   }
 }
 
 ///////////////////////////// rotate dir select
 if(blocked){
   blockCount++;
   if(blockCount> blockConst){
      blocked=0;
      blockCount=0;
   }
 }

}


void main()
{

/*
   setup_adc_ports(ALL_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_1,127,1);
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
*/
//Setup_Oscillator parameter not selected from Intr Oscillator Config tab

   // TODO: USER CODE!!
  setup_adc_ports(ALL_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_1,127,1);
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
  
  stop();
    delay_ms(1000);
   
   for( i = 0 ; i <8 ; i ++) {
      set_adc_channel(i);            // set ref valus
      delay_ms(40);
      ref[i] =read_adc();
      delay_ms(40);
   }
   output_b(7);
   delay_ms(3000);
   
   while(1){
   
   
 
  
  for (i = 0 ; i <8 ;i++){
      delay_ms(1);
      set_adc_channel(i);
      delay_ms(1);                        // take readings
      adc_val[i] =read_adc();
      if(adc_val[i] > ref[i]+cons){
         l[i] =1;
      }
      else{
         l[i]=0;
      }
   }
   binr =(128*l[7]+64*l[6]+32*l[5]+16*l[4]+8*l[3]+4*l[2]+2*l[1]+ l[0]);
if(l[6]==1){
   output_b(31);
//}else{
 //    output_b(currentDir+(8*state));
}

   ////////////////////////////////////////////////////////
   ///////// Edit - to change states///////////////////////
   if(state == 0){
      if(binr == 255){
         count++;
      }else{
         count =0;
      }
      if(count>50){
            state=1;
            count = 0;
      }
      
      // State 0 & 2 (line) code here
      lineFallow();
      
   }else if(state == 1){
      if(binr == 0){
         state = 2;
      }
     avoidObsticals(); 
     // State 1 (obsticle) code here 
      
   }else if(state == 2){
 
      if(binr == 255){
         count++;
         delay_ms(10);
      }else{
         count =0;
      }
      
      if(count>25){                     // This is to stop at the centere of the goal
            state=3;
            count = 0;
      }
      
      // State 0 & 2 (line) code here
      arrowfallow();
   }else{
      stop();
    //  break;
   }
   ////////////////////////////////////////////////////////
  }
}

