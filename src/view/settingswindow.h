// ---------------------------- //
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "iviewsettings.h"
// ---------------------------- //
namespace Ui { class SettingsWindow; }
// ---------------------------- //

class SettingsWindow : public QWidget, public IViewSettings
{
    Q_OBJECT

    int workingRate;

public:

    explicit SettingsWindow( QWidget * parent = 0 );
    ~SettingsWindow();

public: // From Interfaces

    void setWorkingRate( int rate );
    void setWorkingRates( QStringList ratesList );

private:

    Ui::SettingsWindow * gui;

private slots:

    void selectWorkingRate( int rate );

    void currentWorkingRate();
    void saveWorkingRate();

signals: // From Interfaces

    void changedWorkingRate( int );
};

#endif // SETTINGSWINDOW_H
