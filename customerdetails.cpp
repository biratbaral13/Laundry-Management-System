#include "customerdetails.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>

CustomerDetails::CustomerDetails(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {


    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: #FFFFFF;");

    auto *mainLayout = new QVBoxLayout(this);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLabel = new QLabel("Customer Details");
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #1A1A2E;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    m_backBtn = new QPushButton("Back");
    m_backBtn->setStyleSheet("color: #1976D2; border: none; font-size: 12px;");
    headerLayout->addWidget(m_backBtn);
    mainLayout->addLayout(headerLayout);

    // Filter bar
    auto *filterLayout = new QHBoxLayout;
    auto *filterLabel = new QLabel("Filter by Status:");
    filterLabel->setStyleSheet("color: #1A1A2E;");
    m_filterCombo = new QComboBox;
    m_filterCombo->addItem("All");
    m_filterCombo->addItem("Not Washed");
    m_filterCombo->addItem("Washed");
    m_filterCombo->addItem("Ready to Pick");
    m_filterCombo->setStyleSheet(
        "QComboBox { background-color: white; color: #1A1A2E; border: 1px solid #DDDDDD; border-radius: 6px; padding: 4px 8px; }"
        "QComboBox QAbstractItemView { background-color: white; color: #1A1A2E; selection-background-color: #E3F2FD; }"
        );
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(m_filterCombo);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);

    // Orders table
    m_ordersTable = new QTableWidget;
    m_ordersTable->setColumnCount(8);
    m_ordersTable->setHorizontalHeaderLabels({
        "ID", "Bill #", "Customer", "Service", "Weight (kg)",
        "Total", "Status", "Date"
    });
    m_ordersTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ordersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_ordersTable->verticalHeader()->hide();
    m_ordersTable->setStyleSheet(
        "QTableWidget { background-color: white; color: #1A1A2E; border: 1px solid #DDDDDD; gridline-color: #EEEEEE; }"
        "QTableWidget::item:selected { background-color: #E3F2FD; color: #1A1A2E; }"
        );
    m_ordersTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background-color: #F0F1F5; color: #1A1A2E; padding: 6px; border: none; font-weight: bold; }"
        );
    mainLayout->addWidget(m_ordersTable);

    // Bottom controls
    auto *bottomLayout = new QHBoxLayout;
    m_statusLabel = new QLabel;
    m_statusLabel->setStyleSheet("color: gray;");

    m_advanceBtn = new QPushButton("Advance Status");
    m_advanceBtn->setFixedHeight(35);
    m_advanceBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border-radius: 4px; padding: 5px 20px; }"
        "QPushButton:hover { background-color: #388E3C; }"
        );

    bottomLayout->addWidget(m_statusLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_advanceBtn);
    mainLayout->addLayout(bottomLayout);

    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CustomerDetails::onFilterChanged);
    connect(m_advanceBtn, &QPushButton::clicked, this, &CustomerDetails::onAdvanceStatusClicked);
    connect(m_backBtn, &QPushButton::clicked, this, &CustomerDetails::onBackClicked);
}

void CustomerDetails::activate(const QVariantMap &) {
    m_filterCombo->setCurrentIndex(0);
    loadOrders();
}

void CustomerDetails::loadOrders(const QString &filter) {
    QVariantList orders = m_mainWindow->db().getAllOrders(filter);
    m_ordersTable->setRowCount(0);
    m_ordersTable->setRowCount(orders.size());

    for (int i = 0; i < orders.size(); ++i) {
        const QVariantMap &o = orders[i].toMap();
        auto addCell = [&](int row, int col, const QString &text) {
            auto *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            m_ordersTable->setItem(row, col, item);
        };

        addCell(i, 0, o["id"].toString());
        addCell(i, 1, o["bill_number"].toString());
        addCell(i, 2, o["customer_name"].toString());
        addCell(i, 3, o["service_name"].toString());
        addCell(i, 4, QString::number(o["total_weight_kg"].toDouble(), 'f', 2));
        addCell(i, 5, QString("NRs%1").arg(o["final_price"].toDouble(), 0, 'f', 2));
        addCell(i, 6, o["status"].toString());
        addCell(i, 7, o["created_at"].toString());
    }

    m_statusLabel->setText(QString("%1 order(s) found.").arg(orders.size()));
    m_ordersTable->resizeColumnsToContents();
}

void CustomerDetails::onFilterChanged() {
    QString filter = m_filterCombo->currentText();
    if (filter == "All") filter.clear();
    loadOrders(filter);
}

void CustomerDetails::onAdvanceStatusClicked() {
    int row = m_ordersTable->currentRow();
    if (row < 0) {
        QMessageBox box(this);
        box.setIcon(QMessageBox::Warning);
        box.setWindowTitle("No Selection");
        box.setText("Please select an order to update.");
        box.setAttribute(Qt::WA_StyledBackground, true);
        box.setStyleSheet(
            "QMessageBox { background-color: #FFFFFF; }"
            "QLabel { color: #1A1A2E; }"
            "QPushButton { background-color: #4CAF50; color: white; border-radius: 6px; padding: 6px 16px; }"
            "QPushButton:hover { background-color: #388E3C; }"
            );
        box.exec();
        return;
    }

    int orderId = m_ordersTable->item(row, 0)->text().toInt();
    QString currentStatus = m_ordersTable->item(row, 6)->text();

    if (currentStatus == "Ready to Pick") {
        QMessageBox box(this);
        box.setIcon(QMessageBox::Information);
        box.setWindowTitle("Already Complete");
        box.setText("This order is already Ready to Pick.");
        box.setAttribute(Qt::WA_StyledBackground, true);
        box.setStyleSheet(
            "QMessageBox { background-color: #FFFFFF; }"
            "QLabel { color: #1A1A2E; }"
            "QPushButton { background-color: #4CAF50; color: white; border-radius: 6px; padding: 6px 16px; }"
            "QPushButton:hover { background-color: #388E3C; }"
            );
        box.exec();
        return;
    }

    int currentIdx = 0;
    if (currentStatus == "Washed") currentIdx = 1;
    else if (currentStatus == "Ready to Pick") currentIdx = 2;

    int newIdx = nextStatus(currentIdx);
    QString newStatus = statusName(newIdx);

    QMessageBox confirmBox(this);
    confirmBox.setIcon(QMessageBox::Question);
    confirmBox.setWindowTitle("Update Status");
    confirmBox.setText(QString("Change status from \"%1\" to \"%2\"?").arg(currentStatus, newStatus));
    confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmBox.setAttribute(Qt::WA_StyledBackground, true);
    confirmBox.setStyleSheet(
        "QMessageBox { background-color: #FFFFFF; }"
        "QLabel { color: #1A1A2E; }"
        "QPushButton { background-color: #4CAF50; color: white; border-radius: 6px; padding: 6px 16px; }"
        "QPushButton:hover { background-color: #388E3C; }"
        );
    QMessageBox::StandardButton reply = static_cast<QMessageBox::StandardButton>(confirmBox.exec());

    if (reply == QMessageBox::Yes) {
        if (m_mainWindow->db().updateOrderStatus(orderId, newStatus)) {
            m_statusLabel->setText("Status updated successfully.");
            loadOrders(m_filterCombo->currentText() == "All" ? QString() : m_filterCombo->currentText());
        } else {
            m_statusLabel->setText("Failed to update status.");
        }
    }
}

void CustomerDetails::onBackClicked() {
    m_mainWindow->switchToScreen(Screen::OwnerMainMenu);
}