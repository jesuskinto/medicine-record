#include "HorarioGenerator.h"
#include <fstream>
#include <sstream>
#include <algorithm>

HorarioGenerator::HorarioGenerator() {
}

bool HorarioGenerator::cargarMedicamentos(const std::string& archivo) {
    std::ifstream file(archivo);
    if (!file.is_open()) {
        return false;
    }
    
    medicamentos.clear();
    std::string linea;
    
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') {
            continue; // Ignorar líneas vacías y comentarios
        }
        
        std::istringstream iss(linea);
        std::string nombre;
        int frecuencia, duracion;
        
        if (iss >> nombre >> frecuencia >> duracion) {
            medicamentos.emplace_back(nombre, frecuencia, duracion);
        }
    }
    
    file.close();
    return true;
}

bool HorarioGenerator::cargarIncompatibilidades(const std::string& archivo) {
    std::ifstream file(archivo);
    if (!file.is_open()) {
        return false;
    }
    
    incompatibilidades.clear();
    std::string linea;
    
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') {
            continue; // Ignorar líneas vacías y comentarios
        }
        
        std::istringstream iss(linea);
        std::string med1, med2;
        
        if (iss >> med1 >> med2) {
            incompatibilidades.emplace_back(med1, med2);
        }
    }
    
    file.close();
    return true;
}

