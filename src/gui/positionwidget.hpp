#ifndef POSITIONWIDGET_HPP
#define POSITIONWIDGET_HPP

#include <QWidget>

#include "core/libraryitem.hpp"

class PositionWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PositionWidget(QWidget *parent = 0);

	void setCurrentItem(LibraryItem* item);

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

private:
	int mediaLength = 99;
	int playerPosition = 0;

	LibraryItem* currentItem = nullptr;

signals:

public slots:
	void setPlayerPosition(int position);
};

#endif // POSITIONWIDGET_HPP
