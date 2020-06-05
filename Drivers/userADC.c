#include "userADC.h"
#include <math.h>

//initializes the ADC
void initADC(enum MUXPOS ainChannel){

    NVIC_EnableIRQ(ADC_IRQn);

    GCLK->PCHCTRL[30].reg = GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK0) | GCLK_PCHCTRL_CHEN; // enable peripheral clock for ADC adn set it to look to GENCLK0

    uint32_t *calPTR;
    calPTR = (uint32_t *) NVMCTRL_OTP5;
    uint32_t calData = *calPTR;
    
    uint8_t OSCCal = ( calData & 0b1111111000000 ) >> 6 ;


    int NVMbiasrefbuf = calData & 0b000111 ; // add ADC linearity data (0x7 = 000111)
    int NVMbiascomp = ( calData & 0b111000 ) >> 3; // set bias calibration (0x56 = 111000)


    ADC->CALIB.reg = ADC_CALIB_BIASCOMP( NVMbiasrefbuf ) | ADC_CALIB_BIASREFBUF( NVMbiascomp ) ;

    // using the internal reference
    ADC->CTRLB.reg |= ADC_CTRLB_PRESCALER_DIV256 ;
    ADC->REFCTRL.reg |= ADC_REFCTRL_REFSEL_INTVCC1 ;
    ADC->CTRLC.reg |= ADC_CTRLC_RESSEL_16BIT ;
    ADC->AVGCTRL.reg |= ADC_AVGCTRL_SAMPLENUM_1024 ;
    ADC->SAMPCTRL.reg |= ADC_SAMPCTRL_OFFCOMP ; // offcomp cannot be used with sample length time
    ADC->DBGCTRL.reg |= ADC_DBGCTRL_DBGRUN ; 

    ADC->INPUTCTRL.reg |= ADC_INPUTCTRL_MUXPOS(ainChannel) ; 

    ADC->CTRLA.reg |= ADC_CTRLA_ENABLE ;  

}
/*
  @description : return powers of 2

  @var n       : user given number 
  @return      : 2^n
*/
int powTwo(int n){
  int i , product = 1;
  for(i = 0 ; i < n ; i++)
    product *= 2 ;
  return product ;
}
/*
  @description    : reads from the adc

  @var ainChannel : the channel to read from
  @return         : the value read from the adc
*/
double readAIN(enum MUXPOS ainChannel){

        ADC->INPUTCTRL.reg |= ADC_INPUTCTRL_MUXPOS(ainChannel) ; 

        ADC->SWTRIG.reg = ADC_SWTRIG_START ; 
        
        while(!ADC->INTFLAG.bit.RESRDY){
        }

        int adcRes = 16 ; // resolution in bits

        int sampleNum = 16;

        int bitMask = powTwo(adcRes) - 1;
  
        double vddAna = 3.314 ; 

        int adcResultRaw = ADC->RESULT.reg ; 
        int adcResultMasked = adcResultRaw & bitMask ;
        double averagedAdcResult = adcResultMasked / (double)sampleNum ;
        double ratio = averagedAdcResult / (double)powTwo(12) ;
        return ( ratio * vddAna / 2 ); // it's divided by two because the reference is 1/2 vddANA
        
}
/*
  @description    : reads the temperature of a given temperature sensor

  @var ainChannel : the channel to read from
  @return         : the temperature read from the RTD006 temp sensor
*/
double readRTD006(enum MUXPOS ainChannel){

        // these come from the data sheet for the RTD sensor
        double dinA = 3.9083E-3 ; 
        double dinB = -5.775E-07 ; 

        int zeroResistance = 1000;
    
        double quadA = zeroResistance * dinB;
        double quadB = zeroResistance *dinA;
        double quadC;

        int thermocoupleResistor = 989 ; // 1kohm resistor
        double vddAna = 3.314 ; //TODO set up so vddAna comes from a single ended measurement of vddAna
        // AIN1 is the only RTD sensor on the prototype board
        double volts = readAIN(ainChannel);
        
        int PTCresistance = thermocoupleResistor * (vddAna / volts - 1);

        quadC = zeroResistance - PTCresistance;

        double c =  ( 2 * quadA );
        double b =  sqrt( pow( quadB , 2 ) - 4 * quadA * quadC );

        double temp = ( -quadB + sqrt( pow( quadB , 2 ) - 4 * quadA * quadC )) / ( 2 * quadA );

        return  temp;

}