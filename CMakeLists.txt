cmake_minimum_required(VERSION 3.16)
project(MedicineRecord VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar Qt6, si no está disponible, usar Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/Medicamento.cpp
    src/Incompatibilidad.cpp
    src/HorarioGenerator.cpp
    src/MainWindow.cpp
)

# Archivos de cabecera
set(HEADERS
    src/Medicamento.h
    src/Incompatibilidad.h
    src/HorarioGenerator.h
    src/MainWindow.h
)

# Archivos UI (si usamos .ui files)
set(UI_FILES
    src/MainWindow.ui
)

# Crear ejecutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES})

# Enlazar con Qt
target_link_libraries(${PROJECT_NAME} 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Configuración de instalación
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

