#include "Incompatibilidad.h"

Incompatibilidad::Incompatibilidad(const std::string& med1, const std::string& med2)
    : medicamento1(med1), medicamento2(med2) {
}

bool Incompatibilidad::involucra(const std::string& medicamento) const {
    return medicamento1 == medicamento || medicamento2 == medicamento;
}

bool Incompatibilidad::esIncompatible(const std::string& med1, const std::string& med2) const {
    return (medicamento1 == med1 && medicamento2 == med2) ||
           (medicamento1 == med2 && medicamento2 == med1);
}

