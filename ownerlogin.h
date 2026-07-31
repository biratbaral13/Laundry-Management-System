#ifndef OWNERLOGIN_H
#define OWNERLOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

class MainWindow;

class OwnerLogin : public QWidget {
    Q_OBJECT
public:
    explicit OwnerLogin(MainWindow *mainWindow, QWidget *parent = nullptr);
    void activate(const QVariantMap &args = QVariantMap());

private slots:
    void onLoginClicked();
    void onForgotPasswordClicked();
    void onSignUpClicked();
    void onCooldownTick();

private:
    void startCooldown();
    void lockPermanently();
    void resetLoginState();
    void updateUI();

    MainWindow *m_mainWindow;

    QLineEdit *m_loginIdEdit;
    QLineEdit *m_pinEdit;
    QPushButton *m_loginBtn;
    QPushButton *m_forgotBtn;
    QPushButton *m_signUpBtn;
    QLabel *m_statusLabel;
    QLabel *m_cooldownLabel;

    QTimer *m_cooldownTimer;

    int m_attempts;       // wrong attempts in current round (0..3)
    int m_rounds;         // completed cooldown rounds (0..3)
    int m_cooldownRemaining;
    bool m_permanentlyLocked;
    QString m_loginId;    // login ID being attempted (for lockout tracking)
};

#endif // OWNERLOGIN_H
