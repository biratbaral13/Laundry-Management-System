#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class MainWindow;

class ForgotPassword : public QWidget {
    Q_OBJECT
public:
    explicit ForgotPassword(MainWindow *mainWindow, QWidget *parent = nullptr);
    void activate(const QVariantMap &args = QVariantMap());

private slots:
    void onLoadQuestion();
    void onVerifyAnswer();
    void onResetPin();
    void onBackClicked();

private:
    void showStep(int step);

    MainWindow *m_mainWindow;

    QLineEdit *m_loginIdEdit;
    QPushButton *m_loadBtn;

    QLabel *m_questionLabel;
    QLineEdit *m_answerEdit;
    QPushButton *m_verifyBtn;

    QLineEdit *m_newPinEdit;
    QLineEdit *m_confirmPinEdit;
    QPushButton *m_resetBtn;

    QLabel *m_statusLabel;
    QPushButton *m_backBtn;

    QWidget *m_step1Widget;
    QWidget *m_step2Widget;
    QWidget *m_step3Widget;

    QString m_currentLoginId;
    QString m_storedAnswer;
};

#endif // FORGOTPASSWORD_H
