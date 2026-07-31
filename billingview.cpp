#include "billingview.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFont>
#include <QFrame>

BillingView::BillingView(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow), m_comingFromCustomer(false) {

    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(248, 250, 252));
    setPalette(pal);

    auto *mainLayout = new QVBoxLayout(this);

    // Header
    auto *headerLayout = new QHBoxLayout;
    auto *titleLabel = new QLabel("Billing");
    QFont titleFont("Gotham Rounded", 32, QFont::Light);
    titleLabel->setStyleSheet("color: #0F172A;");
    titleLabel->setFont(titleFont);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    m_backBtn = new QPushButton("Back");
    m_backBtn->setStyleSheet("color: #1976D2; border: none; font-size: 12px; font-family: Gotham Rounded;");
    headerLayout->addWidget(m_backBtn);
    mainLayout->addLayout(headerLayout);

    // Search bar
    auto *searchLayout = new QHBoxLayout;
    m_searchEdit = new QLineEdit;
    m_searchEdit->setStyleSheet(
        "color: #0F172A; "
        "border: none; "
        "font-size: 12px; "
        "font-family: Gotham Rounded;"
        "padding: 5px;"
        "background-color:#E5E7EB;"
        "border-radius: 8px;"
        );
    m_searchEdit->setPlaceholderText("Enter Bill Number (e.g. INV-20260716-0001)");
    m_searchBtn = new QPushButton("Search");
    m_searchBtn->setFixedHeight(35);
    m_searchBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; border-radius: 4px; padding: 5px 20px; font-family: Gotham Rounded;}"
        "QPushButton:hover { background-color: #1976D2; }"
    );
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchBtn);
    mainLayout->addLayout(searchLayout);

    m_statusLabel = new QLabel;
    m_statusLabel->setStyleSheet("color: rgb(248, 250, 252); font-weight: bold;font-family: Gotham Rounded;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    // Display widget
    m_displayWidget = new QWidget;
    auto *displayLayout = new QVBoxLayout(m_displayWidget);
    m_displayWidget->setStyleSheet("color: #0F172A; font-weight: bold;font-family: Gotham Rounded;border: 0px;");
    displayLayout->setContentsMargins(0, 0, 0, 0);

    auto *billGroup = new QGroupBox("Invoice / Bill");

    billGroup->setStyleSheet(
        "QGroupBox {"
        "    background-color: transparent;"
        "    border: 1px solid #E2E8F0;"
        "    border-radius: 10px;"
        "    margin-top: 10px;"
        "    padding: 10px;"
        "    color: #0F172A;"
        "    font-family: 'Gotham Rounded';"
        "    font-size: 12px;"
        "    font-weight: 400;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 15px;"
        "    padding: 0 8px;"
        "    color: #0F172A;"
        "    background-color: rgb(248, 250, 252);"
        "}"
        );

    auto *billLayout = new QFormLayout(billGroup);

    QFont valFont("Gotham Rounded");
    valFont.setPointSize(11);

    m_billNumberVal = new QLabel;
    m_billNumberVal->setFont(valFont);
    billLayout->addRow("Bill Number:", m_billNumberVal);

    m_customerVal = new QLabel;
    m_customerVal->setFont(valFont);
    billLayout->addRow("Customer:", m_customerVal);

    m_serviceVal = new QLabel;
    m_serviceVal->setFont(valFont);
    billLayout->addRow("Service:", m_serviceVal);

    m_dateVal = new QLabel;
    m_dateVal->setFont(valFont);
    billLayout->addRow("Date:", m_dateVal);

    m_statusVal = new QLabel;
    m_statusVal->setFont(valFont);
    billLayout->addRow("Status:", m_statusVal);

    displayLayout->addWidget(billGroup);

    // Items
    auto *itemsGroup = new QGroupBox("Clothing Items");
    itemsGroup->setStyleSheet(
        "QGroupBox {"
        "    background-color: transparent;"
        "    border: 1px solid #E2E8F0;"
        "    border-radius: 10px;"
        "    margin-top: 10px;"
        "    padding: 10px;"
        "    color: #0F172A;"
        "    font-family: 'Gotham Rounded';"
        "    font-size: 12px;"
        "    font-weight: 400;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 15px;"
        "    padding: 0 8px;"
        "    color: #0F172A;"
        "    background-color: rgb(248, 250, 252);"
        "}"
        );
    auto *itemsLayout = new QVBoxLayout(itemsGroup);
    m_itemsText = new QLabel;
    m_itemsText->setFont(valFont);
    m_itemsText->setWordWrap(true);
    m_itemsText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    itemsLayout->addWidget(m_itemsText);
    displayLayout->addWidget(itemsGroup);

    // Financial summary
    auto *finGroup = new QGroupBox("Financial Summary");
    finGroup->setStyleSheet(
        "QGroupBox {"
        "    background-color: transparent;"
        "    border: 1px solid #E2E8F0;"
        "    border-radius: 10px;"
        "    margin-top: 10px;"
        "    padding: 10px;"
        "    color: #0F172A;"
        "    font-family: 'Gotham Rounded';"
        "    font-size: 12px;"
        "    font-weight: 400;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 15px;"
        "    padding: 0 8px;"
        "    color: #0F172A;"
        "    background-color: rgb(248, 250, 252);"
        "}"
        );
    auto *finLayout = new QFormLayout(finGroup);

    m_weightVal = new QLabel;
    m_weightVal->setFont(valFont);
    finLayout->addRow("Total Weight:", m_weightVal);

    m_subtotalVal = new QLabel;
    m_subtotalVal->setFont(valFont);
    finLayout->addRow("Subtotal:", m_subtotalVal);

    m_discountVal = new QLabel;
    m_discountVal->setFont(valFont);
    finLayout->addRow("Discount:", m_discountVal);

    m_vatVal = new QLabel;
    m_vatVal->setFont(valFont);
    finLayout->addRow("VAT (12%):", m_vatVal);

    QFont totalFont;
    totalFont.setPointSize(14);
    totalFont.setBold(true);
    m_finalVal = new QLabel;
    m_finalVal->setFont(totalFont);
    m_finalVal->setStyleSheet("color: #4CAF50;");
    finLayout->addRow("FINAL PRICE:", m_finalVal);

    displayLayout->addWidget(finGroup);
    displayLayout->addStretch();

    mainLayout->addWidget(m_displayWidget);
    m_displayWidget->hide();

    connect(m_searchBtn, &QPushButton::clicked, this, &BillingView::onSearchClicked);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &BillingView::onSearchClicked);
    connect(m_backBtn, &QPushButton::clicked, this, &BillingView::onBackClicked);
}

