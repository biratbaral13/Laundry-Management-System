#include "ownerlogin.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>

OwnerLogin::OwnerLogin(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow),
      m_attempts(0), m_rounds(0), m_cooldownRemaining(0), m_permanentlyLocked(false) {



    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(248, 250, 252));
    setPalette(pal);

    m_cooldownTimer = new QTimer(this);
    m_cooldownTimer->setInterval(1000);
    connect(m_cooldownTimer, &QTimer::timeout, this, &OwnerLogin::onCooldownTick);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    //Login Title
    auto *titleLabel = new QLabel("Login");
    QFont titleFont("Gotham Rounded", 32, QFont::DemiBold);
    QString titleStyle =
        "QLabel {"
        "color: #0F172A;"
        "font-size: 32px;"
        "font-weight: semi-bold;"
        "font-family: Gotham Rounded;"
        "}";
    titleLabel->setStyleSheet(titleStyle);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *formLayout = new QFormLayout;
    formLayout->setSpacing(12);

    m_loginIdEdit = new QLineEdit;

    QString placeholderStyle=
        "QLineEdit {"
        "    color: black;"          // typed text color
        "    border: 1px solid #0F172A;"
        "    border-radius: 18px;"
        "    background-color: white;"
        "    padding: 8px;"
        "    font-size: 16px;"
        "    font-family: Gotham Rounded;"
        "}"
        "QLineEdit::placeholder {"
        "    color: black;"        // placeholder color
        "    font-style: italic;"
        "    font-family: Gotham Rounded;"
        "}"
        "QLineEdit::selection {"
         "    background-color: white;"  // selected area color
         "    border-bottom: 0px solid #2563EB;"
         "    border-top: 0px;"
         "    border-left: 0px;"
         "    border-right: 0px;"
         "    color: black;"               // selected text color
         "}";
    m_loginIdEdit->setPlaceholderText("Enter your login ID");
    m_loginIdEdit->setStyleSheet(placeholderStyle);

    QLabel *loginLabel = new QLabel("Login ID:");

    QString labelStyle =
        "QLabel {"
        "color: Black;"
        "font-size: 15px;"
        "font-weight: semi-bold;"
        "font-family: Gotham Rounded;"
        "}";
    loginLabel->setStyleSheet(labelStyle);

    formLayout->addRow(loginLabel, m_loginIdEdit);

    m_pinEdit = new QLineEdit;
    m_pinEdit->setEchoMode(QLineEdit::Password);
    m_pinEdit->setPlaceholderText("Enter your PIN");
    m_pinEdit->setStyleSheet(placeholderStyle);

    QLabel *pinLabel = new QLabel("PIN:");
    pinLabel->setStyleSheet(labelStyle);
    formLayout->addRow(pinLabel, m_pinEdit);

    m_statusLabel = new QLabel;
    QFont lableFont("Gotham Rounded", 13, QFont::Light);
    m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setFont(lableFont);
    m_statusLabel->setWordWrap(true);

    m_cooldownLabel = new QLabel;
    m_cooldownLabel->setStyleSheet("color: orange; font-size: 16px; font-weight: bold;");
    m_cooldownLabel->setAlignment(Qt::AlignCenter);
    m_cooldownLabel->setFont(lableFont);
    m_cooldownLabel->hide();

    m_loginBtn = new QPushButton("Login");
    m_loginBtn->setFixedHeight(40);
    m_loginBtn->setFont(lableFont);
    m_loginBtn->setStyleSheet(
        "QPushButton { background-color: #2563EB; color: white; border-radius: 20px; font-size: 14px; }"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:disabled { background-color: #BDBDBD; }"
    );

    m_forgotBtn = new QPushButton("Forgot Password?");
    m_forgotBtn->setFont(lableFont);
    m_forgotBtn->setStyleSheet("color: #1976D2; border: none; padding: 20px;");

    m_signUpBtn = new QPushButton("Create an Account");
    m_signUpBtn->setFont(lableFont);
    m_signUpBtn->setStyleSheet("color: #4CAF50; border: none; font-weight: bold;");

    mainLayout->addSpacing(30);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addWidget(m_cooldownLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_loginBtn);
    mainLayout->addWidget(m_forgotBtn);
    mainLayout->addWidget(m_signUpBtn);
    mainLayout->addStretch();

    connect(m_loginBtn, &QPushButton::clicked, this, &OwnerLogin::onLoginClicked);
    connect(m_forgotBtn, &QPushButton::clicked, this, &OwnerLogin::onForgotPasswordClicked);
    connect(m_signUpBtn, &QPushButton::clicked, this, &OwnerLogin::onSignUpClicked);
}

