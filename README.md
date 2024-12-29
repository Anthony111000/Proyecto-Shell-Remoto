# Proyecto-Shell-Remoto

Este proyecto implementa una shell remota que permite ejecutar comandos de manera remota a través de una conexión segura. Es ideal para aprender sobre interacción con sistemas remotos y manipulación de procesos en C.

## Descripción

El objetivo del proyecto es proporcionar una forma de ejecutar comandos en una máquina remota utilizando funcionalidades básicas de red y el sistema operativo. Incluye:

- Conexión cliente-servidor.
- Ejecución de comandos enviados desde un cliente hacia el servidor.
- Manejo de la salida de los comandos ejecutados.

## Estructura del Proyecto

El proyecto incluye los siguientes archivos clave:

- `server.c`: Implementa el servidor que escucha peticiones y ejecuta comandos.
- `client.c`: Implementa el cliente que se conecta al servidor y envía comandos.
- `README.md`: Instrucciones y documentación del proyecto.

## Instalación

### Requisitos Previos

- Un sistema operativo basado en Unix/Linux.
- Compilador GCC.

### Instrucciones

1. Clona el repositorio:
   ```bash
   git clone https://github.com/Anthony111000/Proyecto-Shell-Remoto.git
   cd Proyecto-Shell-Remoto
