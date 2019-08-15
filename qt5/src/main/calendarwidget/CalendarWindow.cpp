//
// Created by xxzyjy on 18/12/2017.
//

#include <QGroupBox>
#include <QCalendarWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QDateEdit>
#include "CalendarWindow.h"

CalendarWindow::CalendarWindow() {
  createPreviewGroupBox();
  createGeneralOptionsGroupBox();
  createDatesGroupBox();
  createTextFormatsGroupBox();


  auto layout = new QGridLayout;
  layout->addWidget(previewGroupBox, 0, 0);
  layout->addWidget(generalOptionsGroupBox, 0, 1);
  layout->addWidget(datesGroupBox, 1, 0);
  layout->addWidget(textFormatsGroupBox, 1, 1);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height());
  previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width());

  setWindowTitle(tr("Calendar Widget"));
}

void CalendarWindow::createPreviewGroupBox() {
  previewGroupBox = new QGroupBox(tr("Preview"));
  calendar = new QCalendarWidget;
  calendar->setMinimumDate(QDate(1900, 1, 1));
  calendar->setMaximumDate(QDate(3000, 1, 1));
  calendar->setGridVisible(true);

//  connect(calendar, SIGNAL(currentPageChanged(int, int)), this, SLOT(reformatCalendarPage()));

  previewLayout = new QGridLayout;
  previewLayout->addWidget(calendar, 0, 0, Qt::AlignCenter);
  previewGroupBox->setLayout(previewLayout);
}

void CalendarWindow::createGeneralOptionsGroupBox() {
  generalOptionsGroupBox = new QGroupBox(tr("General Options"));
  localeCombo = new QComboBox;

  int curLocaleIndex = -1;
  int index = 0;
  for (int _lang = QLocale::C; _lang <= QLocale::LastLanguage; ++_lang) {
    auto lang = static_cast<QLocale::Language>(_lang);
    auto countries = QLocale::countriesForLanguage(lang);
    for (auto country : countries) {
      auto label = QLocale::languageToString(lang);
      label += QLatin1Char('/');
      label += QLocale::countryToString(country);
      QLocale locale(lang, country);

      if (this->locale().language() == lang && this->locale().country() == country)
        curLocaleIndex = index;

      localeCombo->addItem(label, locale);
      ++index;

    }
  }

  if (curLocaleIndex != -1)
    localeCombo->setCurrentIndex(curLocaleIndex);
  localeLabel = new QLabel(tr("&Locale"));
  localeLabel->setBuddy(localeCombo);

  firstDayCombo = new QComboBox;
  firstDayCombo->addItem(tr("Sunday"), Qt::Sunday);
  firstDayCombo->addItem(tr("Monday"), Qt::Monday);

  firstDayLabel = new QLabel(tr("Wee&k starts on:"));
  firstDayLabel->setBuddy(firstDayCombo);

  selectionModeCombo = new QComboBox;
  selectionModeCombo->addItem(tr("Single selection"),
                              QCalendarWidget::SingleSelection);
  selectionModeCombo->addItem(tr("None"),
                              QCalendarWidget::NoSelection);

  selectionModeLabel = new QLabel(tr("&Selection mode:"));
  selectionModeLabel->setBuddy(selectionModeCombo);

  gridCheckBox = new QCheckBox(tr("&Grid"));
  gridCheckBox->setChecked(calendar->isGridVisible());

  navigationCheckBox = new QCheckBox(tr("&Navigation bar"));
  navigationCheckBox->setChecked(true);

  horizontalHeaderCombo = new QComboBox;
  horizontalHeaderCombo->addItem(tr("Single letter day names"),
                                 QCalendarWidget::SingleLetterDayNames);
  horizontalHeaderCombo->addItem(tr("Short day names"),
                                 QCalendarWidget::ShortDayNames);
  horizontalHeaderCombo->addItem(tr("None"),
                                 QCalendarWidget::ShortDayNames);
  horizontalHeaderCombo->setCurrentIndex(1);

  horizontalHeaderLabel = new QLabel(tr("&Horizontal header:"));
  horizontalHeaderLabel->setBuddy(horizontalHeaderCombo);

  verticalHeaderCombo = new QComboBox;
  verticalHeaderCombo->addItem(tr("ISO week numbers"),
                               QCalendarWidget::ISOWeekNumbers);
  verticalHeaderCombo->addItem(tr("None"), QCalendarWidget::NoVerticalHeader);
  auto verticalHeaderLabel = new QLabel(tr("&Verticl header:"));
  verticalHeaderLabel->setBuddy(verticalHeaderCombo);

//  connect(localeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(localeChanged(int)));
//  connect(firstDayCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(firstDayChanged(int)));
//  connect(selectionModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionModeChanged(int)));
//  connect(gridCheckBox, SIGNAL(toggled(bool)), calendar, SLOT(setGridVisible(bool)));
//  connect(navigationCheckBox, SIGNAL(toggled(bool)), calendar, SLOT(setNavigationBarVisible(bool)));
//  connect(horizontalHeaderCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(horizontalHeaderChanged(int)));
//  connect(verticalHeaderCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(verticalHeaderChanged(int)));

  auto checkBoxLayout = new QHBoxLayout;
  checkBoxLayout->addWidget(gridCheckBox);
  checkBoxLayout->addStretch();
  checkBoxLayout->addWidget(navigationCheckBox);

  auto outerLayout = new QGridLayout;
  outerLayout->addWidget(localeLabel, 0, 0);
  outerLayout->addWidget(localeCombo, 0, 1);
  outerLayout->addWidget(firstDayLabel, 1, 0);
  outerLayout->addWidget(firstDayCombo, 1, 1);
  outerLayout->addWidget(selectionModeLabel, 2, 0);
  outerLayout->addWidget(selectionModeCombo, 2, 1);
  outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
  outerLayout->addWidget(horizontalHeaderLabel, 4, 0);
  outerLayout->addWidget(horizontalHeaderCombo, 4, 1);
  outerLayout->addWidget(verticalHeaderLabel, 5, 0);
  outerLayout->addWidget(verticalHeaderCombo, 5, 1);
  generalOptionsGroupBox->setLayout(outerLayout);

}

