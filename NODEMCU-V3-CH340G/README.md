# NODEMCU

<div style="display: flex">
    <img src="./images/board.avif" style="margin: auto;" />
</div>

## Descripción general

## Instalación en arduino

1. Agregar la URL del ESP8266
Ve a **File → Preferences** y en "Additional boards manager URLs" agrega esta URL:
```
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

2. Instalar el paquete
Ve a **Tools → Board → Boards Manager**, busca esp8266 e instala "esp8266 by ESP8266 Community".

3. Seleccionar tu placa
En **Tools → Board → esp8266** elige tu modelo:

* NodeMCU 1.0 (ESP-12E) → el más común, placa azul con antena
* Wemos D1 Mini → la pequeña cuadrada
* Generic ESP8266 Module → si tienes solo el módulo sin placa

4. Driver USB
El ESP8266 usa los mismos chips que el ESP32, así que si ya instalaste el driver antes ya está listo. Si no:

* [CH340G](drivers\CH34x_Install_Windows_v3_4.zip)
* [CP210](drivers\CP210x_Windows_Drivers.zip)

## Pinout

## Proyectos
