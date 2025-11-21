#ifndef MEDICAMENTO_H
#define MEDICAMENTO_H

#include <string>
#include <vector>

class Medicamento {
public:
    Medicamento(const std::string& nombre, int frecuencia, int duracion);
    
    std::string getNombre() const { return nombre; }
    int getFrecuencia() const { return frecuencia; }
    int getDuracion() const { return duracion; }
    
    void setNombre(const std::string& n) { nombre = n; }
    void setFrecuencia(int f) { frecuencia = f; }
    void setDuracion(int d) { duracion = d; }
    
private:
    std::string nombre;
    int frecuencia;  // Horas entre dosis
    int duracion;    // Días de tratamiento
};

#endif // MEDICAMENTO_H

