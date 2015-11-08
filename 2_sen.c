#include "2_sen.h"
int cons = 15;
float ref_0;
float ref_3;
float adc_val_0;
float adc_val_3;
int l_0;
int l_3;
void main()
{

   setup_adc_ports(ALL_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);

   // TODO: USER CODE!!
   
      set_adc_channel(0);            // set ref valus
      delay_ms(100);
      ref_0 =read_adc();
      delay_ms(100);
      
      set_adc_channel(3);            // set ref valus
      delay_ms(100);
      ref_3 =read_adc();
      delay_ms(100);
   
   output_b(0b11111111);
   delay_ms(700);
   output_b(0);
   
   while(1){
      set_adc_channel(0);
      delay_ms(20);                        // take readings
      adc_val_0 =read_adc();
      delay_ms(20);
      if(adc_val_0 > ref_0+cons){
         l_0 =1;
         output_high(pin_d7);
         delay_ms(500);
      }
      else{
         l_0=0;
         output_low(pin_d7);
     //    delay_ms(500);
      }
      
      set_adc_channel(3);
      delay_ms(20);                        // take readings
      adc_val_3 =read_adc();
      delay_ms(20);
      if(adc_val_3> ref_3+cons){
         l_3 =1;
         output_high(pin_d6);
         delay_ms(500);
      }
      else{
         l_3=0;
         output_low(pin_d6);
      }
   }
}
