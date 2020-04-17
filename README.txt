Procedimiento de configuraci�n de ajustes del servo.

- Es imprescindible la consola serie (el que viene con el arduino IDE vale, bot�n superior derecha)

- Se utilizar� el bot�n de configuraci�n opcional.

Cargar el programa, y abrir la consola serie. Subir el programa al arduino.

En el momento en que el servo se mueva a la posici�n central, pulsar y mantener pulsado el bot�n hasta que en la consola veamos el mensaje siguiente:

Estado de configuraci�n???1
 En ese momento, soltar el bot�n. El arduino pasa a modo de configuraci�n.
 Al cabo de unos segundos se mostrar� el mensaje:
 
Coloque el servo, regulaci�n de la posici�n ESPIRANDO
Cuando tenga el �ngulo, pulse el bot�n

En ese momento, utilizando el potenci�metro de la frecuencia, ajustar el servo hasta que se encuentre en la posci�n 'espirando'.
Cuando est� en dicha posici�n, apretar y soltar el bot�n. Aparecer� el mensaje:

Bot�n pulsado
Grabando el la EEPROM el valor espirando, max_angle. Valor: 133
Coloque el servo, regulaci�n de la posici�n INSPIRANDO
Cuando tenga el �ngulo, pulse el bot�n
  
Ahora, con el mismo potenci�metro, llevar el servo a la posici�n de 'inspirando'. Cuando est�, aprotar y soltar de nuevo el bot�n. Aparecer� el mensaje:
Bot�n pulsado
Grabando el la EEPROM el valor inspirando, min_angle. Valor: 91
Bot�n pulsado

Ya estar�an configurados los recorridos, y se puede poner en marcha normalmente (sin pulsar ning�n bot�n).

Este proceso se puede repetir las veces que sean necesarias.