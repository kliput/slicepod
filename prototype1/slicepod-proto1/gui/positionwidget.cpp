#include "positionwidget.hpp"

#include <QtCore>
#include <QtGui>
#include <QDebug>

const int PositionWidget::arrowWidth = 16;
const int PositionWidget::arrowHeight = 16;
const int PositionWidget::barHeight = 8;

PositionWidget::PositionWidget(QWidget *parent) :
	QWidget(parent)
{
	setPlayerPosition(0);
	setMinimumHeight(2*arrowHeight + barHeight);
}

inline int PositionWidget::bottomArrowY()
{
	static const int pos = arrowHeight+barHeight;
	return pos;
}

inline int PositionWidget::translateArrowX(const int& positionSec)
{
	return (float(positionSec)/mediaLength)*(width()-arrowWidth);
}

void PositionWidget::paintEvent(QPaintEvent *)
{
	static const QColor barColor(255, 255, 255, 160);
	static const QImage playArrowImage(":/images/arrow-down-double-blue.png");
	static const QImage startArrowImage(":/images/arrow-up-green.png");
	static const QImage endArrowImage(":/images/arrow-up-red.png");
	static const QImage fragmentArrowImage(":/images/arrow-up-yellow.png");

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(Qt::NoPen);
	painter.setBrush(barColor);
	painter.drawRect(arrowWidth/2, arrowWidth,
					 width()-arrowWidth, barHeight);

	painter.drawImage(translateArrowX(playerPosition), 0, playArrowImage);
	painter.drawImage(translateArrowX(0), bottomArrowY(), startArrowImage);
	painter.drawImage(translateArrowX(playerPosition), bottomArrowY(), endArrowImage);

}

void PositionWidget::setPlayerPosition(int position)
{
	playerPosition = position;
	update();
}
