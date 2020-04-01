// ---------------------------- //
#include "iworktime.h"
// ---------------------------- //
#include "presenterworktime.h"
// ---------------------------- //

/*!
 * \brief PresenterWorkTime::PresenterWorkTime
 * \param View
 * \param parent
 *
 * В конструкторе происходит соединение сигналов от \a View.
 *
 * \sa PresenterWorkTime::connectView
 */
PresenterWorkTime::PresenterWorkTime( IWorkTime * View, QObject * parent ) : QObject( parent )
{
#ifdef QT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::PresenterWorkTime(...)";
#endif

    ViewWT = View;

    connectView( View );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userSelectDate
 * \param date Выбранная пользователем дата
 */
void PresenterWorkTime::userSelectDate( const QDate & date )
{

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::connectView
 * \param View Указатель на View
 *
 * Соединение сигналов от View со слотами.
 */
void PresenterWorkTime::connectView( IWorkTime * View )
{
    QObject * ViewObj = dynamic_cast<QObject*>( View );

    QObject::connect( ViewObj, SIGNAL(userSelectDate(QDate)), SLOT(userSelectDate(QDate)) );
}
// ------------------------------------------------------------------------------------ //
