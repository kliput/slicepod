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

#ifndef POSITIONWIDGET_HPP
#define POSITIONWIDGET_HPP

#include <QWidget>

#include "core/libraryinfo.hpp"
#include "db_engine/fragment.hpp"

class MusicPlayer;
class ImagesManager;

class PositionWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PositionWidget(QWidget *parent = 0);

	void setCurrentFragment(Fragment::ptr fragment);
	void setMusicPlayer(MusicPlayer* musicPlayer);

	void drawBar(QPainter &painter);
	void drawBarSelection(QPainter &painter, const int positionX1, const int positionX2, const QColor &color);

protected:
	void paintEvent(QPaintEvent *) override;

	inline void putBottomArrow(const QImage& image, const int positionX,
							   QPainter& painter, bool on = false);

	inline int translateArrowX(const int& positionSec);

	void setMediaLength(int value) { mediaLength = value; }


	static constexpr int arrowWidth = 16;
	static constexpr int arrowHeight = 16;
	static constexpr int barHeight = 8;
	static constexpr int spaceOff = 8;

	static constexpr int bottomArrowY();
	static constexpr int bottomArrowYOff();
	static constexpr int totalHeight();

	int barOffset();
	int barY1();
	int barWidth();
	int barY2();

private:
	int mediaLength = 99;
	int playerPosition = 0;

	Fragment::ptr currentFragment;
	MusicPlayer* musicPlayer = nullptr;
	ImagesManager* images = nullptr;

	// TODO: static, but it must be initialized after loading system palette
	QColor barColor;
	QColor barFragmentSelectionColor;
	QColor barCurrentSelectionColor;

signals:

public slots:
	void setPlayerPosition(int position);
};

#endif // POSITIONWIDGET_HPP
