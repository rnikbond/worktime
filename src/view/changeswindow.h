// ---------------------------- //
#ifndef CHANGESWINDOW_H
#define CHANGESWINDOW_H
// ---------------------------- //
#include <QWidget>
#include <QVBoxLayout>
// ---------------------------- //
namespace Ui { class ChangesWindow; }
// ---------------------------- //

class ChangesWindow : public QWidget
{
    Q_OBJECT

    QVBoxLayout *MainScrollLayout;
    QWidget *ScrollWidget;

public:

    explicit ChangesWindow( QWidget * parent = 0 );
    ~ChangesWindow();

private:

    Ui::ChangesWindow * gui;

    void setInfo();

    void addInfoAboutVersion( QString Version, QString Title, QString Content );
};

#endif // CHANGESWINDOW_H
