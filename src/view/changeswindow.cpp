// ---------------------------- //
#include <QTextEdit>
// ---------------------------- //
#include "coreworktime.h"
// ---------------------------- //
#include "changeswindow.h"
#include "ui_changeswindow.h"
// ---------------------------- //

ChangesWindow::ChangesWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::ChangesWindow )
{
    gui->setupUi( this );

    ScrollWidget = new QWidget( this );
    MainScrollLayout = new QVBoxLayout( ScrollWidget  );

    gui->ScrollViewInfo->setWidget( ScrollWidget );

    setInfo();
}
// ------------------------------------------------------------------------------------ //

ChangesWindow::~ChangesWindow()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void ChangesWindow::addInfoAboutVersion( QString Version, QString Title, QString Content )
{
    QLabel *TitleLabel = new QLabel( ScrollWidget );

    TitleLabel->setWordWrap( true );
    TitleLabel->setMargin( 7 );

    TitleLabel->setStyleSheet( "border: 1px outset #006400;"\
                               "border-radius: 3;" \
                               "background-color: #404040;");

    TitleLabel->setText( QString("<p align='right' margin='5'>Версия %1</p>"\
                                 "<center><h3><font color=\"%4\">%2</font></h3></center>"\
                                 "<p>%3</p>").arg(Version).arg(Title).arg(Content).arg("#C5C5C5") );

    MainScrollLayout->addWidget( TitleLabel );

}
// ------------------------------------------------------------------------------------ //

