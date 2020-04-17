Procedimiento de configuración de ajustes del servo.

- Es imprescindible la consola serie (el que viene con el arduino IDE vale, botón superior derecha)

- Se utilizará el botón de configuración opcional.

Cargar el programa, y abrir la consola serie. Subir el programa al arduino.

En el momento en que el servo se mueva a la posición central, pulsar y mantener pulsado el botón hasta que en la consola veamos el mensaje siguiente:

Estado de configuración???1
 En ese momento, soltar el botón. El arduino pasa a modo de configuración.
 Al cabo de unos segundos se mostrará el mensaje:
 
Coloque el servo, regulación de la posición ESPIRANDO
Cuando tenga el ángulo, pulse el botón

En ese momento, utilizando el potenciómetro de la frecuencia, ajustar el servo hasta que se encuentre en la posción 'espirando'.
Cuando esté en dicha posición, apretar y soltar el botón. Aparecerá el mensaje:

Botón pulsado
Grabando el la EEPROM el valor espirando, max_angle. Valor: 133
Coloque el servo, regulación de la posición INSPIRANDO
Cuando tenga el ángulo, pulse el botón
  
Ahora, con el mismo potenciómetro, llevar el servo a la posición de 'inspirando'. Cuando esté, aprotar y soltar de nuevo el botón. Aparecerá el mensaje:
Botón pulsado
Grabando el la EEPROM el valor inspirando, min_angle. Valor: 91
Botón pulsado

Ya estarían configurados los recorridos, y se puede poner en marcha normalmente (sin pulsar ningún botón).

Este proceso se puede repetir las veces que sean necesarias.