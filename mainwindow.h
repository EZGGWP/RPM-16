#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "graphics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///
/// \brief Это класс главного окна приложения
///
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
	///
	/// \brief Это конструктор главного окна
	/// \param parent Ссылка на виджет-родитель
	///
    MainWindow(QWidget *parent = nullptr);

	///
	/// \brief Это деструктор главного окна
	///
    ~MainWindow();
	///
	/// \brief Это графическая сцена программы
	///
    QGraphicsScene *newScene;

private:
	///
	/// \brief Это объект, содержаший элементы интерфейса
	///
    Ui::MainWindow *ui;

	///
	/// \brief Это объект Graphics, отвечающий за графику
	///
    Graphics *obj = new Graphics(this);

	///
	/// \brief Это таймер, используемый для ресайзинга холста с окном
	///
    QTimer *timer;

	///
	/// \brief Это эвент изменения размеров главного окна
	/// \param event Принимает на вход ссылку на эвент изменения размеров окна
	///
    void resizeEvent(QResizeEvent *event);

	///
	/// \brief Это поле отвечает за необходимость сохранения
	///
    bool firstCreation = true;

	///
	/// \brief Это перегрузка эвента закрытия, которая проверяет, есть ли материал для сохранения
	/// \param event Принимает ссылку на эвент закрытия окна
	///
    void closeEvent(QCloseEvent *event);

public slots:
	///
	/// \brief Это слот, который изменяет размер холста под размер главного окна при масштабировании
	///
    void slotTimer();

	///
	/// \brief Это слот действия создания нового рисунка/холста
	///
    void newAction();

	///
	/// \brief Это слот действия сохранения созданного рисунка
	///
    void saveAction();

	///
	/// \brief Это слот действия открытия существующей картинки
	///
    void openAction();

	///
	/// \brief Это слот выбора цвета кисти
	///
    void pickColor();

signals:
	///
	/// \brief Это сигнал для сохранения картинки
	///
    void saveSignal();
};
#endif // MAINWINDOW_H
