# self-driving-rc-car
Utilizamos motores Dc de 6v, con una bateria de 2s=7.4V. Y 2 ruedas traseras, giramos basado en la diferencia de velocidad entre ambas ruedas.
Sensores de luz QTR8A y un ultrasonid HC-04(opcional) para evitar golpes frontales o para parar las pruebas.

Se puede añadir un módulo bluetooth para cambiar los valores de velocidad y no tener que ir cambiando manualmente.

Próximamente añadiremos un codigo basado en PID que será mas complicado de poner en marcha pero muchísimo más eficaz.

El código normal es un algoritmo voraz, simple, sencillo de entender y que funcionaría en pocos minutos, lo mejor para empezar y tomar de referencia.

El código del pid es muy sensible a cambios, para evitar errores puntuales o que lecturas anómalas destruyan el programa, he creado unos pequeños trucos dentro del programa donde explico perfecatmente como solventarlos.
Lo dificil del PID es conseguir sacar los valores proporcionales, sin un modelo matemático en MAtlab, lo que mejor me ha funcionado ha sido así:

Kd=1
Kp=0
Ki=0;

aumentar la kd, +-0.5 hasta que se mantenga en el carril, cuando lo haga reducelo un 40% y aumenta la kp a 0.15
la ki, se debe ir aumentandi cuando los valores de kd y kp den un resultado relativamente optimo, ki+-0.02.
Está basado en parte en el método de Ziegler-Nichols, pero simplificado, escribidme si queréis sacar los cálculos vosotros o revisarlo.
Os dejo toda la info que he encontrado.
https://sites.google.com/site/picuino/ziegler-nichols
Bien explicado y con algun programa matemático se saca facil.
