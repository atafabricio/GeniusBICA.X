# GeniusBICA.X
Jogo Genius no MicroControlador PIC18F4520
Disciplina: MicroControladores
Curso: Engenharia da Computação, IFSP - BIRIGUI
Data: 2º Sem / 2019
Autores: Fabrício Malta / Henrique Marcolino

Sobre o Jogo: Clássico Jogo Genius onde uma sequência é gerada aleatoriamente, o usuário deve inserir a sequência, sucessivamente
até ganhar o jogo, nesse PIC é exatamente a mesma coisa.

O que funciona: Todo o jogo utilizando apenas LEDs ( não há som, mas nada o impede de ser adicionado posteriormente)

Como jogar? : Nós utilizamos comunicação e transmissão através do módulo RXTX pois previamente o jogo precisaria de uma aplicação feita em java
( também disponível no github), para que o jogo comece, é necessário mandar um caractere ' 1 ' na porta COM do módulo de comunicação.
A cada fase que o jogador passará, a pontuação atual dele é enviada de volta na porta COM, sucessivamente até o jogo se encerrar, sendo quando a
sequência inserida for errada ou o jogador finalizou com sucesso.


Dúvidas?: Nesse mesmo Github há nossa apresentação da ideia do projeto, onde há o fluxograma.


Contato: fabricio.malta@outlook.com
