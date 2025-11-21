#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "HorarioGenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cargarMedicamentos();
    void cargarIncompatibilidades();
    void generarHorario();
    void guardarHorario();

private:
    void actualizarTablaMedicamentos();
    void actualizarTablaHorario(const std::vector<Dosis>& horario);
    void limpiarTablas();
    
    HorarioGenerator* generador;
    QTableWidget* tablaMedicamentos;
    QTableWidget* tablaHorario;
    QPushButton* btnCargarMedicamentos;
    QPushButton* btnCargarIncompatibilidades;
    QPushButton* btnGenerarHorario;
    QPushButton* btnGuardarHorario;
    QLabel* labelEstado;
    
    QString archivoMedicamentos;
    QString archivoIncompatibilidades;
};

#endif // MAINWINDOW_H

