#ifndef HORARIOGENERATOR_H
#define HORARIOGENERATOR_H

#include "Medicamento.h"
#include "Incompatibilidad.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>

struct Dosis {
    std::string medicamento;
    int hora;
    int dia;
    
    Dosis(const std::string& med, int h, int d) : medicamento(med), hora(h), dia(d) {}
};

template<typename T>
class ColaDosis {
private:
    std::queue<T> cola;
    
public:
    void encolar(const T& elemento) {
        cola.push(elemento);
    }
    
    T desencolar() {
        T elemento = cola.front();
        cola.pop();
        return elemento;
    }
    
    bool estaVacia() const {
        return cola.empty();
    }
    
    size_t tamano() const {
        return cola.size();
    }
};

template<typename T>
class PilaHorarios {
private:
    std::stack<T> pila;
    
public:
    void apilar(const T& elemento) {
        pila.push(elemento);
    }
    
    T desapilar() {
        T elemento = pila.top();
        pila.pop();
        return elemento;
    }
    
    bool estaVacia() const {
        return pila.empty();
    }
    
    size_t tamano() const {
        return pila.size();
    }
};

class HorarioGenerator {
public:
    HorarioGenerator();
    
    bool cargarMedicamentos(const std::string& archivo);
    bool cargarIncompatibilidades(const std::string& archivo);
    
    std::vector<Dosis> generarHorario(int dias = 7);
    
    std::vector<Medicamento> getMedicamentos() const { return medicamentos; }
    std::vector<Incompatibilidad> getIncompatibilidades() const { return incompatibilidades; }
    
private:
    std::vector<Medicamento> medicamentos;
    std::vector<Incompatibilidad> incompatibilidades;
    
    bool sonIncompatibles(const std::string& med1, const std::string& med2) const;
    std::vector<std::string> obtenerIncompatibles(const std::string& medicamento) const;
    
    // Función recursiva para verificar conflictos
    bool verificarConflictoRecursivo(const std::vector<std::string>& medicamentosActuales, 
                                     const std::string& nuevoMedicamento, 
                                     int nivel = 0) const;
};

#endif // HORARIOGENERATOR_H

