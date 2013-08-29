#include "positionwidget.hpp"

#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <db_engine/fragment.hpp>

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

void PositionWidget::setCurrentItem(LibraryItem* item)
{
	if (item) {
		// TODO FIXME!!!
		Episode::ptr e;
		if (currentItem) e = currentItem->getEpisode();
		auto b = item->getEpisode();
		if (!currentItem || currentItem->getEpisode()->id() != item->getEpisode()->id()) {
			setMediaLength(item->episodeLengthSec());
		}
		currentItem = item;
		update();
	}
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
	static const QImage markerArrowImage(":/images/arrow-up-yellow.png");

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(Qt::NoPen);
	painter.setBrush(barColor);
	painter.drawRect(arrowWidth/2, arrowWidth,
					 width()-arrowWidth, barHeight);

	if (currentItem) {

		// draw position on top of bar
		painter.drawImage(translateArrowX(playerPosition), 0, playArrowImage);

		// TODO: cache fragments list
		auto fragments = currentItem->episodeFragmentsList();
		auto currentFragment = currentItem->getFragment();

		for (Fragment::ptr f: fragments) {
			if (f->id() != currentFragment->id()) {
				putBottomArrow(markerArrowImage, f->getStart(), painter);
			}
		}

		// TODO: only if there's  playing fragment...
		putBottomArrow(startArrowImage, currentFragment->getStart(), painter, true);
		if (currentFragment->hasEnd()) {
			putBottomArrow(endArrowImage, currentFragment->getEnd(), painter, true);
		}

		// TODO: colorify and draw end for hovered marker
	}
}

void PositionWidget::setPlayerPosition(int position)
{
	playerPosition = position;
	update();
}
