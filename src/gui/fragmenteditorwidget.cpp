#include "fragmenteditorwidget.hpp"
#include "ui_fragmenteditorwidget.h"

#include "core/libraryinfo.hpp"

FragmentEditorWidget::FragmentEditorWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FragmentEditorWidget)
{
	ui->setupUi(this);

	// connections on value changes
	connect(ui->startTimeEdit, SIGNAL(timeChanged(QTime)),
			this, SLOT(changeStart(QTime)));

	connect(ui->endTimeEdit, SIGNAL(timeChanged(QTime)),
			this, SLOT(changeEnd(QTime)));

	connect(ui->endTimeCheckBox, SIGNAL(toggled(bool)),
			this, SLOT(changeEndAvailability(bool)));

	connect(ui->undoButton, SIGNAL(clicked()),
			this, SLOT(handleUndoButton()));

	connect(ui->saveButton, SIGNAL(clicked()),
			this, SLOT(handleSaveButton()));

	connect(ui->removeButton, SIGNAL(clicked()),
			this, SLOT(handleRemoveButton()));

	// TODO: update library view values?

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

		updateButtons();

		ui->podcastLabel->setText(fragment->getEpisode()->getPodcast()->getName());
		ui->episodeLabel->setText(fragment->getEpisode()->getEpisodeName());

		ui->startTimeEdit->blockSignals(true); // prevent from invoking change listeners
		ui->startTimeEdit->setMinimumTime(QTime());
		ui->startTimeEdit->setMaximumTime(fragment->getLibraryInfo()->getEpisodeLengthTime());
		ui->startTimeEdit->setTime(fragment->getLibraryInfo()->getFragmentStartTime());

		ui->startTimeEdit->blockSignals(false);

		ui->endTimeEdit->blockSignals(true);
		ui->endTimeCheckBox->blockSignals(true);

		updateEndAvailability();

		ui->endTimeEdit->blockSignals(false);
		ui->endTimeCheckBox->blockSignals(false);

		ui->tagsListLabel->setText(fragment->getLibraryInfo()->getTagsString());

		// lock time edit in start fragments
		setTimeReadOnly(fragment->isStartFragment());
	}
}

inline void FragmentEditorWidget::updateSaveButton()
{
	ui->saveButton->setEnabled(fragment->isStored() && fragment->isDirty());
}

inline void FragmentEditorWidget::updateRemoveButton()
{
	ui->removeButton->setEnabled(fragment->canRemove());
}

inline void FragmentEditorWidget::updateUndoButton()
{
	ui->undoButton->setEnabled(fragment->isStored() && fragment->isDirty());
}

void FragmentEditorWidget::updateButtons()
{
	updateSaveButton();
	updateRemoveButton();
	updateUndoButton();
}

// editor value changes handlers -->

void FragmentEditorWidget::changeStart(const QTime& time)
{
	fragment->setStart(QTime().msecsTo(time));
	updateButtons();
}

void FragmentEditorWidget::changeEnd(const QTime& time)
{
	fragment->setEnd(QTime().msecsTo(time));
	updateButtons();
}

/**
 * @brief FragmentEditorWidget::changeEndAvailability is executed, when user
 * changes state of checkbox of fragment end time. If fragment has end, user
 * changes state to false and then back to true, end time will be RESETTED to
 * default (now it's end of episode).
 * @param hasEnd new state of corresponding checkbox - true if end time should
 * be initialized to default; false if end time should be disabled.
 */
void FragmentEditorWidget::changeEndAvailability(bool hasEnd)
{
	if (hasEnd) {
		// TODO: default end position
		fragment->setEnd(fragment->getEpisode()->getAudioLengthSec()*1000);
	} else {
		fragment->setEnd(-1); // TODO: null constant expression for this
	}

	updateEndAvailability();
}

// <--

void FragmentEditorWidget::updateEndAvailability()
{
	if (fragment->hasEnd()) {
		ui->endTimeEdit->setMinimumTime(fragment->getLibraryInfo()->getFragmentStartTime());
		ui->endTimeEdit->setMaximumTime(fragment->getLibraryInfo()->getEpisodeLengthTime());
		ui->endTimeEdit->setTime(fragment->getLibraryInfo()->getFragmentEndTime());

		ui->endTimeCheckBox->setChecked(true);
		ui->endTimeEdit->setEnabled(true);
	} else {
		ui->endTimeEdit->setTime(QTime()); // reset to 00:00:00
		ui->endTimeCheckBox->setChecked(false);
		ui->endTimeEdit->setEnabled(false);
	}
}

void FragmentEditorWidget::setTimeReadOnly(bool readOnly)
{
	ui->startTimeEdit->setReadOnly(readOnly);
	ui->endTimeCheckBox->setDisabled(readOnly);
	ui->endTimeEdit->setReadOnly(readOnly);
}

// handle buttons -->

void FragmentEditorWidget::handleUndoButton()
{
	// TODO: question if user really wants to revert changes
	fragment->revert();
	fillEditorView();
}

void FragmentEditorWidget::handleSaveButton()
{
	fragment->save();
	fillEditorView();
}

void FragmentEditorWidget::handleRemoveButton()
{
	if (fragment->canRemove()) {
		if (fragment->remove()) {
			auto fragmentCopy = fragment;
			emit fragmentRemoved(fragmentCopy);
			handleCurrentFragmentRemoved();
		} else {
			qCritical("Current fragment from fragment editor cannot be removed,"
					  " because of database engine error.");
		}
	} else {
		// TODO: nasty error!
		qCritical("Current fragment cannot be removed.");
	}
}

void FragmentEditorWidget::handleCurrentFragmentRemoved()
{
	setFragment(Fragment::ptr()); // set null
}

// <--
