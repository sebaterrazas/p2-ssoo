# Proyecto 2 - IIC2333


## Integrantes

-   Isidora Becker - 18640737
-   Vicente Gavilán - 15623513
-   Benjamín González - 17626730
-   Fernanda Reyes - 18639860
-   Sebastián Terrazas - 20638078
-   Tatsuya Yamaguchi - 16637941


## Ejecución

Para poder ejecutar el juego se debe correr los siguientes comandos desde la terminal tanto del cliente como del servidor:
´´´´´´
./server -i 127.0.0.1 -p 8000
´´´

´´´
./client -i 127.0.0.1 -p 8000
´´´

## Uso del programa

Una vez establecida la conexión entre el cliente y el servidor, cada jugador se dirige al lobby para luego trasladarse a la sala de juego. A medida que avanza el programa se van indicando por medio de la consola los comandos que se esperan recibir para continuar la ejecución del juego.

## Paquetes utilizados en la comunicación

No aplica

## Principales funciones del programa

Siguiendo las recomendaciones de diseño para construir el programa indicadas en el enunciado se crearon diversas funciones, dentro de las cuales las más importantes son handle_communication que permite manejar la comunicación entre cliente y servidor de manera robusta.Tambien, mostrar_lobby, start_game y reconnect_msg las cuales permiten las condiciones necesarias para el inicio del juego y reconexión en caso de ser necesario. Por otra parte para la ejececución del juego se utilizaron las siguientes funciones: mostrar_tablero, iniciar_tablero, colocar_barco, validar_coordenadas, pedir_coordenadas, pedir_disparo, verificar_disparo, pedir_confirmación las cuales permiten que la dinámica de juego estilo battleship se implemente de manera correcta.

## Bonus

### SSH Tunneling

### Descarga Imagen

Se creó una función de enviar y recibir imagen, la cual se basa en la función de enviar y recibir mensaje, pero con un manejo de archivos pertinente para leer y guardar la imagen.

### Conexión y Desconexión

Cuando ocurre una señal de término, como `CTRL + C`, se maneja esa señal en el cliente, donde antes de terminar se envía el mensaje `exit` al servidor. El método de autenticar al jugador es con su nombre, ya que todos los nombres son únicos, donde si alguien quiere crear otro usuario con un nombre ya usado, el servidor no lo permite. Cabe recalcar que los usuarios no tienen clave, por lo que cualquiera puede entrar a una cuenta offline si se sabe su nombre.
