#include "rolescreen.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

RoleSelection::RoleSelection(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {

    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(248, 250, 252));
    setPalette(pal);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);



    //Project Label
    m_titleLabel = new QLabel("Laundry Management System");
    QFont titleFont("Gotham Rounded", 32, QFont::Bold);
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color:#0F172A;");

    //Project Title
    m_subtitleLabel = new QLabel("Choose Your Role?");
    QFont subFont("Gotham Rounded", 32, QFont::DemiBold);
    subFont.setPointSize(14);
    m_subtitleLabel->setFont(subFont);
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    // m_subtitleLabel->setStyleSheet();
    m_subtitleLabel->setStyleSheet(
        "QLabel {"
        "padding: 15px 10px 0px 10px;"
        "color:#2563EB;"
        "}"
        );

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(60);

    //Owner Button
    m_ownerBtn = new QPushButton("Owner");
    m_ownerBtn->setFixedSize(130, 60);
    QFont btnFont("Gotham Rounded", 32, QFont::Light);
    btnFont.setPointSize(14);
    m_ownerBtn->setFont(btnFont);
    m_ownerBtn->setStyleSheet(
        "QPushButton { background-color: #10B981; color: white; border-radius: 30px; }"
        "QPushButton:hover { background-color: #34D399; }"
    );

    //Customer Button
    m_customerBtn = new QPushButton("Customer");
    m_customerBtn->setFixedSize(130, 60);
    m_customerBtn->setFont(btnFont);
    m_customerBtn->setStyleSheet(
        "QPushButton { background-color: #2563EB; color: white; border-radius: 30px; }"
        "QPushButton:hover { background-color: #3B82F6; }"
    );

    buttonLayout->addWidget(m_ownerBtn);
    buttonLayout->addWidget(m_customerBtn);

    mainLayout->addSpacing(80);
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_subtitleLabel);
    mainLayout->addSpacing(60);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    connect(m_ownerBtn, &QPushButton::clicked, this, &RoleSelection::onOwnerClicked);
    connect(m_customerBtn, &QPushButton::clicked, this, &RoleSelection::onCustomerClicked); //Customer Interface Connection
}

void RoleSelection::onOwnerClicked() { //Owner Role Seelection Switch
    m_mainWindow->switchToScreen(Screen::OwnerLogin);
}

void RoleSelection::onCustomerClicked() { //Customer Role Selection Switch
    m_mainWindow->switchToScreen(Screen::CustomerFlow);
}