bool HorarioGenerator::sonIncompatibles(const std::string& med1, const std::string& med2) const {
    for (const auto& inc : incompatibilidades) {
        if (inc.esIncompatible(med1, med2)) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> HorarioGenerator::obtenerIncompatibles(const std::string& medicamento) const {
    std::vector<std::string> resultado;
    for (const auto& inc : incompatibilidades) {
        if (inc.involucra(medicamento)) {
            if (inc.getMedicamento1() == medicamento) {
                resultado.push_back(inc.getMedicamento2());
            } else {
                resultado.push_back(inc.getMedicamento1());
            }
        }
    }
    return resultado;
}

bool HorarioGenerator::verificarConflictoRecursivo(const std::vector<std::string>& medicamentosActuales,
                                                    const std::string& nuevoMedicamento,
                                                    int nivel) const {
    // Caso base: si el nivel es muy profundo, evitar recursión infinita
    if (nivel > 10) {
        return false;
    }
    
    // Verificar si el nuevo medicamento es incompatible con alguno de los actuales
    for (const auto& med : medicamentosActuales) {
        if (sonIncompatibles(med, nuevoMedicamento)) {
            return true;
        }
    }
    
    // Recursión: verificar incompatibilidades transitivas
    auto incompatibles = obtenerIncompatibles(nuevoMedicamento);
    for (const auto& incompatible : incompatibles) {
        for (const auto& med : medicamentosActuales) {
            if (med == incompatible) {
                return true;
            }
        }
    }
    
    return false;
}

std::vector<Dosis> HorarioGenerator::generarHorario(int dias) {
    std::vector<Dosis> horario;
    
    if (medicamentos.empty()) {
        return horario;
    }
    
    // Mapa: medicamento -> última hora real en que se tomó (incluye desplazamientos)
    // Inicializar con -1 para indicar que aún no se ha tomado ninguna dosis
    std::map<std::string, int> ultimaHoraReal;
    for (const auto& med : medicamentos) {
        ultimaHoraReal[med.getNombre()] = -1;
    }
    
    // Para cada día, generar el horario
    for (int dia = 0; dia < dias; ++dia) {
        // Mapa: hora -> lista de medicamentos que deben tomarse a esa hora (calculados desde hora real anterior)
        std::map<int, std::vector<std::string>> medicamentosPorHora;
        
        // Mapa: hora -> lista de medicamentos ya aceptados en esa hora (incluye movidos)
        std::map<int, std::vector<std::string>> medicamentosAceptadosPorHora;
        
        // Generar todas las dosis necesarias para este día según frecuencia
        // Pero calcular desde la última hora real, no desde hora teórica
        for (const auto& med : medicamentos) {
            // Solo incluir si el medicamento aún está en tratamiento
            if (dia < med.getDuracion()) {
                int horaInicio;
                
                // Si es el primer día o no se ha tomado ninguna dosis aún, empezar desde hora 0
                if (dia == 0 || ultimaHoraReal[med.getNombre()] == -1) {
                    horaInicio = 0;
                } else {
                    // Calcular desde la última hora real + frecuencia
                    // Si la última hora fue del día anterior, ajustar
                    horaInicio = ultimaHoraReal[med.getNombre()] + med.getFrecuencia();
                    
                    // Si la hora calculada es >= 24, significa que la siguiente dosis es del día siguiente
                    // En ese caso, empezar desde hora 0 de este día
                    if (horaInicio >= 24) {
                        horaInicio = horaInicio % 24;
                    }
                }
                
                // Calcular las horas en que debe tomarse según su frecuencia, desde la hora real
                int hora = horaInicio;
                while (hora < 24) {
                    medicamentosPorHora[hora].push_back(med.getNombre());
                    hora += med.getFrecuencia();
                }
            }
        }
        
        // Procesar cada hora del día, resolviendo incompatibilidades
        for (int hora = 0; hora < 24; ++hora) {
            // Usar cola para procesar medicamentos de esta hora
            ColaDosis<std::string> medicamentosPendientes;
            
            // Agregar medicamentos originalmente programados para esta hora
            if (medicamentosPorHora.find(hora) != medicamentosPorHora.end()) {
                for (const auto& med : medicamentosPorHora[hora]) {
                    medicamentosPendientes.encolar(med);
                }
            }
            
            // Procesar cada medicamento
            while (!medicamentosPendientes.estaVacia()) {
                std::string med = medicamentosPendientes.desencolar();
                
                // Obtener medicamentos ya aceptados en esta hora
                std::vector<std::string>& medicamentosAceptados = medicamentosAceptadosPorHora[hora];
                
                // Verificar si puede agregarse sin conflictos con los ya aceptados
                if (!verificarConflictoRecursivo(medicamentosAceptados, med)) {
                    medicamentosAceptados.push_back(med);
                    horario.push_back(Dosis(med, hora, dia));
                    // Actualizar la última hora real de este medicamento
                    ultimaHoraReal[med] = hora;
                } else {
                    // Si hay conflicto, buscar siguiente hora disponible
                    int siguienteHora = hora + 1;
                    bool colocado = false;
                    
                    // Buscar hora disponible (máximo hasta el final del día)
                    while (siguienteHora < 24 && !colocado) {
                        // Obtener medicamentos ya aceptados en la siguiente hora
                        std::vector<std::string>& medsEnSiguienteHora = medicamentosAceptadosPorHora[siguienteHora];
                        
                        // Verificar si este medicamento puede ir en la siguiente hora
                        if (!verificarConflictoRecursivo(medsEnSiguienteHora, med)) {
                            // Agregar a la siguiente hora (tanto a aceptados como a pendientes)
                            medsEnSiguienteHora.push_back(med);
                            horario.push_back(Dosis(med, siguienteHora, dia));
                            // Actualizar la última hora real de este medicamento
                            ultimaHoraReal[med] = siguienteHora;
                            colocado = true;
                        } else {
                            siguienteHora++;
                        }
                    }
                    
                    // Si no se pudo colocar en ninguna hora, agregarlo de todas formas
                    // (mejor tenerlo aunque haya conflicto que perder la dosis)
                    if (!colocado) {
                        medicamentosAceptados.push_back(med);
                        horario.push_back(Dosis(med, hora, dia));
                        // Actualizar la última hora real de este medicamento
                        ultimaHoraReal[med] = hora;
                    }
                }
            }
        }
    }
    
    // Ordenar horario por día y hora
    std::sort(horario.begin(), horario.end(), 
              [](const Dosis& a, const Dosis& b) {
                  if (a.dia != b.dia) return a.dia < b.dia;
                  return a.hora < b.hora;
              });
    
    return horario;
}

