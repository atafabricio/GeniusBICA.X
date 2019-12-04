#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pic18f4520.h>
#include <pic18.h>
#include <time.h>
#include "configura.h"
#include "lcd.h"

#define NIVEIS 6
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


void envia(int acabou) //envia para o tx
{
    while(!TXIF)
           {}
    if (acabou == 1)
        TXREG = level-1;
    else
        TXREG = 255;
}

void Som_Botao1() //botao 1 acionado
{
    Led1 = 1;
    __delay_ms(300);
    Led1 = 0;
}

void Som_Botao2() //botao 2 acionado
{
    Led2 = 1;
    __delay_ms(300);
    Led2 = 0;
}

void Som_Botao3() //botao 3 acionado
{
    Led3 = 1;
    __delay_ms(300);
    Led3 = 0;
}

void Som_Botao4() //botao 4 acionado
{
    Led4 = 1;
    __delay_ms(300);
    Led4 = 0;
}

void Som_Inicial() //animação inicio
{
    Led1 = 1; Led2 = 1; Led3 = 1; Led4 = 1;
    lcd_clear();
    lcd_puts("Bem Vindo s2");
    __delay_ms(2000);
    Led1 = 0; Led2 = 0; Led3 = 0; Led4 = 0;
}

void iniciogame() //aguardando rx
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
    Som_Inicial();
}

void Som_Perdeu() //animaçao perdeu
{
    
    Led1 = 1; Led2 = 1;
    lcd_clear();
    lcd_puts("Perdeu");
    envia(0);
    __delay_ms(1000);
    lcd_clear();
    Led1 = 0; Led2 = 0;
    
    iniciogame();
}

void Som_Ganhou() //animaçao ganhou
{
    Led1 = 1; Led2 = 1; Led3 = 1; Led4 = 1;
    lcd_clear();
    lcd_puts(" | Parabens | ");
    lcd_goto(40);
    lcd_puts(" | Voce Venceu |");
    __delay_ms(10000);
    lcd_clear();
    Led1 = 0; Led2 = 0; Led3 = 0; Led4 = 0;
}

//Gera uma sequencia
void GerarSequencia() //gerando a sequencia 1 por 1
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
    LATA5=1;
    jogo();
}

void jogo(){
    GIE=1;
    TXREG=-1;
    iniciogame();
    int flags = 0;
    while(1)
    {      
        
        GerarSequencia();
        MostraSequencia();
        pos = 1;
                   
        lcd_clear();
        lcd_puts("Insira sequencia...");
        
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
             envia(1);
             lcd_clear();
             break;
        }
    }
        if(level == NIVEIS)
           {
              envia(0);
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
        if(RCREG=='2')
        {
            level=1;
            lcd_clear();
            lcd_puts("JOGO CANCELADO");
            __delay_ms(1000);
            jogo();
        }
        else if (RCREG=='1'){
            inicio=1;
        }
    }    
}
