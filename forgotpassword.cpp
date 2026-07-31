#include "forgotpassword.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>
#include <QFrame>

ForgotPassword::ForgotPassword(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {


    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(
        "ForgotPassword { background-color: #F8F9FC; }"
        "QLabel { color: #1A1A2E; background-color: transparent; }"
        );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(60, 40, 60, 40);

    auto *titleLabel = new QLabel("Forgot Password");
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #1A1A2E;");
    titleLabel->setAlignment(Qt::AlignCenter);

    m_statusLabel = new QLabel;
    m_statusLabel->setStyleSheet("color: #E53935;");
    m_statusLabel->setAlignment(Qt::AlignCenter);

    // Shared style strings so every field/button looks the same, easy to tweak
    QString inputStyle =
        "QLineEdit {"
        "  background-color: white;"
        "  color: #1A1A2E;"
        "  border: 1px solid #DDDDDD;"
        "  border-radius: 8px;"
        "  padding: 10px;"
        "  font-size: 13px;"
        "}";

    QString primaryButtonStyle =
        "QPushButton {"
        "  background-color: #3F6FEB;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 10px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #3660D0; }";

    QString linkButtonStyle =
        "QPushButton {"
        "  background-color: transparent;"
        "  color: #3F6FEB;"
        "  border: none;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover { color: #2A4FC0; }";

    // Step 1: Enter login ID
    auto *step1Layout = new QFormLayout;
    step1Layout->setSpacing(15);
    m_loginIdEdit = new QLineEdit;
    m_loginIdEdit->setPlaceholderText("Enter your login ID");
    m_loginIdEdit->setStyleSheet(inputStyle);
    m_loginIdEdit->setMinimumHeight(40);
    step1Layout->addRow("Login ID:", m_loginIdEdit);

    m_loadBtn = new QPushButton("Next");
    m_loadBtn->setFixedHeight(45);
    m_loadBtn->setStyleSheet(primaryButtonStyle);

    // Step 2: Answer security question
    m_step2Widget = new QWidget;
    m_step2Widget->setAttribute(Qt::WA_StyledBackground, true);
    m_step2Widget->setStyleSheet("background-color: #F8F9FC;");
    auto *step2Layout = new QFormLayout(m_step2Widget);
    step2Layout->setContentsMargins(0, 0, 0, 0);
    step2Layout->setSpacing(15);

    m_questionLabel = new QLabel;
    QFont qFont;
    qFont.setBold(true);
    m_questionLabel->setFont(qFont);
    m_questionLabel->setStyleSheet("color: #1A1A2E;");
    m_questionLabel->setWordWrap(true);
    step2Layout->addRow("Question:", m_questionLabel);

    m_answerEdit = new QLineEdit;
    m_answerEdit->setPlaceholderText("Your answer");
    m_answerEdit->setStyleSheet(inputStyle);
    m_answerEdit->setMinimumHeight(40);
    step2Layout->addRow("Answer:", m_answerEdit);

    m_verifyBtn = new QPushButton("Verify");
    m_verifyBtn->setFixedHeight(45);
    m_verifyBtn->setStyleSheet(primaryButtonStyle);
    step2Layout->addRow("", m_verifyBtn);

    // Step 3: Set new PIN
    m_step3Widget = new QWidget;
    m_step3Widget->setAttribute(Qt::WA_StyledBackground, true);
    m_step3Widget->setStyleSheet("background-color: #F8F9FC;");
    auto *step3Layout = new QFormLayout(m_step3Widget);
    step3Layout->setContentsMargins(0, 0, 0, 0);
    step3Layout->setSpacing(15);

    m_newPinEdit = new QLineEdit;
    m_newPinEdit->setEchoMode(QLineEdit::Password);
    m_newPinEdit->setPlaceholderText("New PIN (min 4 chars)");
    m_newPinEdit->setStyleSheet(inputStyle);
    m_newPinEdit->setMinimumHeight(40);
    step3Layout->addRow("New PIN:", m_newPinEdit);

    m_confirmPinEdit = new QLineEdit;
    m_confirmPinEdit->setEchoMode(QLineEdit::Password);
    m_confirmPinEdit->setPlaceholderText("Confirm new PIN");
    m_confirmPinEdit->setStyleSheet(inputStyle);
    m_confirmPinEdit->setMinimumHeight(40);
    step3Layout->addRow("Confirm:", m_confirmPinEdit);

    m_resetBtn = new QPushButton("Reset PIN");
    m_resetBtn->setFixedHeight(45);
    m_resetBtn->setStyleSheet(primaryButtonStyle);
    step3Layout->addRow("", m_resetBtn);

    m_backBtn = new QPushButton("Back to Login");
    m_backBtn->setStyleSheet(linkButtonStyle);
    m_backBtn->setCursor(Qt::PointingHandCursor);

    // Assemble layout with generous spacing like the Login screen
    mainLayout->addSpacing(20);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(30);

    m_step1Widget = new QWidget;
    m_step1Widget->setAttribute(Qt::WA_StyledBackground, true);
    m_step1Widget->setStyleSheet("background-color: #F8F9FC;");
    auto *s1 = new QVBoxLayout(m_step1Widget);
    s1->setContentsMargins(0, 0, 0, 0);
    s1->setSpacing(20);
    s1->addLayout(step1Layout);
    s1->addWidget(m_loadBtn);
    mainLayout->addWidget(m_step1Widget);

    mainLayout->addWidget(m_step2Widget);
    m_step2Widget->hide();
    mainLayout->addWidget(m_step3Widget);
    m_step3Widget->hide();

    mainLayout->addSpacing(15);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(m_backBtn, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    connect(m_loadBtn, &QPushButton::clicked, this, &ForgotPassword::onLoadQuestion);
    connect(m_verifyBtn, &QPushButton::clicked, this, &ForgotPassword::onVerifyAnswer);
    connect(m_resetBtn, &QPushButton::clicked, this, &ForgotPassword::onResetPin);
    connect(m_backBtn, &QPushButton::clicked, this, &ForgotPassword::onBackClicked);

    showStep(1);
}

void ForgotPassword::activate(const QVariantMap &) {
    m_loginIdEdit->clear();
    m_answerEdit->clear();
    m_newPinEdit->clear();
    m_confirmPinEdit->clear();
    m_statusLabel->clear();
    showStep(1);
}

void ForgotPassword::showStep(int step) {
    m_step1Widget->setVisible(step == 1);
    m_step2Widget->setVisible(step == 2);
    m_step3Widget->setVisible(step == 3);
    m_statusLabel->clear();
}

void ForgotPassword::onLoadQuestion() {
    QString loginId = m_loginIdEdit->text().trimmed();
    if (loginId.isEmpty()) {
        m_statusLabel->setText("Please enter your Login ID.");
        return;
    }

    QVariantMap owner = m_mainWindow->db().getOwner(loginId);
    if (owner.isEmpty()) {
        m_statusLabel->setText("Login ID not found.");
        return;
    }

    m_currentLoginId = loginId;
    m_storedAnswer = owner["security_answer"].toString();
    m_questionLabel->setText(owner["security_question"].toString());
    m_statusLabel->clear();
    showStep(2);
}

void ForgotPassword::onVerifyAnswer() {
    QString answer = m_answerEdit->text().trimmed().toLower();
    if (answer.isEmpty()) {
        m_statusLabel->setText("Please enter your answer.");
        return;
    }

    if (answer == m_storedAnswer) {
        m_statusLabel->clear();
        showStep(3);
    } else {
        m_statusLabel->setText("Incorrect answer. Please try again.");
    }
}

void ForgotPassword::onResetPin() {
    QString newPin = m_newPinEdit->text().trimmed();
    QString confirmPin = m_confirmPinEdit->text().trimmed();

    if (newPin.isEmpty()) {
        m_statusLabel->setText("Please enter a new PIN.");
        return;
    }

    if (newPin.length() < 4) {
        m_statusLabel->setText("PIN must be at least 4 characters.");
        return;
    }

    if (newPin != confirmPin) {
        m_statusLabel->setText("PINs do not match.");
        return;
    }

    if (m_mainWindow->db().updateOwnerPin(m_currentLoginId, newPin)) {
        QMessageBox successBox(this);
        successBox.setWindowTitle("Success");
        successBox.setText("PIN updated successfully!");
        successBox.setIcon(QMessageBox::Information);
        successBox.setStyleSheet(
            "QMessageBox { background-color: #F8F9FC; }"
            "QLabel { color: #1A1A2E; }"
            "QPushButton {"
            "  background-color: #3F6FEB;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 6px;"
            "  padding: 6px 16px;"
            "}"
            "QPushButton:hover { background-color: #3660D0; }"
            );
        successBox.exec();
        m_mainWindow->switchToScreen(Screen::OwnerLogin, {{"resetLockout", true}});
    } else {
        m_statusLabel->setText("Failed to update PIN. Try again.");
    }
}

void ForgotPassword::onBackClicked() {
    m_mainWindow->switchToScreen(Screen::OwnerLogin, {{"resetLockout", true}});
}
