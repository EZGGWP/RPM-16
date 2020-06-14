#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QInputDialog>
#include <QLabel>
#include <QFormLayout>
#include <QValidator>
#include <QDialogButtonBox>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QScreen>
#include <QSize>
#include <QColorDialog>
#include <QCloseEvent>

///
/// \brief Это конструктор главного окна
/// \param parent Ссылка на виджет-родитель
///
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    ui->graphicsView->hide();
    ui->graphicsView->setScene(obj);
    this->setMinimumSize(400, 300);
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);

    setWindowTitle("Paint.da");
    QMenu *fileMenu = ui->menubar->addMenu("File...");
    fileMenu->addAction("Create...", this, &MainWindow::newAction, Qt::CTRL + Qt::Key_C);
    fileMenu->addAction("Open...", this, &MainWindow::openAction, Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("Save...", this, &MainWindow::saveAction, Qt::CTRL + Qt::Key_S);
    QAction *color = new QAction("Pick Color...", this);
    ui->menubar->addAction(color);
    connect(color, &QAction::triggered, this, &MainWindow::pickColor);
    connect(this, &MainWindow::saveSignal, this, &MainWindow::saveAction);
}

///
/// \brief Это деструктор главного окна
///
MainWindow::~MainWindow() {
    delete ui;
}

///
/// \brief Это слот, который изменяет размер холста под размер главного окна при масштабировании
///
void MainWindow::slotTimer() {
    timer->stop();
    obj->setSceneRect(0, 0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
};

///
/// \brief Это эвент изменения размеров главного окна
/// \param event Принимает на вход ссылку на эвент изменения размеров окна
///
void MainWindow::resizeEvent(QResizeEvent *event) {
    timer->start(100);
    QMainWindow::resizeEvent(event);
};

///
/// \brief Это слот действия создания нового рисунка/холста
///
void MainWindow::newAction() {
    if (!firstCreation) {
        emit saveSignal();
    }
    firstCreation = false;
    QDialog newDocInput(this);
    newDocInput.setFixedSize(300, 300);
    QFormLayout layout(&newDocInput);
    layout.addRow(new QLabel("Enter wanted canvas size:"));
    QLabel *lbl1 = new QLabel("Width:");
    QLabel *lbl2 = new QLabel("Height:");
    QLineEdit *wdth = new QLineEdit();
    QLineEdit *hght = new QLineEdit();
    wdth->setValidator(new QIntValidator(0, 800, this));
    hght->setValidator(new QIntValidator(0, 800, this));

    QDialogButtonBox btnBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &newDocInput);
    layout.addRow(lbl1, wdth);
    layout.addRow(lbl2, hght);
    layout.addRow(&btnBox);
    QObject::connect(&btnBox, SIGNAL(accepted()), &newDocInput, SLOT(accept()));
    QObject::connect(&btnBox, SIGNAL(rejected()), &newDocInput, SLOT(reject()));
    newDocInput.setFixedSize(200, 200);
    if (newDocInput.exec() == QDialog::Accepted) {
        int newDocWidth = wdth->text().toInt();
        int newDocHeight = hght->text().toInt();
        qDebug() << newDocWidth;
        qDebug() << newDocHeight;
        this->resize(newDocWidth + 100, newDocHeight + 100);
        ui->graphicsView->setFixedSize(newDocWidth, newDocHeight);

    }

    obj->clear();
    ui->graphicsView->show();

};

///
/// \brief Это слот действия открытия существующей картинки
///
void MainWindow::openAction() {
    if (!firstCreation) {
        emit saveSignal();
    }
    QUrl imagePath = QFileDialog::getOpenFileUrl(this, "Choose image you want to edit", QUrl(""), "Images (*.png *.xpm *.jpg)");
    QPixmap editFile(imagePath.toLocalFile());
    ui->graphicsView->show();
    firstCreation = false;
    QSize scrSize = qApp->screens()[0]->size();
    int deltaW = editFile.width() - scrSize.width();
    int deltaH = editFile.height() - scrSize.height();
    if (deltaW > 0 || deltaH > 0) {
        if (deltaW > deltaH) {
            editFile = editFile.scaledToWidth(scrSize.width() - 200);
        } else if (deltaH > deltaW) {
            editFile = editFile.scaledToHeight(scrSize.height() - 200);
        }
    }
    qDebug() << editFile.width();
    qDebug() << editFile.height();
    ui->graphicsView->setFixedSize(editFile.width(), editFile.height());
    this->resize(editFile.width() + 100, editFile.height() + 100);
    QGraphicsPixmapItem *pic = new QGraphicsPixmapItem(editFile);
    pic->setPos(-10, -10);
    obj->addItem(pic);
};

///
/// \brief Это слот действия сохранения созданного рисунка
///
void MainWindow::saveAction() {
    QDialog confirmNewDoc(this);
    confirmNewDoc.setFixedSize(300, 100);
    QFormLayout confLyt(&confirmNewDoc);
    QDialogButtonBox cnfBtnBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, Qt::Horizontal, &confirmNewDoc);
    confLyt.addRow(new QLabel("Do you want to save your artwork?"));
    confLyt.addRow(&cnfBtnBox);
    QObject::connect(&cnfBtnBox, SIGNAL(accepted()), &confirmNewDoc, SLOT(accept()));
    QObject::connect(&cnfBtnBox, SIGNAL(rejected()), &confirmNewDoc, SLOT(reject()));
    if (confirmNewDoc.exec() == QDialog::Accepted) {
        if (!ui->graphicsView->isHidden()) {
            QPixmap img = ui->graphicsView->grab(ui->graphicsView->sceneRect().toRect());
            QFileDialog save(this);
            save.setFixedSize(600, 400);
            save.setAcceptMode(QFileDialog::AcceptSave);
            save.setFileMode(QFileDialog::AnyFile);
            QUrl filepath = save.getSaveFileUrl();
            qDebug() << filepath.toLocalFile();
            QFile imgFile(filepath.toLocalFile());
            imgFile.open(QIODevice::WriteOnly);
            img.save(&imgFile, "PNG");
            imgFile.close();
        } else {
            QMessageBox box;
            box.setText("Nothing to save.");
            box.setWindowTitle("Whoopsie...");
            box.setDefaultButton(QMessageBox::Ok);
            box.exec();
        }
    }
};

///
/// \brief Это слот выбора цвета кисти
///
void MainWindow::pickColor() {
    QColor clr = QColorDialog::getColor();
    qDebug() << clr;
    obj->brushColor = clr;
}

///
/// \brief Это перегрузка эвента закрытия, которая проверяет, есть ли материал для сохранения
/// \param event Принимает ссылку на эвент закрытия окна
///
void MainWindow::closeEvent(QCloseEvent *event) {
	Q_UNUSED(event);
    if (!firstCreation) {
        emit saveAction();
    };
}

