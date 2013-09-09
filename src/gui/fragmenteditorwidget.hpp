#ifndef FRAGMENTEDITORWIDGET_HPP
#define FRAGMENTEDITORWIDGET_HPP

#include <QWidget>

#include "db_engine/fragment.hpp"

namespace Ui {
class FragmentEditorWidget;
}

class FragmentEditorWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit FragmentEditorWidget(QWidget *parent = 0);
	~FragmentEditorWidget();
	
	void setFragment(const Fragment::ptr fragment);
	void fillEditorView();

	void updateRemoveButton();
	void updateSaveButton();

private:
	Ui::FragmentEditorWidget *ui;
	Fragment::ptr fragment;
};

#endif // FRAGMENTEDITORWIDGET_HPP
