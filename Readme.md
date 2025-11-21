# Gestor de Horarios de Medicamentos

Aplicación de escritorio desarrollada en C++ con Qt para gestionar horarios de medicamentos, evitando mezclar medicamentos incompatibles.

## Características

- Carga de medicamentos desde archivo de texto
- Carga de incompatibilidades entre medicamentos
- Generación automática de horarios evitando conflictos
- Interfaz gráfica intuitiva con tablas
- Exportación de horarios a archivo de texto

## Requisitos

- C++17 o superior
- CMake 3.16 o superior
- Qt 6 (o Qt 5 compatible)
- Compilador compatible (GCC, Clang, MSVC)

## Estructura del Proyecto

```
medicine-record/
├── CMakeLists.txt          # Configuración de CMake
├── src/
│   ├── main.cpp            # Punto de entrada
│   ├── MainWindow.h/cpp    # Ventana principal
│   ├── Medicamento.h/cpp    # Clase para medicamentos
│   ├── Incompatibilidad.h/cpp  # Clase para incompatibilidades
│   └── HorarioGenerator.h/cpp  # Generador de horarios
└── data/
    ├── medicamentos.txt         # Ejemplo de medicamentos
    └── incompatibilidades.txt   # Ejemplo de incompatibilidades
```

## Compilación

### Linux/macOS

```bash
mkdir build
cd build
cmake ..
make
```

### Windows (con Visual Studio)

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build .
```

## Uso

1. Ejecutar la aplicación: `./MedicineRecord` (o `MedicineRecord.exe` en Windows)

2. Cargar medicamentos:
   - Click en "Cargar Medicamentos"
   - Seleccionar archivo con formato: `Nombre Frecuencia Duracion`
   - Ejemplo: `Paracetamol 6 7` (cada 6 horas, durante 7 días)

3. Cargar incompatibilidades:
   - Click en "Cargar Incompatibilidades"
   - Seleccionar archivo con formato: `Medicamento1 Medicamento2`
   - Ejemplo: `Paracetamol Ibuprofeno`

4. Generar horario:
   - Click en "Generar Horario"
   - El sistema generará un horario evitando mezclar medicamentos incompatibles

5. Guardar horario:
   - Click en "Guardar Horario" para exportar a archivo de texto

## Formato de Archivos

### medicamentos.txt

```
# Comentarios empiezan con #
# Formato: Nombre Frecuencia(horas) Duracion(dias)
Paracetamol 6 7
Ibuprofeno 8 5
Aspirina 12 3
```

### incompatibilidades.txt

```
# Comentarios empiezan con #
# Formato: Medicamento1 Medicamento2
Paracetamol Ibuprofeno
Aspirina Ibuprofeno
```

## Características Técnicas

El proyecto utiliza:

- **Templates**: `ColaDosis` y `PilaHorarios` para gestión de estructuras de datos
- **Pilas (Stacks)**: Para organizar medicamentos por hora
- **Colas (Queues)**: Para gestionar dosis pendientes
- **Recursividad**: Función `verificarConflictoRecursivo` para validar incompatibilidades
- **Punteros**: Gestión de objetos Qt y memoria
- **Lectura de archivos**: Carga de medicamentos e incompatibilidades desde archivos de texto
- **STL**: Uso exclusivo de la biblioteca estándar de C++ junto con Qt

## Algoritmo de Generación de Horarios

1. Se cargan todos los medicamentos y sus incompatibilidades
2. Se generan todas las dosis necesarias según frecuencia y duración
3. Se organizan las dosis por hora usando una cola
4. Para cada hora, se verifica que no haya conflictos entre medicamentos
5. Si hay conflicto, se desplaza la dosis a la siguiente hora disponible
6. El resultado se ordena por día y hora

## Licencia

Este proyecto es de código abierto y está disponible para uso educativo.