void OwnerLogin::activate(const QVariantMap &args) {
    if (args.value("resetLockout", false).toBool())
        resetLoginState();
    m_statusLabel->clear();
    updateUI();
}

void OwnerLogin::onLoginClicked() {
    if (m_permanentlyLocked) return;

    QString loginId = m_loginIdEdit->text().trimmed();
    QString pin = m_pinEdit->text().trimmed();

    if (loginId.isEmpty() || pin.isEmpty()) {
        m_statusLabel->setText("Please enter both Login ID and PIN.");
        return;
    }

    QVariantMap owner = m_mainWindow->db().getOwner(loginId);
    if (owner.isEmpty()) {
        m_statusLabel->setText("Invalid Login ID or PIN.");
        m_attempts++;
        if (m_attempts >= 3) {
            m_rounds++;
            if (m_rounds >= 3) {
                lockPermanently();
            } else {
                startCooldown();
            }
        }
        updateUI();
        return;
    }

    if (owner["is_locked"].toBool()) {
        m_permanentlyLocked = true;
        m_statusLabel->setText("Account is permanently locked.\nContact your developer.");
        updateUI();
        return;
    }

    if (owner["pin"].toString() != pin) {
        m_attempts++;
        int remaining = 3 - m_attempts;
        if (remaining > 0)
            m_statusLabel->setText(QString("Invalid PIN. %1 attempt(s) remaining this round.").arg(remaining));
        else {
            m_rounds++;
            if (m_rounds >= 3) {
                lockPermanently();
            } else {
                startCooldown();
            }
        }
        updateUI();
        return;
    }

    // Success
    m_mainWindow->setCurrentOwner(owner["id"].toInt(), owner["full_name"].toString());
    resetLoginState();
    m_mainWindow->switchToScreen(Screen::OwnerMainMenu);
}

void OwnerLogin::onForgotPasswordClicked() {
    m_mainWindow->switchToScreen(Screen::ForgotPassword);
}

void OwnerLogin::onSignUpClicked() {
    m_mainWindow->switchToScreen(Screen::OwnerSignUp);
}

void OwnerLogin::startCooldown() {
    m_cooldownRemaining = 30;
    m_attempts = 0;
    m_cooldownLabel->show();
    m_cooldownTimer->start();
    m_statusLabel->setText(QString("Too many attempts. Cooldown: %1 seconds").arg(m_cooldownRemaining));
    updateUI();
}

void OwnerLogin::onCooldownTick() {
    m_cooldownRemaining--;
    if (m_cooldownRemaining <= 0) {
        m_cooldownTimer->stop();
        m_cooldownLabel->hide();
        m_statusLabel->setText(QString("Round %1 of 3. You have 3 attempts.").arg(m_rounds + 1));
        updateUI();
        return;
    }
    m_cooldownLabel->setText(QString("Locked for %1 seconds").arg(m_cooldownRemaining));
    m_statusLabel->setText(QString("Cooldown active. %1 seconds remaining.").arg(m_cooldownRemaining));
}

void OwnerLogin::lockPermanently() {
    m_cooldownTimer->stop();
    m_cooldownLabel->hide();
    m_permanentlyLocked = true;

    QString loginId = m_loginIdEdit->text().trimmed();
    if (!loginId.isEmpty())
        m_mainWindow->db().lockOwner(loginId);

    m_statusLabel->setText("Account permanently locked.\nContact your developer.");
    updateUI();
}

void OwnerLogin::resetLoginState() {
    m_attempts = 0;
    m_rounds = 0;
    m_cooldownRemaining = 0;
    m_permanentlyLocked = false;
    m_cooldownTimer->stop();
    m_cooldownLabel->hide();
    m_loginIdEdit->clear();
    m_pinEdit->clear();
}

void OwnerLogin::updateUI() {
    bool locked = m_permanentlyLocked || m_cooldownTimer->isActive();
    m_loginBtn->setEnabled(!locked);
    m_loginIdEdit->setEnabled(!locked);
    m_pinEdit->setEnabled(!locked);
}
