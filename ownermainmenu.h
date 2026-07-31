#ifndef OWNERMAINMENU_H
#define OWNERMAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class MainWindow;

class OwnerMainMenu : public QWidget {
    Q_OBJECT
public:
    explicit OwnerMainMenu(MainWindow *mainWindow, QWidget *parent = nullptr);
    void activate(const QVariantMap &args = QVariantMap());

private slots:
    void onNewOrderClicked();
    void onCustomerDetailsClicked();
    void onBillingClicked();
    void onLogoutClicked();

private:
    MainWindow *m_mainWindow;
    QLabel *m_welcomeLabel;
    QPushButton *m_newOrderBtn;
    QPushButton *m_customerDetailsBtn;
    QPushButton *m_billingBtn;
    QPushButton *m_logoutBtn;
};

#endif // OWNERMAINMENU_H
