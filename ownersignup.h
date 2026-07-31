#ifndef OWNERSIGNUP_H
#define OWNERSIGNUP_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class MainWindow;

class OwnerSignUp : public QWidget {
    Q_OBJECT
public:
    explicit OwnerSignUp(MainWindow *mainWindow, QWidget *parent = nullptr);

private slots:
    void onSignUpClicked();
    void onBackClicked();

private:
    MainWindow *m_mainWindow;
    QLineEdit *m_fullNameEdit;
    QLineEdit *m_loginIdEdit;
    QLineEdit *m_pinEdit;
    QLineEdit *m_confirmPinEdit;
    QComboBox *m_secQuestionCombo;
    QLineEdit *m_secAnswerEdit;
    QPushButton *m_signUpBtn;
    QPushButton *m_backBtn;
    QLabel *m_statusLabel;
};

#endif // OWNERSIGNUP_H
