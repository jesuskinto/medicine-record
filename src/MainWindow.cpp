#include "MainWindow.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), generador(new HorarioGenerator()) {
    
    setWindowTitle("Gestor de Horarios de Medicamentos");
    setMinimumSize(1000, 600);
    
    // Widget central
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Etiqueta de estado
    labelEstado = new QLabel("Listo para cargar medicamentos e incompatibilidades", this);
    mainLayout->addWidget(labelEstado);
    
    // Botones de control
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    btnCargarMedicamentos = new QPushButton("Cargar Medicamentos", this);
    btnCargarIncompatibilidades = new QPushButton("Cargar Incompatibilidades", this);
    btnGenerarHorario = new QPushButton("Generar Horario", this);
    btnGuardarHorario = new QPushButton("Guardar Horario", this);
    
    btnGenerarHorario->setEnabled(false);
    btnGuardarHorario->setEnabled(false);
    
    buttonLayout->addWidget(btnCargarMedicamentos);
    buttonLayout->addWidget(btnCargarIncompatibilidades);
    buttonLayout->addWidget(btnGenerarHorario);
    buttonLayout->addWidget(btnGuardarHorario);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Layout horizontal para tablas de medicamentos e incompatibilidades
    QHBoxLayout* tablasLayout = new QHBoxLayout();
    
    // Tabla de medicamentos
    QVBoxLayout* medicamentosLayout = new QVBoxLayout();
    QLabel* labelMedicamentos = new QLabel("Medicamentos Cargados:", this);
    medicamentosLayout->addWidget(labelMedicamentos);
    
    tablaMedicamentos = new QTableWidget(this);
    tablaMedicamentos->setColumnCount(3);
    tablaMedicamentos->setHorizontalHeaderLabels(QStringList() << "Nombre" << "Frecuencia (horas)" << "Duración (días)");
    tablaMedicamentos->horizontalHeader()->setStretchLastSection(true);
    tablaMedicamentos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    medicamentosLayout->addWidget(tablaMedicamentos);
    
    // Tabla de incompatibilidades
    QVBoxLayout* incompatibilidadesLayout = new QVBoxLayout();
    QLabel* labelIncompatibilidades = new QLabel("Incompatibilidades Cargadas:", this);
    incompatibilidadesLayout->addWidget(labelIncompatibilidades);
    
    tablaIncompatibilidades = new QTableWidget(this);
    tablaIncompatibilidades->setColumnCount(2);
    tablaIncompatibilidades->setHorizontalHeaderLabels(QStringList() << "Medicamento 1" << "Medicamento 2");
    tablaIncompatibilidades->horizontalHeader()->setStretchLastSection(true);
    tablaIncompatibilidades->setEditTriggers(QAbstractItemView::NoEditTriggers);
    incompatibilidadesLayout->addWidget(tablaIncompatibilidades);
    
    // Añadir ambas tablas al layout horizontal
    tablasLayout->addLayout(medicamentosLayout);
    tablasLayout->addLayout(incompatibilidadesLayout);
    mainLayout->addLayout(tablasLayout);
    
    // Tabla de horario
    QLabel* labelHorario = new QLabel("Horario Generado:", this);
    mainLayout->addWidget(labelHorario);
    
    tablaHorario = new QTableWidget(this);
    tablaHorario->setColumnCount(3);
    tablaHorario->setHorizontalHeaderLabels(QStringList() << "Día" << "Hora" << "Medicamento");
    tablaHorario->horizontalHeader()->setStretchLastSection(true);
    tablaHorario->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(tablaHorario);
    
    // Conectar señales
    connect(btnCargarMedicamentos, &QPushButton::clicked, this, &MainWindow::cargarMedicamentos);
    connect(btnCargarIncompatibilidades, &QPushButton::clicked, this, &MainWindow::cargarIncompatibilidades);
    connect(btnGenerarHorario, &QPushButton::clicked, this, &MainWindow::generarHorario);
    connect(btnGuardarHorario, &QPushButton::clicked, this, &MainWindow::guardarHorario);
}

MainWindow::~MainWindow() {
    delete generador;
}

void MainWindow::cargarMedicamentos() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Cargar Medicamentos", "", "Archivos de Texto (*.txt);;Todos los archivos (*.*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    archivoMedicamentos = fileName;
    
    if (generador->cargarMedicamentos(fileName.toStdString())) {
        actualizarTablaMedicamentos();
        labelEstado->setText("Medicamentos cargados correctamente");
        
        // Habilitar botón de generar si también hay incompatibilidades
        if (!archivoIncompatibilidades.isEmpty()) {
            btnGenerarHorario->setEnabled(true);
        }
    } else {
        QMessageBox::warning(this, "Error", "No se pudo cargar el archivo de medicamentos");
        labelEstado->setText("Error al cargar medicamentos");
    }
}

