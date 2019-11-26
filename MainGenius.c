#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pic18f4520.h>
#include <pic18.h>
#include <time.h>
#include "configura.h"
#include "lcd.h"

#define NIVEIS 20
#define _XTAL_FREQ 4000000

//Botoes
#define Botao1 PORTCbits.RC1
#define Botao2 PORTCbits.RC2
#define Botao3 PORTCbits.RC3
#define Botao4 PORTCbits.RC0

//Leds
#define Led1 PORTBbits.RB7
#define Led2 PORTBbits.RB6
#define Led3 PORTBbits.RB5
#define Led4 PORTBbits.RB0

     
//nivel atual do jogo
int level = 1;
int pos;
int temp;
int inicio = 0;
int flag = 1;
//buffers
int Memoria1[NIVEIS];
//int Memoria2[NIVEIS];

void display(int u)
{
   
    const char tabela []={
    
/*0*/     0X3F,
/*1*/     0X06,
/*2*/     0X5B,
/*3*/     0X4F,
/*4*/     0X66,
/*5*/     0X6D,
/*6*/     0X7D,
/*7*/     0X07,
/*8*/     0X7F,
/*9*/     0X67,
/*A*/     0X77,
/*b*/     0X7C,
/*C*/     0X39,
/*d*/     0X5E,
/*E*/     0X79,
/*F*/     0X71
       };
   PORTD=tabela[u];
   //LATB1=1;
   __delay_ms(5);
   //LATB1=0;           
}

void Write_Display(int u){
     int uni, dez;
        uni = u%10; 
        dez = u/10;
        PORTB = 0B00001000;
        display(uni);
        PORTB = 0B00010000;
        display(dez);
}

void Som_Botao1()
{
    Led1 = 1;
    __delay_ms(300);
    Led1 = 0;
}

void Som_Botao2()
{
    Led2 = 1;
    __delay_ms(300);
    Led2 = 0;
}

void Som_Botao3()
{
    Led3 = 1;
    __delay_ms(300);
    Led3 = 0;
}

void Som_Botao4()
{
    Led4 = 1;
    __delay_ms(300);
    Led4 = 0;
}

void Som_Inicial()
{
    Led1 = 1; Led2 = 1; Led3 = 1; Led4 = 1;
    lcd_clear();
    lcd_puts("Bem Vindo s2");
    __delay_ms(2000);
    Led1 = 0; Led2 = 0; Led3 = 0; Led4 = 0;
}

void iniciogame()
{
    lcd_clear();
    lcd_puts("Aperte em jogar");
    while (inicio!=1)
        {
        level = 1;
        __delay_ms(100);
        }
    lcd_clear();
    inicio=0;
}

void Som_Perdeu()
{
    Led1 = 1; Led2 = 1;
    lcd_clear();
    lcd_puts("Perdeu");
    __delay_ms(1000);
    lcd_clear();
    Led1 = 0; Led2 = 0;
    iniciogame();
}

void Som_Ganhou()
{
    Led1 = 1; Led2 = 1; Led3 = 1; Led4 = 1;
    lcd_clear();
    lcd_puts("Parabens");
    __delay_ms(1000);
    lcd_clear();
    Led1 = 0; Led2 = 0; Led3 = 0; Led4 = 0;
}

//Gera uma sequencia
void GerarSequencia()
{
unsigned semente;
int random;

    semente = (TMR0H << 8) + TMR0L;
    srand(semente);
    lcd_clear();
     lcd_puts("Gerando");
        random = (rand() % 4);
        Memoria1[level] = random;
         __delay_ms(1000);
}

void MostraSequencia(){
    int i;
    for(i=1;i<=level;i++){
        __delay_ms(500);
         if(Memoria1[i] == 0)
           Som_Botao1();
         else if(Memoria1[i] == 1)
           Som_Botao2();
         else if(Memoria1[i] == 2)
           Som_Botao3();
         else
           Som_Botao4();
    }
}

//Verifica a sequencia do usuário com a
//sequencia gerado pelo pic
char VerificarSequencia(int but, int cnt)
{
    
	if(but!=Memoria1[cnt]){
		return 0;
	}
    else
     return 1;
}

void jogo();

void main()

{
        config();
        lcd_clear();
        lcd_init();
    PORTB = 0;
        
    PORTD = 0;
    PORTE = 0;
    
   // TMR0 = 6;
    
    
   
   
    //__delay_ms(2000);;
    
    jogo();
}

void jogo(){
    GIE=1;
    iniciogame();
    int flags = 0;
    Som_Inicial();
    while(1)
    {      
        GerarSequencia();
        MostraSequencia();
        pos = 1;
                   
        lcd_clear();
        lcd_puts("Aguardando...");
        
        while(1)
        {
          
            if(!Botao1) flags = 1;
           else if(!Botao2) flags = 2;
           else if(!Botao3) flags = 3;
           else if(!Botao4) flags = 4;
           
           
           if(Botao1 && flags == 1)
           {
               flags = 0;
               Som_Botao1();
             if(VerificarSequencia(0, pos))
             {
                    pos++;
             }
             else{
                Som_Perdeu();
                break;
             }
           }
           else if(Botao2 && flags == 2)
           {
               flags = 0;
               Som_Botao2();             		
             if(VerificarSequencia(1, pos)){
                    pos++;
             }
             else{
                Som_Perdeu();
                break;
             }
           }
           
           else if(Botao3 && flags == 3)
           {
               flags = 0;
               Som_Botao3();
             if(VerificarSequencia(2, pos)){
                    pos++;
             }
             else{
                Som_Perdeu();
                break;
             }
           }
           
           else if(Botao4 && flags == 4)
           {
               flags = 0;
               Som_Botao4();
             if(VerificarSequencia(3, pos)){
                    pos++;
             }
             else{
                Som_Perdeu();
                break;
             }
            }
           
         if( pos > level ) 
         {
             level++;
             lcd_clear();
             lcd_puts("Fase Concluida");
             __delay_ms(1000);
             lcd_clear();
             break;
        }
    }
        if(level == NIVEIS)
           {
              level = 1;
              Som_Ganhou();
              iniciogame();
           }
    }
}

void __interrupt(high_priority) tmr (void)
{
    if (RCIF)
    {
        RCIF=0;        
        if(RCREG==2)
        {
            level=1;
            jogo();
        }
        else if (RCREG==1){
            inicio=1;
        }
    }    
}
