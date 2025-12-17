# **Proyecto de Sistemas Operativos: Extensiones al Kernel XV6**

**Universidad Nacional Jorge Basadre Grohmann** **Facultad de Ingeniería** **Escuela Profesional de Ingeniería en Informática y Sistemas**

## **📋 Información del Proyecto**

* **Asignatura:** Sistemas Operativos  
* **Semestre:** 2025-II  
* **Unidad:** 02 \- Evaluación de Producto

### **👥 Integrantes**

* Fabian Arturo Vargas Quispe		2022-119095  
* Sebastian Joshua Quispe Condori		2023-119065

## **📝 Descripción**

Este proyecto consiste en la modificación y extensión del sistema operativo educativo **XV6** (una reimplementación moderna de Unix v6 desarrollada por el MIT).

El objetivo principal es interactuar con un kernel real para implementar nuevas funcionalidades que permitan una mejor observabilidad y gestión del sistema. Las modificaciones realizadas abarcan:

1. **Instrumentación del Kernel:** Rastreo de llamadas al sistema en tiempo real.  
2. **Nuevos Utilitarios de Usuario:** Comandos para inspeccionar procesos, inodos y tiempo de actividad.  
3. **Estadísticas de Uso:** Sistema de contabilidad ("accounting") para medir la frecuencia de ejecución de las system calls.

Estas extensiones aplican conceptos teóricos de gestión de procesos, memoria virtual y sistemas de archivos.

## **🚀 Instrucciones de Instalación y Compilación**

### **Requisitos Previos**

Para ejecutar XV6, necesitas un entorno Linux (o WSL en Windows) con las siguientes herramientas instaladas:

* qemu-system-i386 (Emulador)  
* gcc (Compilador)  
* make (Construcción)

### **Pasos para Compilar y Ejecutar**

1. **Clonar el repositorio:**  
   git clone https://github.com/ESIS-DevTeam/ProyectoFinal-SO-ESIS-VI-2025.git  
   cd ProyectoFinal-SO-ESIS-VI-2025

2. **Limpiar compilaciones previas (Opcional pero recomendado):**  
   make clean

3. **Compilar y ejecutar en QEMU:**  
   make qemu

   *(Nota: Si no deseas interfaz gráfica, usa make qemu-nox)*.

## **🎮 Guía de Uso de los Nuevos Comandos**

Una vez dentro de la consola de XV6 (QEMU), puedes probar las siguientes funcionalidades implementadas:

### **1\. Rastreo de Llamadas (trace)**

Activa o desactiva la impresión en consola de cada syscall ejecutada por el sistema.

* **Activar:**  
  $ trace 1

* **Desactivar:**  
  $ trace 0

### **2\. Información del Sistema**

* **uptime**: Muestra los *ticks* del reloj del sistema desde el arranque.  
  $ uptime

* **lsx (Listado Extendido)**: Muestra archivos con metadatos adicionales (Tipo, Inodo, Tamaño).  
  $ lsx

* **ps (Estado de Procesos)**: Lista todos los procesos activos, su PID y su estado actual.  
  $ ps

### **3\. Estadísticas (syscount)**

Muestra cuántas veces se ha llamado a cada system call desde el inicio.

* **Ver todo el listado:**  
  $ syscount

* **Ver conteo de una syscall específica por nombre:**  
  $ syscount write

* **Ver conteo por ID:**  
  $ syscount 16

## **📂 Archivos Modificados**

Los cambios principales se realizaron en los siguientes archivos del código fuente:

* **Kernel Space:**  
  * syscall.c / syscall.h: Lógica de intercepción (trace) y contabilidad (syscount).  
  * sysproc.c: Implementación de las nuevas syscalls (sys\_uptime, sys\_trace, etc.).  
  * proc.c / proc.h: Modificaciones a la estructura struct proc y acceso a la ptable.  
  * defs.h: Definiciones de nuevas funciones del kernel.  
* **User Space:**  
  * user.h: Definición de las nuevas syscalls para programas de usuario.  
  * usys.S: Macros de ensamblador para las syscalls.  
  * trace.c, ps.c, lsx.c, syscount.c, uptime.c: Código fuente de los nuevos comandos.

## **📚 Referencias**

* **MIT XV6:** "Xv6, a Simple Unix-like Teaching Operating System". [Enlace MIT](http://pdos.csail.mit.edu/6.828/2014/xv6.html).  
* **Material del Curso:** Guías de laboratorio y diapositivas de la Unidad II (Sistemas Operativos 2025-II).  
* **Bibliografía:** Tanenbaum, A. S. (2006). *Sistemas Operativos Modernos*.