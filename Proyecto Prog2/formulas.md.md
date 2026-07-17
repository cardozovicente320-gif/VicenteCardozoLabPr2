# Especificación Matemática: Cálculos de Offsets en Archivos Binarios
**Proyecto:** Sistema de Gestión de Torneos - Fase 2  
**Desarrollador:** Vicente Cardozo  

---

## 1. El Concepto de Acceso Aleatorio (Random Access)
En lugar de leer secuencialmente el archivo binario completo para encontrar un registro (lo cual es sumamente ineficiente a medida que crece el tamaño del almacenamiento), el sistema calcula matemáticamente la posición física exacta del dato en el disco. Esto permite saltar inmediatamente al byte deseado utilizando los punteros del flujo de archivos[cite: 2].

---

## 2. Ecuación General del Offset en Bytes
Para cualquier archivo con registros de tamaño constante que posea un encabezado de control (`ArchivoHeader`) en el inicio[cite: 2], la dirección física de un registro en el índice $i$ (comenzando en $0$) se rige por la siguiente ecuación:

$$Offset_{bytes} = sizeof(ArchivoHeader) + (i \times sizeof(Entidad))$$

Donde:
*   $sizeof(ArchivoHeader)$ es el desplazamiento inicial constante para saltar los metadatos de control (16 bytes)[cite: 2].
*   $i$ es la posición lógica relativa del registro (0, 1, 2, ... N)[cite: 2].
*   $sizeof(Entidad)$ es la constante de tamaño de la estructura que se está leyendo o escribiendo en bytes (ej: 536 para Equipo)[cite: 2].

---

## 3. Casos de Aplicación en el Código

### Caso A: Lectura Directa de un Registro por Índice
Para colocar el puntero de lectura (`seekg`) en la posición del registro deseado:

```cpp
// Offset para leer el Jugador en la posición lógica 'index'
streamoff offsetBytes = sizeof(ArchivoHeader) + (index * sizeof(Jugador));
archivo.seekg(offsetBytes, ios::beg);