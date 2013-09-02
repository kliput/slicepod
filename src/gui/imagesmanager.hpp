#ifndef IMAGESMANAGER_HPP
#define IMAGESMANAGER_HPP

#include <QImage>

class ImagesManager
{
public:
	inline static ImagesManager* getInstance() {
		static ImagesManager instance;
		return &instance;
	}

	const QImage& getPlayArrow() { return playArrow; }
	const QImage& getStartArrow() { return startArrow; }
	const QImage& getEndArrow() { return endArrow; }
	const QImage& getMarkerArrow() { return markerArrow; }

private:
	// THIS CLASS IS SINGLETON
	ImagesManager();
	~ImagesManager();
	ImagesManager(const ImagesManager&);
	ImagesManager& operator=(const ImagesManager&);

	QImage playArrow;
	QImage startArrow;
	QImage endArrow;
	QImage markerArrow;
};

#endif // IMAGESMANAGER_HPP
