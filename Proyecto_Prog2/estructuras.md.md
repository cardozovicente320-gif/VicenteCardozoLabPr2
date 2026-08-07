# Reporte Técnico: Estructuras de Datos y Tamaños en Memoria (sizeof)
**Proyecto:** Sistema de Gestión de Torneos - Fase 2  
**Desarrollador:** Vicente Cardozo  
**Fecha:** Julio 2026  

---

## 1. Introducción a la Persistencia Estática
Para garantizar que el software pueda realizar operaciones de lectura y escritura directa "bit a bit" en el disco duro, todas las estructuras de datos principales se han diseñado con un tamaño fijo y estático en memoria. Se eliminaron estructuras dinámicas como `std::string` en favor de arreglos de caracteres estáticos (`char[]`) para asegurar la consistencia del tamaño de registro (`sizeof`).

---

## 2. Tabla General de Tamaños en bytes (`sizeof`)

| Estructura | Tamaño Estático (Bytes)* | Descripción del Contenido y Atributos de Control |
| :--- | :---: | :--- |
| **`ArchivoHeader`** | **16 bytes** | Control interno del archivo binario: cantidad total de registros, IDs autoincrementales y registros activos. |
| **`Torneo`** | **198 bytes** | Metadatos únicos del torneo (Nombre, Deporte, Formato, Fechas de control temporales). |
| **`Gol`** | **20 bytes** | Estructura interna de apoyo para el registro detallado de anotaciones (ID, Minuto, Equipo). |
| **`Equipo`** | **536 bytes** | Datos de club, estadísticas de liga, historial de IDs de partidos y metadatos de auditoría. |
| **`Jugador`** | **184 bytes** | Atributos de jugador, estadísticas individuales, claves foráneas y flag de borrado lógico. |
| **`Partido`** | **900 bytes** | Datos de encuentro, marcador, historial de hasta 22 anotaciones (estructuras `Gol`) y auditoría. |

*\*Nota técnica:* Los tamaños reflejados corresponden a la compilación estándar en sistemas de arquitectura de 64 bits (x86_64), contemplando el alineamiento de memoria (*padding*) que añade el compilador GCC.

---

## 3. Desglose Detallado de Atributos por Estructura

### A. Estructura `Equipo` (`sizeof: 536 bytes`)
*   `id` (int): 4 bytes
*   `nombre` (char[100]): 100 bytes
*   `ciudad` (char[100]): 100 bytes
*   `entrenador` (char[100]): 100 bytes
*   Estadísticas lógicas (`puntos`, `victorias`, `empates`, `derrotas`, `golesAFavor`, `golesEnContra`): 24 bytes (6 x 4 bytes)[cite: 3]
*   `partidosIDs` (int[50]): 200 bytes[cite: 3]
*   `cantidadPartidos` (int): 4 bytes[cite: 3]
*   Campos de control (`eliminado` [bool], `fechaCreacion` [time_t], `fechaUltimaModificacion` [time_t]): 17 bytes (1 + 8 + 8)[cite: 2, 3]

### B. Estructura `Jugador` (`sizeof: 184 bytes`)[cite: 3]
*   `id` (int): 4 bytes[cite: 3]
*   `idEquipo` (int): 4 bytes[cite: 3]
*   `nombre` (char[100]): 100 bytes[cite: 3]
*   `cedula` (char[20]): 20 bytes[cite: 3]
*   `posicion` (char[20]): 20 bytes[cite: 3]
*   `edad` (int) y `numeroDorsal` (int): 8 bytes[cite: 3]
*   Estadísticas (`golesAnotados`, `tarjetasAmarillas`, `tarjetasRojas`): 12 bytes[cite: 3]
*   Campos de control (`eliminado`, `fechaCreacion`, `fechaUltimaModificacion`): 17 bytes[cite: 2, 3]

### C. Estructura `Partido` (`sizeof: 900 bytes`)[cite: 3]
*   `id`, `idEquipoLocal`, `idEquipoVisitante` (int): 12 bytes[cite: 3]
*   `fecha` (char[11]): 11 bytes[cite: 3]
*   `estado` (char[12]): 12 bytes[cite: 3]
*   `descripcion` (char[200]): 200 bytes[cite: 3]
*   `golesLocal` (int), `golesVisitante` (int): 8 bytes[cite: 3]
*   `goles` (Gol[22]): 440 bytes (22 x 20 bytes)[cite: 3]
*   `numGoles` (int): 4 bytes[cite: 3]
*   Campos de control (`eliminado`, `fechaCreacion`, `fechaUltimaModificacion`): 17 bytes[cite: 2, 3]