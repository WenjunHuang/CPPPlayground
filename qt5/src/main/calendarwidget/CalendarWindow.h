#pragma once
#include <QWidget>

class QGroupBox;
class QClandarWidget;
class QGridLayout;
class QComboBox;
class QCalendarWidget;
class QLabel;
class QCheckBox;

class CalendarWindow : public QWidget{
  Q_OBJECT
public:
  CalendarWindow();

  void createPreviewGroupBox();

  QGroupBox *previewGroupBox;
  QCalendarWidget *calendar;
  QGridLayout *previewLayout;

  void createGeneralOptionsGroupBox();

  QGroupBox *generalOptionsGroupBox;
  QComboBox *localeCombo;
  QLabel *localeLabel;
  QComboBox *firstDayCombo;
  QLabel *firstDayLabel;
  QComboBox *selectionModeCombo;
  QLabel *selectionModeLabel;
  QCheckBox *gridCheckBox;
  QCheckBox *navigationCheckBox;
  QComboBox *horizontalHeaderCombo;
  QLabel *horizontalHeaderLabel;
  QComboBox *verticalHeaderCombo;

  void createDatesGroupBox();

  QGroupBox *datesGroupBox;

  void createTextFormatsGroupBox();

  QGroupBox *textFormatsGroupBox;

  QComboBox *createColorComboBox();

  QComboBox *weekdayColorCombo;
  QComboBox *weekendColorCombo;
  QComboBox *headerTextFormatCombo;
  QCheckBox *firstFridayCheckBox;
  QCheckBox *mayFirstCheckBox;
};



