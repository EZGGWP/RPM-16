#include "graphics.h"
#include <QDebug>

///
/// \brief Это конструктор класса Graphics, отвечающего за рисование
/// \param parent Принимает ссылку на объект-родитель
///
Graphics::Graphics(QObject *parent) : QGraphicsScene(parent) {
    brushColor = Qt::blue;
}

///
/// \brief Это эвент нажатия кнопки мыши
/// \param event Принимает ссылку на эвент действия мыши в графической сцене
///
void Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    addEllipse(event->scenePos().x() - 5, event->scenePos().y() - 5, 10, 10, QPen(Qt::NoPen), QBrush(brushColor));
    prevPos = event->scenePos();

}

///
/// \brief Это эвент перемещения кнопки мыши
/// \param event Принимает ссылку на эвент действия мыши в графической сцене
///
void Graphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    addLine(prevPos.x(), prevPos.y(), event->scenePos().x(), event->scenePos().y(), QPen(brushColor, 10, Qt::SolidLine, Qt::RoundCap));
    prevPos = event->scenePos();

}
