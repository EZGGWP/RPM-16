#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>

///
/// \brief Это класс Graphics, отвечающий за графику рисования
///
class Graphics : public QGraphicsScene {
        Q_OBJECT
public:
	///
	/// \brief Это конструктор класса Graphics, отвечающего за рисование
	/// \param parent Принимает ссылку на объект-родитель
	///
    Graphics(QObject *parent = 0);

	///
	/// \brief Это эвент нажатия кнопки мыши
	/// \param event Принимает ссылку на эвент действия мыши в графической сцене
	///
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

	///
	/// \brief Это эвент перемещения кнопки мыши
	/// \param event Принимает ссылку на эвент действия мыши в графической сцене
	///
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	///
	/// \brief Это поле содержит цвет кисти
	///
    QColor brushColor;

private:
	///
	/// \brief Это поле хранит предыдущее положение мыши/кисти
	///
    QPointF prevPos;

};

#endif // GRAPHICS_H
