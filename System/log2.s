
; log2:   32 bit base 2 logarithm function for ARM Cortex-M3      KEIL  
; --------------------------------------------------------------------------
; (c) 2010 Ivan Mellen                                            April 2010 
; --------------------------------------------------------------------------
; Free Personal, Non-Commercial Use License. 
; The Software is licensed to you for your personal, NON-COMMERCIAL USE.
; If you have questions about this license, need other sizes or would like
; a different license please email : imellen(at)embeddedsignals(dot)com

 
; This file contains only one function:
; int log2(unsigned int x);   

; input 16q16 unsigned   1/65536 to 65535+65535/65536  
; output 5q27 signed   -15.99999 to 15.99999
; log2(0x80800000)=0x780b81b7   e.g. log2(32896)= 15.005618505179882049560546875
; log2(0x0000abcd)= -77230085   e.g. log2(0.67109680175781)= -0.57540897279978
 
; max error: +- 4E-7 = +-0.4 ppm   (calculated - correct)/correct
; 1+11 cycles + call overhead

; Base B logarithm : logB(x) = k* log2(x)
;  where k = 1/ log2(B)  ; for log10: k=0.301029995;      for ln: k=0.693147180


            THUMB
	    AREA    FFTLIB2_CORTEXM3, CODE, READONLY        
	    EXPORT  log2
            ALIGN 8

log2

            adr r12, LUTlog2  				;value not changed, reused in batch processing
            
            clz r1, r0       				;core, 11 cycles
            lsls r0, r1						;Logical Shift Left
            ubfx r3, r0, #0x0F, #0x09		;Unsigned bit extract
            ubfx r0, r0, #0x18, #0x07		;Unsigned bit extract
            ldr.w r2, [r12, r0, lsl #0x2]	;Load Register with word
            mov.w r0, r2, asr #0x03			;Move
            ubfx r2, r2, #0x00, #0x0C		;Unsigned bit extract
            mul  r2, r3						;Multiply
            adds.w r0, r2, r0, lsr #0x01	;Add
            subs.w r0, r0, r1, lsl #0x1B	;Sub
            
            bx lr  							;only r0-r3, r12 modified
	
                   
           ALIGN 2

LUTlog2 

  DCD 0x80002b7f,0x81701b68,0x82dd1b52,0x84475b3c,0x85aedb26,0x8713ab11,0x8875cafc,0x89d53ae7
  DCD 0x8b321ad3,0x8c8c6abf,0x8de43aab,0x8f399a97,0x908c7a84,0x91dcea71,0x932b0a5e,0x9476ca4c
  DCD 0x95c03a39,0x97076a27,0x984c4a16,0x998efa04,0x9acf79f3,0x9c0dd9e2,0x9d4a09d1,0x9e8429c0
  DCD 0x9fbc39b0,0xa0f239a0,0xa2262990,0xa3582980,0xa4882971,0xa5b64961,0xa6e26952,0xa80ca943
  DCD 0xa9350934,0xaa5b9926,0xab805917,0xaca33909,0xadc458fb,0xaee3c8ed,0xb00168df,0xb11d58d2
  DCD 0xb23788c5,0xb35028b7,0xb46708aa,0xb57c489d,0xb68ff890,0xb7a1f884,0xb8b27877,0xb9c1686b
  DCD 0xbaceb85f,0xbbda9853,0xbce4e847,0xbdedc83b,0xbef5282f,0xbffb0824,0xc0ff8818,0xc202880d
  DCD 0xc3042802,0xc40467f7,0xc50337ec,0xc600b7e1,0xc6fcd7d6,0xc7f797cc,0xc8f117c1,0xc9e947b7
  DCD 0xcae017ad,0xcbd5b7a2,0xccca0798,0xcdbd178e,0xceaed785,0xcf9f777b,0xd08ed771,0xd17cf768
  DCD 0xd269f75e,0xd355b755,0xd440574c,0xd529d742,0xd6121739,0xd6f94730,0xd7df5727,0xd8c4371f
  DCD 0xd9a81716,0xda8ad70d,0xdb6c6705,0xdc4d06fc,0xdd2c86f4,0xde0b06eb,0xdee876e3,0xdfc4d6db
  DCD 0xe0a036d3,0xe17a96cb,0xe253e6c3,0xe32c46bb,0xe403a6b3,0xe4da16ab,0xe5af76a4,0xe683f69c
  DCD 0xe7576695,0xe82a068d,0xe8fba686,0xe9cc667e,0xea9c3677,0xeb6b1670,0xec390669,0xed062662
  DCD 0xedd2565b,0xee9da654,0xef68264d,0xf031c646,0xf0fa863f,0xf1c26638,0xf2896632,0xf34fa62b
  DCD 0xf4150625,0xf4d9961e,0xf59d5618,0xf6605611,0xf722760b,0xf7e3c605,0xf8a465fe,0xf96425f8
  DCD 0xfa2325f2,0xfae165ec,0xfb9ed5e6,0xfc5b95e0,0xfd1785da,0xfdd2c5d4,0xfe8d45ce,0xff4705c8

  END