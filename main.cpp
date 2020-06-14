#include "mainwindow.h"

#include <QApplication>

///
/// \brief Это функция мэйн, в которой выполняется приложение
/// \param argc Количество аргументов
/// \param argv Массив аргументов (вектор)
/// \return Возвращает результат выполнения программы
///
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
