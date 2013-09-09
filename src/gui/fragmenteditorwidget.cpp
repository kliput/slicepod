#include "fragmenteditorwidget.hpp"
#include "ui_fragmenteditorwidget.h"

#include "core/libraryinfo.hpp"

FragmentEditorWidget::FragmentEditorWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FragmentEditorWidget)
{
	ui->setupUi(this);
}

FragmentEditorWidget::~FragmentEditorWidget()
{
	delete ui;
}

void FragmentEditorWidget::setFragment(const Fragment::ptr fragment)
{
	// TODO: warning, when current fragment is modified and not saved
	this->fragment = fragment;
	fillEditorView();
}

void FragmentEditorWidget::fillEditorView()
{
	if (!fragment) {
		setEnabled(false);
	} else {
		setEnabled(true);

		updateSaveButton();
		updateRemoveButton();

		ui->editorPodcastLabel->setText(fragment->getEpisode()->getPodcast()->getName());
		ui->editorEpisodeLabel->setText(fragment->getEpisode()->getEpisodeName());

		ui->editorStartTimeEdit->setTimeRange(QTime(), fragment->getLibraryInfo()->getFragmentEndTime());
		ui->editorStartTimeEdit->setTime(fragment->getLibraryInfo()->getFragmentStartTime());

		if (fragment->hasEnd()) {
			ui->editorEndTimeEdit->setTimeRange(QTime(), fragment->getLibraryInfo()->getEpisodeLengthTime());
			ui->editorEndTimeEdit->setTime(fragment->getLibraryInfo()->getFragmentEndTime());
			ui->editorEndTimeCheckBox->setChecked(true);
			ui->editorEndTimeEdit->setEnabled(true);
		} else {
			ui->editorEndTimeCheckBox->setChecked(false);
			ui->editorEndTimeEdit->setEnabled(false);
		}

		ui->editorTagsListLabel->setText(fragment->getLibraryInfo()->getTagsString());
	}
}

//void FragmentEditorWidget::setEnabled(bool state)
//{
//	static QWidget* fragmentPanelElements[] = {
//		ui->editorEndTimeCheckBox,
//		ui->editorEndTimeEdit,
//		ui->editorStartTimeEdit,
//		ui->editorStartTimeLabel,
//		ui->editorEpisodeLabel,
//		ui->editorEpisodeStaticLabel,
//		ui->editorBox,
//		ui->editorPodcastLabel,
//		ui->editorPodcastStaticLabel,
//		ui->editorTagsStaticLabel,
//		ui->editorTagsListLabel,
//		ui->editorAddTagButton,
//		ui->editorAddTagCombo,
//		// save button should be updated separately after this method with updateEditorSaveButton()
//		ui->editorSaveButton,
//		// remove button should be updated separately after this method with updateEditorRemoveButton()
//		ui->editorRemoveButton
//	};

//	for (auto w: fragmentPanelElements) {
//		w->setEnabled(state);
//	}
//}

void FragmentEditorWidget::updateSaveButton()
{
	ui->editorSaveButton->setEnabled(fragment->isStored() && fragment->isDirty());
}

void FragmentEditorWidget::updateRemoveButton()
{
	ui->editorSaveButton->setEnabled(fragment->canRemove());
}
