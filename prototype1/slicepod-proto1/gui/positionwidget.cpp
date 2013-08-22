#include "positionwidget.hpp"

#include <QtCore>
#include <QtGui>
#include <QDebug>

PositionWidget::PositionWidget(QWidget *parent) :
	QWidget(parent)
{
	setPlayerPosition(0);
	setMinimumHeight(totalHeight());
}

constexpr int PositionWidget::bottomArrowY()
{
	return arrowHeight+barHeight;
}

constexpr int PositionWidget::bottomArrowYOff()
{
	return bottomArrowY()+spaceOff;
}

constexpr int PositionWidget::totalHeight()
{
	return bottomArrowYOff()+arrowHeight;
}

int PositionWidget::translateArrowX(const int& positionSec)
{
	return (float(positionSec)/mediaLength)*(width()-arrowWidth);
}


inline void PositionWidget::putBottomArrow(const QImage& image,
										   const int positionX, QPainter& painter,
										   bool on)
{
	painter.drawImage(translateArrowX(positionX), on?
						  bottomArrowY() : bottomArrowYOff(), image);
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

	putBottomArrow(startArrowImage, 0, painter, true);
	putBottomArrow(endArrowImage, playerPosition+100, painter, false);
}

void PositionWidget::setPlayerPosition(int position)
{
	playerPosition = position;
	update();
}
