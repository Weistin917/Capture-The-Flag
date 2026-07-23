# **Captura la Bandera** 

### **Reglamento básico** 

### **Objetivo** 

Cada jugador compite de forma individual. El objetivo es: 

1. Entrar al círculo central. 

2. Tomar la bandera. 

3. Salir completamente del círculo mientras continúa llevando la bandera. 

El primer jugador que lo consiga gana la partida. 

### **Inicio de la partida** 

- Existe una única bandera ubicada exactamente en el centro del mapa. 

- Todos los jugadores aparecen en posiciones aleatorias fuera del círculo. 

- Ningún jugador inicia con la bandera. 

- La partida comienza cuando el servidor envía la señal de inicio. 

### **Movimiento** 

Los jugadores pueden desplazarse libremente por el mapa utilizando los controles que se configuren en el teclado. 

Algunos conceptos que pueden enviar o recibir de los jugadores (suelen ser más) 

- Nombre 

- Posición 

- Dirección 

- Estado del jugador 

- Colisión/Interacción/Acción 

### **Captura de la bandera** 

Cuando un jugador se encuentra suficientemente cerca de la bandera, puede presionar la tecla de interacción (por ejemplo “ **E”** , esto puede ser totalmente arbitrario de cada proyecto). 

Si la bandera está libre: 

- pasa inmediatamente a pertenecer a ese jugador. 

A partir de ese momento: 

- La bandera deja de estar en el suelo; 

- La bandera acompaña al jugador. 

### **Robo de la bandera** 

Si un jugador posee la bandera, cualquier otro jugador puede robársela. 

Condiciones: 

- Debe estar suficientemente cerca del portador. 

- Debe presionar la tecla de interacción. 

Si ambas condiciones se cumplen: 

- la bandera cambia inmediatamente de propietario. 

No existe tiempo de espera. 

No existe inmunidad. 

El robo es instantáneo. 

Si existen más jugadores cerca, solo valide el primero y luego deben de presionar la tecla para robar al nuevo portador. 



## **Implementación del Proyecto** 

- Este proyecto se realizará de forma individual 

- El desarrollo del proyecto puede ser en cualquier lenguaje de programación. 

- El proyecto puede utilizar cualquier librería que el lenguaje de programación le permite utilizar, como también utilizar sockets. 

- La interfaz gráfica puede realizarla con lo que le acomode según el lenguaje de programación que haya escogido, con que cumpla lo básico del juego de Captura la Bandera. 

- Su proyecto debe de comportarse como servidor o cliente de otro juego. 

- Su proyecto debe de poder soportar N cantidad de usuarios conectados, (límite 100) 

- Puede utilizar cualquier inteligencia artificial para apoyarse en su aprendizaje del lenguaje de programación o alguna implementación que desee aprender. 

#### **<mark>Limitaciones de implementación</mark>** 

- <mark>No pueden existir más de 4 proyectos que estén desarrollados en un mismo lenguaje de programación.</mark> 

- <mark>No pueden existir más de 2 proyectos que utilicen la misma librería de conexión o de generación de gráficas. La excepción a la regla es utilizar sockets básicos.</mark> 

- <mark>Cuando el Proyecto se configure como servidor solo debe mostrar el juego de todos los jugadores, solo cuando está configurado como cliente a conectarse a un servidor es posible jugar en en esa máquina.</mark> 

#### **Recomendaciones:** 

- Utilice Broadcast para la comunicación general, para poder descubrir usuarios que se quieran conectar a un proyecto que se esté configurado como servidor e iniciar una nueva partida y tener un Countdown para el inicio de la partida, para la confirmación de inicio del juego. 

- Las validaciones que existan se deben de cumplir para que un jugador pueda ganar, estas las vamos hacer en el modo servidor y en el modo cliente vamos a reaccionar a las banderas que envié el servidor para cambiar el estado del juego. 

- Todos los jugadores deben de poder ver su movimiento en todos los proyectos cliente conectados no solamente en el servidor. 

#### **Entregable** 

- Juego funcional 

   - Servidor Soporte multijugador y visualización de jugadores y validación 

   - Cliente con soporte de descubrir servidor a conectarse y poder unirse a una partida que aún no haya iniciado, validando los estados y mostrando todos los jugadores conectados. 

- Entorno Gráfico 

   - Entorno Web 

   - 3D 

   - 2D 

   - ASCII 

   - etc 

- Soporte de Múltiples conexiones. 

   - Debe de poder soportar hasta 100 conexiones, 

   - El límite real es la cantidad de proyectos entregados. 

   - <u>Todos los proyectos deben poder conectarse entre sí, no existe excepción de que algunos se conecten (Proyecto Funcional), si 2 o la minoría que se “logren comunicar bien” o “se conectan bien” esos proyectos se calificará con cero. Eso quiere decir que no existen grupos en esta asignación, en otras palabras el grupo es la Clase.</u> 

- Documentación de implementación, desde la versión 1 

   - Desde el día 1 de inicio de desarrollo hasta el día final de la implementación. 

   - Debe de tener todas las versiones de cambio de ideas dentro del documento y referenciado a GIT. 

   - Explicación de las conexiones que se realizan para lograr la comunicación entre todos los otros proyectos. 

   - Utilización de GIT para tener cronología de progreso y forma de LOG que soporta la Documentación. 

   - Inteligencia Artificial utilizada y Prompts que utilice para generar cierta parte de código o implementación que esté generando. 

