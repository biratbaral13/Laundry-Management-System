#include "ownersignup.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>

OwnerSignUp::OwnerSignUp(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {


    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: #F8F9FC;");

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    auto *titleLabel = new QLabel("Owner Sign Up");
    QFont titleFont;
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #1A1A2E;");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *formLayout = new QFormLayout;
    formLayout->setSpacing(15);

    QString labelStyle = "color: #1A1A2E;";
    QString fieldStyle = "background-color: white; color: #1A1A2E; border: 1px solid #DDDDDD; border-radius: 6px; padding: 8px;";

    auto *fullNameLabel = new QLabel("Full Name:");
    fullNameLabel->setStyleSheet(labelStyle);
    m_fullNameEdit = new QLineEdit;
    m_fullNameEdit->setPlaceholderText("Enter your full name");
    m_fullNameEdit->setStyleSheet(fieldStyle);
    formLayout->addRow(fullNameLabel, m_fullNameEdit);

    auto *loginIdLabel = new QLabel("Login ID:");
    loginIdLabel->setStyleSheet(labelStyle);
    m_loginIdEdit = new QLineEdit;
    m_loginIdEdit->setPlaceholderText("Choose a login ID");
    m_loginIdEdit->setStyleSheet(fieldStyle);
    formLayout->addRow(loginIdLabel, m_loginIdEdit);

    auto *pinLabel = new QLabel("PIN:");
    pinLabel->setStyleSheet(labelStyle);
    m_pinEdit = new QLineEdit;
    m_pinEdit->setEchoMode(QLineEdit::Password);
    m_pinEdit->setPlaceholderText("Enter a PIN");
    m_pinEdit->setStyleSheet(fieldStyle);
    formLayout->addRow(pinLabel, m_pinEdit);

    auto *confirmPinLabel = new QLabel("Confirm PIN:");
    confirmPinLabel->setStyleSheet(labelStyle);
    m_confirmPinEdit = new QLineEdit;
    m_confirmPinEdit->setEchoMode(QLineEdit::Password);
    m_confirmPinEdit->setPlaceholderText("Confirm your PIN");
    m_confirmPinEdit->setStyleSheet(fieldStyle);
    formLayout->addRow(confirmPinLabel, m_confirmPinEdit);

    auto *secQuestionLabel = new QLabel("Security Question:");
    secQuestionLabel->setStyleSheet(labelStyle);
    m_secQuestionCombo = new QComboBox;
    m_secQuestionCombo->addItem("What is your mother's maiden name?");
    m_secQuestionCombo->addItem("What was the name of your first pet?");
    m_secQuestionCombo->addItem("What city were you born in?");
    m_secQuestionCombo->addItem("What is the name of your favorite teacher?");
    m_secQuestionCombo->addItem("What was the make of your first car?");
    m_secQuestionCombo->setStyleSheet(fieldStyle);
    formLayout->addRow(secQuestionLabel, m_secQuestionCombo);

    auto *answerLabel = new QLabel("Answer:");
    answerLabel->setStyleSheet(labelStyle);
    m_secAnswerEdit = new QLineEdit;
    m_secAnswerEdit->setPlaceholderText("Your answer");
    m_secAnswerEdit->setStyleSheet(fieldStyle);
    formLayout->addRow(answerLabel, m_secAnswerEdit);

    m_statusLabel = new QLabel;
    m_statusLabel->setStyleSheet("color: red;");
    m_statusLabel->setAlignment(Qt::AlignCenter);

    m_signUpBtn = new QPushButton("Sign Up");
    m_signUpBtn->setFixedHeight(40);
    m_signUpBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background-color: #388E3C; }"
        );

    m_backBtn = new QPushButton("Back to Login");
    m_backBtn->setFixedHeight(35);
    m_backBtn->setStyleSheet("color: #1976D2; border: none;");

    mainLayout->addSpacing(20);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_signUpBtn);
    mainLayout->addWidget(m_backBtn);
    mainLayout->addStretch();

    connect(m_signUpBtn, &QPushButton::clicked, this, &OwnerSignUp::onSignUpClicked);
    connect(m_backBtn, &QPushButton::clicked, this, &OwnerSignUp::onBackClicked);
}

void OwnerSignUp::onSignUpClicked() {
    QString fullName = m_fullNameEdit->text().trimmed();
    QString loginId = m_loginIdEdit->text().trimmed();
    QString pin = m_pinEdit->text().trimmed();
    QString confirmPin = m_confirmPinEdit->text().trimmed();
    QString secQuestion = m_secQuestionCombo->currentText();
    QString secAnswer = m_secAnswerEdit->text().trimmed();

    if (fullName.isEmpty() || loginId.isEmpty() || pin.isEmpty() || secAnswer.isEmpty()) {
        m_statusLabel->setText("All fields are required.");
        return;
    }

    if (pin != confirmPin) {
        m_statusLabel->setText("PINs do not match.");
        return;
    }

    if (pin.length() < 4) {
        m_statusLabel->setText("PIN must be at least 4 characters.");
        return;
    }

    QVariantMap existing = m_mainWindow->db().getOwner(loginId);
    if (!existing.isEmpty()) {
        m_statusLabel->setText("Login ID already exists.");
        return;
    }

    if (m_mainWindow->db().addOwner(fullName, loginId, pin, secQuestion, secAnswer)) {
        QMessageBox box(this);
        box.setWindowTitle("Success");
        box.setText("Account created successfully!");
        box.setAttribute(Qt::WA_StyledBackground, true);
        box.setStyleSheet(
            "QMessageBox { background-color: #F8F9FC; }"
            "QLabel { color: #1A1A2E; }"
            "QPushButton { background-color: #4CAF50; color: white; border-radius: 6px; padding: 6px 16px; }"
            "QPushButton:hover { background-color: #388E3C; }"
            );
        box.exec();
        m_mainWindow->switchToScreen(Screen::OwnerLogin);
    } else {
        m_statusLabel->setText("Sign up failed. Please try again.");
    }
}

void OwnerSignUp::onBackClicked() {
    m_mainWindow->switchToScreen(Screen::OwnerLogin);
}