void BillingView::activate(const QVariantMap &args) {
    m_comingFromCustomer = args.value("fromCustomer", false).toBool();
    m_statusLabel->clear();
    clearDisplay();

    if (args.contains("billNumber")) {
        m_searchEdit->setText(args["billNumber"].toString());
        loadBill(args["billNumber"].toString());
    } else {
        m_searchEdit->clear();
    }

    if (m_comingFromCustomer) {
        m_backBtn->setText("Back to Search");
    } else {
        m_backBtn->setText("Back");
    }
}

void BillingView::loadBill(const QString &billNumber) {
    QVariantMap order = m_mainWindow->db().getOrderByBillNumber(billNumber.trimmed());
    if (order.isEmpty()) {
        m_statusLabel->setText("No order found with that bill number.");
        QFont errorFont("Gotham Rounded",14,QFont::Light);
        m_statusLabel->setFont(errorFont);
        m_statusLabel->setStyleSheet("color: red; font-weight: bold; font-size: 11px;");
        m_displayWidget->hide();
        return;
    }
    m_statusLabel->clear();
    displayOrder(order);
}

void BillingView::onSearchClicked() {
    QString billNum = m_searchEdit->text().trimmed();
    if (billNum.isEmpty()) {
        m_statusLabel->setText("Please enter a bill number.");
        QFont errorFont("Gotham Rounded",14,QFont::Light);
        m_statusLabel->setFont(errorFont);
        m_statusLabel->setStyleSheet("color: red; font-weight: bold; font-size: 11px;");
        return;
    }
    loadBill(billNum);
}

void BillingView::displayOrder(const QVariantMap &order) {
    m_billNumberVal->setText(order["bill_number"].toString());
    m_customerVal->setText(order["customer_name"].toString());
    m_serviceVal->setText(order["service_name"].toString());
    m_dateVal->setText(order["created_at"].toString());

    QString status = order["status"].toString();
    m_statusVal->setText(status);
    if (status == "Ready to Pick")
        m_statusVal->setStyleSheet("color: #4CAF50; font-weight: bold; font-size: 11px;");
    else if (status == "Washed")
        m_statusVal->setStyleSheet("color: #FF9800; font-weight: bold; font-size: 11px;");
    else
        m_statusVal->setStyleSheet("color: #f44336; font-weight: bold; font-size: 11px;");

    // Items
    QVariantList items = order["items"].toList();
    QStringList itemStrs;
    for (const auto &item : items) {
        QVariantMap im = item.toMap();
        itemStrs << QString("%1 x %2").arg(im["item_name"].toString()).arg(im["quantity"].toInt());
    }
    m_itemsText->setText(itemStrs.isEmpty() ? "No items" : itemStrs.join("\n"));

    // Financials
    double weight = order["total_weight_kg"].toDouble();
    double subtotal = order["subtotal"].toDouble();
    double discount = order["discount"].toDouble();
    double vat = order["vat_amount"].toDouble();
    double finalPrice = order["final_price"].toDouble();

    m_weightVal->setText(QString("%1 kg").arg(weight, 0, 'f', 2));
    m_subtotalVal->setText(QString("NRs%1").arg(subtotal, 0, 'f', 2));
    m_discountVal->setText(QString("- NRs %1").arg(discount, 0, 'f', 2));
    m_vatVal->setText(QString("NRs%1").arg(vat, 0, 'f', 2));
    m_finalVal->setText(QString("NRs%1").arg(finalPrice, 0, 'f', 2));

    m_displayWidget->show();
}

void BillingView::clearDisplay() {
    m_displayWidget->hide();
}

void BillingView::onBackClicked() {
    if (m_comingFromCustomer)
        m_mainWindow->switchToScreen(Screen::CustomerFlow);
    else
        m_mainWindow->switchToScreen(Screen::OwnerMainMenu);
}
