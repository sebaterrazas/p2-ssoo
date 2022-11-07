# Proyecto 2 - IIC2333


## Integrantes

-   Benjamín González - 17626730
-   Vicente Gavilán - 15623513
-   Tatsuya Yamaguchi - 16637941
-   Isidora Becker - 18640737
-   Fernanda Reyes - 18639860
-   Sebastián Terrazas - 20638078

## Descripción

## Bonus

### SSH Tunneling

### Descarga Imagen

Se creó una función de enviar y recibir imagen, la cual se basa en la función de enviar y recibir mensaje, pero con un manejo de archivos pertinente para leer y guardar la imagen.

### Conexión y Desconexión

Cuando ocurre una señal de término, como `CTRL + C`, se maneja esa señal en el cliente, donde antes de terminar se envía el mensaje `exit` al servidor. El método de autenticar al jugador es con su nombre, ya que todos los nombres son únicos, donde si alguien quiere crear otro usuario con un nombre ya usado, el servidor no lo permite. Cabe recalcar que los usuarios no tienen clave, por lo que cualquiera puede entrar a una cuenta offline si se sabe su nombre.