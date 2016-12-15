# CoucouRubanLED

///////////////
//COUCOU-rubanLED//
////////////////


/* D'après le programme Petit BOT conçu par Julien Rat sous licence CC-By-Sa. 
Ce programme sert à piloter le "Petit Bot". C'est un robot pédagogique, très peu cher, utilisé par les petits débrouillards*/


/*

Programme COUCOU-rubanLED par Guilaume Apremont/ Les Petits débrouillards Aquitaine/ sous licence CC-By-Sa
= Lampe Informative RGB WIFI

Les outils :
- Wemos Mini D1mini  
- Ruban de LED WS2812B 
La Wemos a la bonne idée de fournir une sortie 5V : juste ce qu'il faut pour alimenter le ruban de led.

Branchement :
- Le fil rouge du ruban raccordé a la PIN 5V de la Wemos
- Le fil noir du ruban raccodé a la PIN GND de la Wemos
- Le fil DATA du ruban raccordé a la PIn D4 (GPIO2) de la Wemos

Pilotage :
Connectez-vous au réseau wifi COUCOU
Vous pourrez piloter votre ruban de LED de plusieurs façons :
      1- Dans un  navigateur, tapez 192.168.4.1/ puis l'instruction ( rouge, vert, bleu, .....)
      ou
      2- Dans un  navigateur, tapez coucou.local/ puis l'instruction ( rouge, vert, bleu, .....)
      ou
      3- Installez l'apk android 
*/

//See more at: http://www.esp8266.com/viewtopic.php?f=29&t=6419#sthash.gd1tJhwU.dpuf
