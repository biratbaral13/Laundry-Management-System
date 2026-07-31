#include "customerflow.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>

CustomerFlow::CustomerFlow(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {


    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: #F8F9FC;");

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 25, 30, 25);
    mainLayout->setSpacing(10);

    // Header
    auto *headerLayout = new QHBoxLayout;
    auto *titleLabel = new QLabel("Customer Portal");
    QFont titleFont;
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #1A1A2E;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    m_backBtn = new QPushButton("Back to Role Selection");
    m_backBtn->setStyleSheet("color: #3F6FEB; border: none; font-size: 12px;");
    m_backBtn->setCursor(Qt::PointingHandCursor);
    headerLayout->addWidget(m_backBtn);
    mainLayout->addLayout(headerLayout);

    auto *subtitleLabel = new QLabel("Search for your order by Bill Number or Full Name");
    QFont subFont;
    subFont.setPointSize(11);
    subtitleLabel->setFont(subFont);
    subtitleLabel->setStyleSheet("color: #6B7280;");
    mainLayout->addWidget(subtitleLabel);

    // Search bar
    auto *searchLayout = new QHBoxLayout;
    searchLayout->setSpacing(10);
    m_searchEdit = new QLineEdit;
    m_searchEdit->setPlaceholderText("Enter Bill Number or Full Name...");
    m_searchEdit->setFixedHeight(40);
    m_searchEdit->setStyleSheet(
        "QLineEdit {"
        "  background-color: white;"
        "  color: #1A1A2E;"
        "  border: 1px solid #DDDDDD;"
        "  border-radius: 8px;"
        "  padding: 8px;"
        "}"
        );
    m_searchBtn = new QPushButton("Search");
    m_searchBtn->setFixedHeight(40);
    m_searchBtn->setStyleSheet(
        "QPushButton { background-color: #3F6FEB; color: white; border-radius: 8px; padding: 5px 20px; font-weight: bold; }"
        "QPushButton:hover { background-color: #3660D0; }"
        );
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchBtn);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(searchLayout);

    m_statusLabel = new QLabel;
    m_statusLabel->setStyleSheet("color: #E53935; font-weight: bold;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    // Results table
    m_resultsTable = new QTableWidget;
    m_resultsTable->setColumnCount(7);
    m_resultsTable->setHorizontalHeaderLabels({
        "Bill #", "Customer", "Service", "Weight", "Total", "Status", "Date"
    });
    m_resultsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_resultsTable->verticalHeader()->hide();
    m_resultsTable->setMinimumHeight(200);
    m_resultsTable->setStyleSheet(
        "QTableWidget {"
        "  background-color: white;"
        "  color: #1A1A2E;"
        "  border: 1px solid #E5E7EB;"
        "  border-radius: 8px;"
        "  gridline-color: #EEEEEE;"
        "}"
        "QHeaderView::section {"
        "  background-color: #F1F3F9;"
        "  color: #1A1A2E;"
        "  font-weight: bold;"
        "  border: none;"
        "  padding: 8px;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color: #3F6FEB;"
        "  color: white;"
        "}"
        );
    mainLayout->addWidget(m_resultsTable);

    // Selected order status
    auto *statusLayout = new QHBoxLayout;
    auto *statusInfoLabel = new QLabel("Selected Order Status:");
    QFont siFont;
    siFont.setPointSize(12);
    siFont.setBold(true);
    statusInfoLabel->setFont(siFont);
    statusInfoLabel->setStyleSheet("color: #1A1A2E;");
    m_selectedStatusLabel = new QLabel("No order selected");
    QFont ssFont;
    ssFont.setPointSize(14);
    ssFont.setBold(true);
    m_selectedStatusLabel->setFont(ssFont);
    m_selectedStatusLabel->setStyleSheet("color: #6B7280;");
    statusLayout->addWidget(statusInfoLabel);
    statusLayout->addWidget(m_selectedStatusLabel);
    statusLayout->addStretch();
    mainLayout->addLayout(statusLayout);

    // View Bill button
    m_viewBillBtn = new QPushButton("View Full Bill");
    m_viewBillBtn->setFixedHeight(45);
    m_viewBillBtn->setEnabled(false);
    m_viewBillBtn->setStyleSheet(
        "QPushButton { background-color: #9C27B0; color: white; border-radius: 10px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #7B1FA2; }"
        "QPushButton:disabled { background-color: #D1D5DB; color: #6B7280; }"
        );
    mainLayout->addWidget(m_viewBillBtn);

    mainLayout->addStretch();

    connect(m_searchBtn, &QPushButton::clicked, this, &CustomerFlow::onSearchClicked);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &CustomerFlow::onSearchClicked);
    connect(m_viewBillBtn, &QPushButton::clicked, this, &CustomerFlow::onViewBillClicked);
    connect(m_backBtn, &QPushButton::clicked, this, &CustomerFlow::onBackClicked);

    connect(m_resultsTable, &QTableWidget::currentCellChanged,
            this, [this](int row) {
                if (row >= 0) {
                    QString status = m_resultsTable->item(row, 5)->text();
                    m_selectedStatusLabel->setText(status);
                    if (status == "Ready to Pick")
                        m_selectedStatusLabel->setStyleSheet("color: #4CAF50; font-size: 14px; font-weight: bold;");
                    else if (status == "Washed")
                        m_selectedStatusLabel->setStyleSheet("color: #FF9800; font-size: 14px; font-weight: bold;");
                    else
                        m_selectedStatusLabel->setStyleSheet("color: #f44336; font-size: 14px; font-weight: bold;");
                    m_viewBillBtn->setEnabled(true);
                } else {
                    m_selectedStatusLabel->setText("No order selected");
                    m_selectedStatusLabel->setStyleSheet("color: #6B7280; font-size: 14px;");
                    m_viewBillBtn->setEnabled(false);
                }
            });
}

