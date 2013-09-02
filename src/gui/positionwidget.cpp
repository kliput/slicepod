/****************************************************************************
* Slicepod
* Copyright (C) 2013 Jakub Liput <jakub.liput@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/ 

#include "positionwidget.hpp"

#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <vlc-qt/MediaPlayer.h>

#include "db_engine/fragment.hpp"
#include "db_engine/episode.hpp"
#include "core/musicplayer.hpp"
#include "gui/imagesmanager.hpp"

PositionWidget::PositionWidget(QWidget *parent) :
	QWidget(parent),
	images(ImagesManager::getInstance())
{
	setPlayerPosition(0);
	setMinimumHeight(totalHeight());

	barColor = QApplication::palette().color(QPalette::Base);
	barColor.setAlpha(160);

	barCurrentSelectionColor = QApplication::palette().color(QPalette::Highlight);

	barFragmentSelectionColor = QColor(QApplication::palette().color(QPalette::Highlight));
	barFragmentSelectionColor.setAlpha(80);
}

//inline int PositionWidget::barMarginX() { return arrowWidth/2; }

inline int PositionWidget::barOffset() { return arrowWidth/2; }
inline int PositionWidget::barY1() { return arrowWidth; }
inline int PositionWidget::barWidth() { return width()-arrowWidth; }

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

int PositionWidget::translateArrowX(const int& positionMs)
{
	return (float(positionMs)/mediaLength)*(width()-arrowWidth);
}

void PositionWidget::setMusicPlayer(MusicPlayer *musicPlayer)
{
	if (this->musicPlayer) {
		disconnect(this->musicPlayer->getVlcPlayer(), SIGNAL(timeChanged(int)),
				   this, SLOT(setPlayerPosition(int)));
	}
	this->musicPlayer = musicPlayer;
	connect(this->musicPlayer->getVlcPlayer(), SIGNAL(timeChanged(int)),
			this, SLOT(setPlayerPosition(int)));
}

void PositionWidget::setCurrentFragment(Fragment::ptr fragment)
{
	if (fragment) {
		if (!currentFragment || currentFragment->getEpisode()->id() != fragment->getEpisode()->id()) {
			setMediaLength(fragment->getEpisode()->audioLength());
		}
		currentFragment = fragment;
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

void PositionWidget::drawBar(QPainter &painter)
{
	painter.setPen(Qt::NoPen);
	painter.setBrush(barColor);
	painter.drawRect(barOffset(), barY1(),
					 barWidth(), barHeight);
}

void PositionWidget::drawBarSelection(QPainter& painter, const int positionX1,
									  const int positionX2, const QColor& color)
{
	painter.setPen(Qt::NoPen);
	painter.setBrush(color);
//	painter.drawRect(translateArrowX(positionX1), barY1(),
//					 translateArrowX(positionX2), barY2());

	int posX = barOffset() + translateArrowX(positionX1);
	int width = translateArrowX(positionX2)-translateArrowX(positionX1);

	painter.drawRect(posX, barY1(), width, barHeight);
}

void PositionWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	drawBar(painter);

	if (currentFragment) {
		// draw position on top of bar
		painter.drawImage(translateArrowX(playerPosition), 0, images->getPlayArrow());

		// TODO: cache fragments list
		QList<Fragment::ptr> fragments = currentFragment->getEpisode()->getFragmentsList();

		for (Fragment::ptr f: fragments) {
			if (f->id() != currentFragment->id()) {
				putBottomArrow(images->getMarkerArrow(), f->getStart(), painter);

				if (f->hasEnd()) {
					drawBarSelection(painter, f->getStart(),
									 f->getEnd(), barFragmentSelectionColor);
				} else {
					// TODO: draw only line above marker
				}
			}
		}

		// TODO: only if there's  playing fragment...
		int selectionEnd;;
		putBottomArrow(images->getStartArrow(), currentFragment->getStart(), painter, true);
		if (currentFragment->hasEnd()) {
			putBottomArrow(images->getEndArrow(), currentFragment->getEnd(), painter, true);
			selectionEnd = currentFragment->getEnd();
		} else {
			selectionEnd = currentFragment->getEpisode()->audioLength();
		}

		drawBarSelection(painter, currentFragment->getStart(),
						 selectionEnd, barCurrentSelectionColor);

		// TODO: colorify and draw end for hovered marker
	}
}

void PositionWidget::setPlayerPosition(int position)
{
	playerPosition = position;
	update();
}
