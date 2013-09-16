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
	void updateUndoButton();

	void updateButtons();
	void updateEndAvailability();

	void setTimeReadOnly(bool readOnly);

private slots:
	void changeStart(const QTime &time);
	void changeEnd(const QTime &time);
	void changeEndAvailability(bool hasEnd);
	void handleUndoButton();
	void handleSaveButton();
	void handleRemoveButton();

	void handleCurrentFragmentRemoved();

private:
	Ui::FragmentEditorWidget *ui;
	Fragment::ptr fragment;

signals:
	void fragmentRemoved(Fragment::ptr);
	void fragmentDataChanged(Fragment::ptr);
};

#endif // FRAGMENTEDITORWIDGET_HPP
