#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMap>
#include "enums.h"
#include "database.h"

class RoleSelection;
class OwnerSignUp;
class OwnerLogin;
class ForgotPassword;
class OwnerMainMenu;
class NewOrder;
class CustomerDetails;
class BillingView;
class CustomerFlow;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void switchToScreen(Screen screen, const QVariantMap &args = QVariantMap());
    DatabaseManager& db();

    void setCurrentOwner(int ownerId, const QString &ownerName);
    int currentOwnerId() const;
    QString currentOwnerName() const;
    void clearSession();

private:
    void setupUi();
    void createScreens();

    QStackedWidget *m_stack;
    DatabaseManager m_db;

    RoleSelection     *m_roleSelection;
    OwnerSignUp       *m_ownerSignUp;
    OwnerLogin        *m_ownerLogin;
    ForgotPassword    *m_forgotPassword;
    OwnerMainMenu     *m_ownerMainMenu;
    NewOrder          *m_newOrder;
    CustomerDetails   *m_customerDetails;
    BillingView       *m_billingView;
    CustomerFlow      *m_customerFlow;

    QMap<Screen, int> m_indices;
    int m_nextIndex = 0;

    int m_currentOwnerId = -1;
    QString m_currentOwnerName;
};

#endif // MAINWINDOW_H