void CustomerFlow::activate(const QVariantMap &) {
    m_searchEdit->clear();
    m_resultsTable->setRowCount(0);
    m_statusLabel->clear();
    m_selectedStatusLabel->setText("No order selected");
    m_selectedStatusLabel->setStyleSheet("color: #6B7280; font-size: 14px;");
    m_viewBillBtn->setEnabled(false);
}

void CustomerFlow::onSearchClicked() {
    QString query = m_searchEdit->text().trimmed();
    if (query.isEmpty()) {
        m_statusLabel->setText("Please enter a search term.");
        return;
    }

    QVariantList results = m_mainWindow->db().searchOrders(query);
    if (results.isEmpty()) {
        m_statusLabel->setText("No orders found.");
        m_resultsTable->setRowCount(0);
        m_viewBillBtn->setEnabled(false);
        return;
    }

    m_statusLabel->clear();
    displayResults(results);
}

void CustomerFlow::displayResults(const QVariantList &results) {
    m_resultsTable->setRowCount(0);
    m_resultsTable->setRowCount(results.size());

    for (int i = 0; i < results.size(); ++i) {
        const QVariantMap &o = results[i].toMap();
        auto addCell = [&](int col, const QString &text) {
            auto *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            m_resultsTable->setItem(i, col, item);
        };

        addCell(0, o["bill_number"].toString());
        addCell(1, o["customer_name"].toString());
        addCell(2, o["service_name"].toString());
        addCell(3, QString::number(o["total_weight_kg"].toDouble(), 'f', 2) + " kg");
        addCell(4, QString("NRs%1").arg(o["final_price"].toDouble(), 0, 'f', 2));
        addCell(5, o["status"].toString());
        addCell(6, o["created_at"].toString());
    }

    m_resultsTable->resizeColumnsToContents();
}

void CustomerFlow::onViewBillClicked() {
    int row = m_resultsTable->currentRow();
    if (row < 0) return;

    QString billNumber = m_resultsTable->item(row, 0)->text();
    QVariantMap args;
    args["billNumber"] = billNumber;
    args["fromCustomer"] = true;
    m_mainWindow->switchToScreen(Screen::BillingView, args);
}

void CustomerFlow::onBackClicked() {
    m_mainWindow->switchToScreen(Screen::RoleSelection);
}
