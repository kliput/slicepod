#ifndef POSITIONWIDGET_HPP
#define POSITIONWIDGET_HPP

#include <QWidget>

class PositionWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PositionWidget(QWidget *parent = 0);

	void setMediaLength(int value) { mediaLength = value; }

protected:
	void paintEvent(QPaintEvent *) override;

	inline int translateArrowX(const int& positionSec);
	static inline int bottomArrowY();

private:
	int mediaLength = 99;
	int playerPosition = 0;

	static const int arrowWidth;
	static const int arrowHeight;
	static const int barHeight;

signals:

public slots:
	void setPlayerPosition(int position);
};

#endif // POSITIONWIDGET_HPP