void MainWindow::cargarIncompatibilidades() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Cargar Incompatibilidades", "", "Archivos de Texto (*.txt);;Todos los archivos (*.*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    archivoIncompatibilidades = fileName;
    
    if (generador->cargarIncompatibilidades(fileName.toStdString())) {
        actualizarTablaIncompatibilidades();
        labelEstado->setText("Incompatibilidades cargadas correctamente");
        
        // Habilitar botón de generar si también hay medicamentos
        if (!archivoMedicamentos.isEmpty()) {
            btnGenerarHorario->setEnabled(true);
        }
    } else {
        QMessageBox::warning(this, "Error", "No se pudo cargar el archivo de incompatibilidades");
        labelEstado->setText("Error al cargar incompatibilidades");
    }
}

void MainWindow::generarHorario() {
    if (archivoMedicamentos.isEmpty() || archivoIncompatibilidades.isEmpty()) {
        QMessageBox::warning(this, "Advertencia", 
            "Debe cargar tanto medicamentos como incompatibilidades antes de generar el horario");
        return;
    }
    
    // Calcular el número máximo de días basado en la duración máxima de los medicamentos
    auto medicamentos = generador->getMedicamentos();
    int maxDias = 7; // Por defecto 7 días
    if (!medicamentos.empty()) {
        maxDias = medicamentos[0].getDuracion();
        for (const auto& med : medicamentos) {
            if (med.getDuracion() > maxDias) {
                maxDias = med.getDuracion();
            }
        }
    }
    
    std::vector<Dosis> horario = generador->generarHorario(maxDias);
    
    if (horario.empty()) {
        QMessageBox::information(this, "Información", 
            "No se pudo generar un horario. Verifique los datos cargados.");
        labelEstado->setText("No se pudo generar horario");
        return;
    }
    
    actualizarTablaHorario(horario);
    btnGuardarHorario->setEnabled(true);
    labelEstado->setText(QString("Horario generado: %1 dosis programadas").arg(horario.size()));
}

void MainWindow::guardarHorario() {
    QString fileName = QFileDialog::getSaveFileName(this,
        "Guardar Horario", "", "Archivos de Texto (*.txt);;Todos los archivos (*.*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        QMessageBox::warning(this, "Error", "No se pudo crear el archivo");
        return;
    }
    
    file << "HORARIO DE MEDICAMENTOS\n";
    file << "======================\n\n";
    
    for (int row = 0; row < tablaHorario->rowCount(); ++row) {
        QString dia = tablaHorario->item(row, 0)->text();
        QString hora = tablaHorario->item(row, 1)->text();
        QString medicamento = tablaHorario->item(row, 2)->text();
        
        file << "Día " << dia.toStdString() << ", Hora " << hora.toStdString() 
             << ": " << medicamento.toStdString() << "\n";
    }
    
    file.close();
    QMessageBox::information(this, "Éxito", "Horario guardado correctamente");
    labelEstado->setText("Horario guardado en: " + fileName);
}

void MainWindow::actualizarTablaMedicamentos() {
    tablaMedicamentos->setRowCount(0);
    
    auto medicamentos = generador->getMedicamentos();
    tablaMedicamentos->setRowCount(medicamentos.size());
    
    for (size_t i = 0; i < medicamentos.size(); ++i) {
        const auto& med = medicamentos[i];
        tablaMedicamentos->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(med.getNombre())));
        tablaMedicamentos->setItem(i, 1, new QTableWidgetItem(QString::number(med.getFrecuencia())));
        tablaMedicamentos->setItem(i, 2, new QTableWidgetItem(QString::number(med.getDuracion())));
    }
}

void MainWindow::actualizarTablaIncompatibilidades() {
    tablaIncompatibilidades->setRowCount(0);
    
    auto incompatibilidades = generador->getIncompatibilidades();
    tablaIncompatibilidades->setRowCount(incompatibilidades.size());
    
    for (size_t i = 0; i < incompatibilidades.size(); ++i) {
        const auto& inc = incompatibilidades[i];
        tablaIncompatibilidades->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(inc.getMedicamento1())));
        tablaIncompatibilidades->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(inc.getMedicamento2())));
    }
}

void MainWindow::actualizarTablaHorario(const std::vector<Dosis>& horario) {
    tablaHorario->setRowCount(0);
    tablaHorario->setRowCount(horario.size());
    
    for (size_t i = 0; i < horario.size(); ++i) {
        const auto& dosis = horario[i];
        tablaHorario->setItem(i, 0, new QTableWidgetItem(QString::number(dosis.dia + 1)));
        tablaHorario->setItem(i, 1, new QTableWidgetItem(QString::number(dosis.hora) + ":00"));
        tablaHorario->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(dosis.medicamento)));
    }
}

void MainWindow::limpiarTablas() {
    tablaMedicamentos->setRowCount(0);
    tablaIncompatibilidades->setRowCount(0);
    tablaHorario->setRowCount(0);
}