void ChangesWindow::setInfo()
{
    gui->TitleLabel->setText( tr("Текущая версия: %1.%2.%3")
                              .arg(VERSION_MAJOR   )
                              .arg(VERSION_MINOR   )
                              .arg(VERSION_SUBMINOR) );

    addInfoAboutVersion( tr("1.1.21"),
                         tr("В зарплате"),
                         tr("В статистике добавлены разделители в числа.<br/>") );

    addInfoAboutVersion( tr("1.1.20"),
                         tr("Снова время работы за день"),
                         tr("Там, где указано время за день (правый нижний угол), появилась опция 'Использовать моё время'.<br/>"\
                            "Выбирая ее, Вы сможете изменить, сколько времени нужно отработать за день.<br/>"\
                            "<u>Важно: указывая своё время, сколько нужно отработать, пересчет обеда не происходит!</u>") );

    addInfoAboutVersion( tr("1.1.19"),
                         tr("Время рабочего дня"),
                         tr("Исправлено изменение времени, сколько нужно отработать за день.<br/>"\
                         "<u>Если Вы его меняете - указывайте время сразу с учетом обеда</u>.<br/>"\
                         "Время обеда в таком случае не пересчитывается!<br/>") );

    addInfoAboutVersion( tr("1.1.18"),
                         tr("Виджет рабочего стола"),
                         tr("Он стал чуть интереснее: добавлена стилизация.<br/>") );

    addInfoAboutVersion( tr("1.1.17"),
                         tr("+++"),
                         tr("1. Исправлено закрытие приложения, когда Вы этого не подозреваете.<br/>"\
                            "2. Добавилась статистика зарплаты.<br.>") );

    addInfoAboutVersion( tr("1.1.16"),
                         tr("Ваш доход за год теперь визуальнее!"),
                         tr("Добавлена гистограмма дохода.<br/>"\
                            "В меню пункт: Зарплата.<br.>"\
                            "Добавляйте зарплату и наблюдайте за графиком своего дохода :) ") );

    addInfoAboutVersion( tr("1.1.15"),
                         tr("Уведомление о конце рабочего дня"),
                         tr("Добавилась опция предупреждения, что скоро наступит время, когда надо точно уйти с работы.<br/>"\
                            "Настройки->Время->Максимальное время конца рабочего дня.<br.>"\
                            "За 15 минут, до указанного в этом поле времени, Вам поступит уведомление, если Вы включите эту опцию (по умолчанию она включена).") );

    addInfoAboutVersion( tr("1.1.14"),
                         tr("Улучшения"),
                         tr("1. Доработаны обновления.<br/>"\
                            "2. Исправления недочетов.<br/>") );

    addInfoAboutVersion( tr("1.1.13"),
                         tr("I'm sorry..."),
                         tr("1. Исправлен запуск программы.<br/>"\
                            "2. Исправлено обновление времени конца рабочего дня (если Вы выбрали НЕ сегодня в календаре и свернули программу - время конца не обновлялось).") );

    addInfoAboutVersion( tr("1.1.12"),
                         tr("Bug fix +"),
                         tr("1. Исправлено изменение прозрачности главного окна.<br/>"\
                            "2. Добавлен сброс положения виджета:<br/><u>Настройки->Виджет->Сбросить положения виджета.</u><br/>"\
                            "3. Доработана тема \"gray\".<br/>"\
                            "4. Исправлено сохранение времени, сколько отнимать и добавлять.<br/>"\
                            "5. Исправлен автозапуск программы.<br/>") );

    addInfoAboutVersion( tr("1.1.11"),
                         tr("HTML"),
                         tr("В редакторе заметки теперь можно вводить теги HTML. Кликните правой кнопкой "\
                            "в редакторе в выберете \"В HTML\" для преобразования текста.<br/>"\
                            "Пока работают не все теги HTML, только примитивные.<br/>") );

    addInfoAboutVersion( tr("1.1.10"),
                         tr("Чуть-чуть лучше"),
                         tr("Добавлена сетка в календаре.") );

    addInfoAboutVersion( tr("1.1.9"),
                         tr("Исправлены значения по умолчанию"),
                         tr("По умолчанию запускается темная тема.<br/>"\
                            "И трей используется по умолчанию теперь тоже.<br/>"\
                            "Время конца обеда теперь 15:00 (было 16:00).<br/><br/>"\
                            "Не паникуйте! Ничего у Вас не сбросится!<br/>Чтоб изменить: Меню -> Настройки.") );

    addInfoAboutVersion( tr("1.1.8"),
                         tr("Bug fix"),
                         tr("Исправлено отображение иконки кaлькулятора времени в меню.<br/>"\
                            "Оптимизация.") );

    addInfoAboutVersion( tr("1.1.7"),
                         tr("Добавлен калькулятор времени"),
                         tr("Где? Меню -> Калькулятор времени.<br/>"\
                            "Как пользоваться? Как и обычным калькулятором.") );

    addInfoAboutVersion( tr("1.1.6"),
                         tr("Изменения и исправления"),
                         tr("<b>1.</b> Доделана тема gray.<br/>"\
                            "<b>2.</b> Исправлен запуск двух экземпляров программы.<br/>"\
                            "<b>3.</b> Со следующим обновлением вы можете просто перетащить новую программу в окно WorkTime и она обновится.") );

    addInfoAboutVersion( tr("1.1.5"),
                         tr("Изменения и исправления"),
                         tr("<b>1.</b> Исправлено обновление времени конца интервала при автоматическом увеличении времени.<br/>"\
                            "<b>2.</b> Добавлен сивол '+' слева от таймера, если у Вы начали перерабытвать.") );

    addInfoAboutVersion( tr("1.1.4"),
                         tr("Изменения и исправления"),
                         tr("<b>1.</b> Исправлено обновление статистики времени при автоматическом увеличении времени.<br/>"\
                            "<b>2.</b> Окно \"Таблица времени\" теперь меньшего размера.<br/>"\
                            "<b>3.</b> В окне \"Зарплата\" теперь подсвечиваются дни с зарплатой.") );

    addInfoAboutVersion( tr("1.1.3"),
                         tr("Изменения и исправления"),
                         tr("<b>1.</b> После запуска таймера время устанавливается текущее время начала интервала.<br/>"\
                            "<b>2.</b> Добавлен экспорт(сохранение) и импорт(загрузка) данных. Воспользуйтесь, если, "\
                            "например, Вы собираетесь сменить компьютер.<br/>" \
                            "<b>3.</b> В окне \"Таблица времени\" появилась сверху строка. Если Вы хотите добавить "\
                            "какой-то текст при печати таблицы или её сохранении - введите его в строке, и она будет " \
                            "добавлено в конец таблицы.") );

    addInfoAboutVersion( tr("1.1.2"),
                         tr("Изменения и исправления"),
                         tr("<b>1.</b> Таблица времени: добавлен знак '-', если за рабочий день ничего не отработано.<br/>"\
                            "<b>2.</b> Остановить/Запустить: если Вы остановили таймер, то после его дальнейшего "\
                            "запуска автоматически будет добавлен интервал.<br/>"\
                            "<b>3.</b> Тоже про таймер: теперь отображается кнопка, на какое состояние можно переключить "\
                            "таймер. То есть, если у Вас таймер запущен, будет отображаться кнопка остановки таймера, "\
                            "и наоборот, если таймер остановлен, то будет отображаться кнопка запуска таймера.") );

    addInfoAboutVersion( tr("1.1.1"),
                         tr("Добро пожаловать!"),
                         tr("На этом месте могла бы быть Ваша реклама...") );
}
// ------------------------------------------------------------------------------------ //
