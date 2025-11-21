#ifndef INCOMPATIBILIDAD_H
#define INCOMPATIBILIDAD_H

#include <string>
#include <vector>

class Incompatibilidad {
public:
    Incompatibilidad(const std::string& med1, const std::string& med2);
    
    std::string getMedicamento1() const { return medicamento1; }
    std::string getMedicamento2() const { return medicamento2; }
    
    bool involucra(const std::string& medicamento) const;
    bool esIncompatible(const std::string& med1, const std::string& med2) const;
    
private:
    std::string medicamento1;
    std::string medicamento2;
};

#endif // INCOMPATIBILIDAD_H

