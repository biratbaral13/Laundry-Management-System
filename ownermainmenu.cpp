#include "ownermainmenu.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QMessageBox>

OwnerMainMenu::OwnerMainMenu(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {

    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(248, 250, 252));
    setPalette(pal);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    m_welcomeLabel = new QLabel;
    QFont welcomeFont;
    welcomeFont.setPointSize(18);
    welcomeFont.setBold(true);
    m_welcomeLabel->setFont(welcomeFont);
    m_welcomeLabel->setStyleSheet("color: #0F172A;font-family: 'Gotham Rounded';font-size: 32px;");
    m_welcomeLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel("What would you like to do?");
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "    color: #0F172A;"
        "    font-family: 'Gotham Rounded';"
        "    font-size: 14px;"
        "    font-weight: 400;"
        "    padding: 4px 0px;"
        "}"
        );
    subtitleLabel->setAlignment(Qt::AlignCenter);

    auto *btnLayout = new QVBoxLayout;
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->setSpacing(15);

    QFont btnFont("Gotham Rounded",14,QFont::Light);
    btnFont.setPointSize(14);

    m_newOrderBtn = new QPushButton("New Order");
    m_newOrderBtn->setFixedSize(280, 60);
    m_newOrderBtn->setFont(btnFont);
    m_newOrderBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; border-radius: 30px; }"
        "QPushButton:hover { background-color: #1976D2; }"
    );

    m_customerDetailsBtn = new QPushButton("Customer Details");
    m_customerDetailsBtn->setFixedSize(280, 60);
    m_customerDetailsBtn->setFont(btnFont);
    m_customerDetailsBtn->setStyleSheet(
        "QPushButton { background-color: #FF9800; color: white; border-radius:30px; }"
        "QPushButton:hover { background-color: #F57C00; }"
    );

    m_billingBtn = new QPushButton("Billing");
    m_billingBtn->setFixedSize(280, 60);
    m_billingBtn->setFont(btnFont);
    m_billingBtn->setStyleSheet(
        "QPushButton { background-color: #9C27B0; color: white; border-radius: 30px; }"
        "QPushButton:hover { background-color: #7B1FA2; }"
    );

    m_logoutBtn = new QPushButton("Logout");
    m_logoutBtn->setFixedSize(280, 45);
    QFont logoutFont("Gotham Rounded",14,QFont::Light);
    logoutFont.setPointSize(12);
    m_logoutBtn->setFont(logoutFont);
    m_logoutBtn->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; border-radius: 22px; }"
        "QPushButton:hover { background-color: #d32f2f; }"
    );

    btnLayout->addWidget(m_newOrderBtn);
    btnLayout->addWidget(m_customerDetailsBtn);
    btnLayout->addWidget(m_billingBtn);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(m_logoutBtn);

    mainLayout->addSpacing(40);
    mainLayout->addWidget(m_welcomeLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(40);
    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch();

    connect(m_newOrderBtn, &QPushButton::clicked, this, &OwnerMainMenu::onNewOrderClicked);
    connect(m_customerDetailsBtn, &QPushButton::clicked, this, &OwnerMainMenu::onCustomerDetailsClicked);
    connect(m_billingBtn, &QPushButton::clicked, this, &OwnerMainMenu::onBillingClicked);
    connect(m_logoutBtn, &QPushButton::clicked, this, &OwnerMainMenu::onLogoutClicked);
}

void OwnerMainMenu::activate(const QVariantMap &) {
    m_welcomeLabel->setText("Welcome, " + m_mainWindow->currentOwnerName());
}

void OwnerMainMenu::onNewOrderClicked() {
    m_mainWindow->switchToScreen(Screen::NewOrder);
}

void OwnerMainMenu::onCustomerDetailsClicked() {
    m_mainWindow->switchToScreen(Screen::CustomerDetails);
}

void OwnerMainMenu::onBillingClicked() {
    m_mainWindow->switchToScreen(Screen::BillingView);
}

void OwnerMainMenu::onLogoutClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Logout", "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_mainWindow->clearSession();
        m_mainWindow->switchToScreen(Screen::RoleSelection);
    }
}