void CalendarWindow::createDatesGroupBox() {
  datesGroupBox = new QGroupBox(tr("Dates"));

  auto minimumDateEdit = new QDateEdit;
  minimumDateEdit->setDisplayFormat("MMM d yyyy");
  minimumDateEdit->setDateRange(calendar->minimumDate(),
                                calendar->maximumDate());
  minimumDateEdit->setDate(calendar->minimumDate());

  auto minimumDateLabel = new QLabel(tr("&Minimum Date:"));
  minimumDateLabel->setBuddy(minimumDateEdit);

  auto currentDateEdit = new QDateEdit;
  currentDateEdit->setDisplayFormat("MMM d yyyy");
  currentDateEdit->setDate(calendar->selectedDate());
  currentDateEdit->setDateRange(calendar->minimumDate(),
                                calendar->maximumDate());

  auto currentDateLabel = new QLabel(tr("&Current Date:"));
  currentDateLabel->setBuddy(currentDateEdit);

  auto maximumDateEdit = new QDateEdit;
  maximumDateEdit->setDisplayFormat("MMM d yyyy");
  maximumDateEdit->setDateRange(calendar->minimumDate(),
                                calendar->maximumDate());
  maximumDateEdit->setDate(calendar->maximumDate());

  auto maximumDateLabel = new QLabel(tr("Ma&ximum Date:"));
  maximumDateLabel->setBuddy(maximumDateEdit);

  auto dateBoxLayout = new QGridLayout;
  dateBoxLayout->addWidget(currentDateLabel, 1, 0);
  dateBoxLayout->addWidget(currentDateEdit, 1, 1);
  dateBoxLayout->addWidget(minimumDateLabel, 0, 0);
  dateBoxLayout->addWidget(minimumDateEdit, 0, 1);
  dateBoxLayout->addWidget(maximumDateLabel, 2, 0);
  dateBoxLayout->addWidget(maximumDateEdit, 2, 1);
  dateBoxLayout->setRowStretch(3, 1);

  datesGroupBox->setLayout(dateBoxLayout);
}

void CalendarWindow::createTextFormatsGroupBox() {
  textFormatsGroupBox = new QGroupBox(tr("Text Formats"));
  weekdayColorCombo = createColorComboBox();
  weekdayColorCombo->setCurrentIndex(weekdayColorCombo->findText(tr("Black")));

  auto weekdayColorLabel = new QLabel(tr("&Weekday color:"));
  weekdayColorLabel->setBuddy(weekdayColorCombo);

  weekendColorCombo = createColorComboBox();
  weekendColorCombo->setCurrentIndex(weekendColorCombo->findText(tr("Red")));

  auto weekendColorLabel = new QLabel(tr("Week&end color:"));
  weekendColorLabel->setBuddy(weekendColorCombo);

  headerTextFormatCombo = new QComboBox;
  headerTextFormatCombo->addItem(tr("Bold"));
  headerTextFormatCombo->addItem(tr("Italic"));
  headerTextFormatCombo->addItem(tr("Plain"));

  auto headerTextFormatLabel = new QLabel(tr("&Header text:"));
  headerTextFormatLabel->setBuddy(headerTextFormatCombo);

  firstFridayCheckBox = new QCheckBox(tr("&First Friday in blue"));
  mayFirstCheckBox = new QCheckBox(tr("May &1 in red"));

  auto checkBoxLayout = new QHBoxLayout;
  checkBoxLayout->addWidget(firstFridayCheckBox);
  checkBoxLayout->addStretch();
  checkBoxLayout->addWidget(mayFirstCheckBox);

  auto outerLayout = new QGridLayout;
  outerLayout->addWidget(weekdayColorLabel, 0, 0);
  outerLayout->addWidget(weekdayColorCombo, 0, 1);
  outerLayout->addWidget(weekendColorLabel, 1, 0);
  outerLayout->addWidget(weekendColorCombo, 1, 1);
  outerLayout->addWidget(headerTextFormatLabel, 2, 0);
  outerLayout->addWidget(headerTextFormatCombo, 2, 1);
  outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
  textFormatsGroupBox->setLayout(outerLayout);

}

QComboBox *CalendarWindow::createColorComboBox() {
  auto comboBox = new QComboBox;
  comboBox->addItem(tr("Red"), QColor(Qt::red));
  comboBox->addItem(tr("Blue"), QColor(Qt::blue));
  comboBox->addItem(tr("Black"), QColor(Qt::black));
  comboBox->addItem(tr("Magenta"), QColor(Qt::magenta));
  return comboBox;
}
