#include "neworder.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>
#include <QSpinBox>
#include <QStyleFactory>

NewOrder::NewOrder(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {

    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(248, 250, 252));
    setPalette(pal);

    auto *mainLayout = new QVBoxLayout(this);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLabel = new QLabel("New Order");

    QString titleStyle =
        "QLabel {"
        "color: #0F172A;"
        "font-size: 32px;"
        "font-weight: semi-bold;"
        "font-family: Gotham Rounded;"
        "}";
    titleLabel->setStyleSheet(titleStyle);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    m_backBtn = new QPushButton("Back");
    m_backBtn->setStyleSheet("color: #1976D2; border: none; font-size: 12px; font-family: Gotham Rounded");
    headerLayout->addWidget(m_backBtn);
    mainLayout->addLayout(headerLayout);

    // Bill number
    m_billNumberLabel = new QLabel;
    QFont billFont("Gotham Rounded", 32, QFont::Bold);
    billFont.setPointSize(12);
    billFont.setBold(true);
    m_billNumberLabel->setFont(billFont);
    m_billNumberLabel->setStyleSheet("color: #D1D5DB;");
    mainLayout->addWidget(m_billNumberLabel);


    QString placeholderStyle=
        "QLineEdit {"
        "    color: black;"
        "    border-bottom: 0px solid #2563EB;"
        "    border-top: 0px;"
        "    border-left: 0px;"
        "    border-right: 0px;"
        "    border-radius: 8px;"
        "    background-color: white;"
        "    padding: 8px;"
        "    font-size: 16px;"
        "}"
        "QLineEdit::placeholder {"
        "    color: black;"
        "    font-style: italic;"
        "}"
        "QLineEdit::selection {"
        "    background-color: white;"
        "    border-bottom: 0px solid #2563EB;"
        "    border-top: 0px;"
        "    border-left: 0px;"
        "    border-right: 0px;"
        "    color: black;"
        "}";



    auto *topForm = new QFormLayout;
    m_customerNameEdit = new QLineEdit;
    m_customerNameEdit->setPlaceholderText("Enter customer full name");
    m_customerNameEdit->setStyleSheet(placeholderStyle);

    QLabel *customerLable = new QLabel("Customer Name");

    QString labelStyle =
        "QLabel {"
        "color: Black;"
        "font-size: 15px;"
        "font-weight: semi-bold;"
        "font-family: Gotham Rounded;"
        "}";
    customerLable->setStyleSheet(labelStyle);

    topForm->addRow(customerLable, m_customerNameEdit);

    m_serviceCombo = new QComboBox;//DropDown Box
    m_serviceCombo->setStyleSheet(
        "QComboBox {"
        "    font-family:Gotham Rounded;"
        "    background-color: white;"
        "    color: #0F172A;"
        "    border-bottom: 2px solid #2563EB;"
        "    border-top: 0px;"
        "    border-right: 0px;"
        "    border-left: 0px;"
        "    border-radius: 8px;"
        "    padding: 8px;"
        "}"

        "QComboBox QAbstractItemView {"
        "    background-color: white;"
        "    color: #0F172A;"
        "    border-bottom: 2px solid #2563EB;"
        "    border-top: 0px;"
        "    border-right: 0px;"
        "    border-left: 0px;"
        "    selection-background-color: #2563EB;"
        "    selection-color: white;"
        "    outline: 0px;"
        "}"

        "QComboBox::focus {"
        "    background-color: white;"
        "    color: #0F172A;"
        "    border-bottom: 2px solid #2563EB;"
        "    border-top: 0px;"
        "    border-right: 0px;"
        "    border-left: 0px;"
        "}"

        );


    QLabel *serviceLable = new QLabel("Service Type");

    serviceLable->setStyleSheet(labelStyle);
    topForm->addRow(serviceLable, m_serviceCombo);
    mainLayout->addLayout(topForm);





    auto *itemsGroup = new QGroupBox("Clothing Items");
    itemsGroup->setStyleSheet(
        "QGroupBox {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #0F172A;"
        "    border-radius: 10px;"
        "    margin-top: 12px;"
        "    padding: 15px;"
        "    font-size: 14px;"
        "    font-weight: Light;"
        "    color: #1E293B;"
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

    m_itemsTable = new QTableWidget(0, 2);
    m_itemsTable->setHorizontalHeaderLabels({"Item Name", "Quantity"});
    m_itemsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_itemsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_itemsTable->setMinimumHeight(150);
    m_itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_itemsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    itemsLayout->addWidget(m_itemsTable);

    m_itemsTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #D1D5DB;"
        "    color: #0F172A;"
        "    font-size: 12px;"
        "    font-weight: 600;"
        "    border: none;"
        "}"
        );

    m_itemsTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #F2F2F2;"
        "    color: #0F172A;"
        "    border: 0px solid #CBD5E1;"
        "    border-radius: 8px;"
        "}"

        "QTableWidget::item {"
        "    padding: 6px;"
        "}"

        "QTableWidget::item:selected {"
        "    background-color: #E0E0E0;"
        "    color: #0F172A;"
        "}"
        );

    auto *itemBtnLayout = new QHBoxLayout;
    m_addItemBtn = new QPushButton("+ Add Item");
    m_addItemBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 4px; font-family: Gotham Rounded;}"
        );
    m_removeItemBtn = new QPushButton("- Remove Selected");
    m_removeItemBtn->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; padding: 5px 15px; border-radius: 4px; font-family: Gotham Rounded;}"
        );
    itemBtnLayout->addWidget(m_addItemBtn);//add item
    itemBtnLayout->addWidget(m_removeItemBtn);//remove item
    itemBtnLayout->addStretch();
    itemsLayout->addLayout(itemBtnLayout);
    mainLayout->addWidget(itemsGroup);

    // Weight
    auto *weightLayout = new QHBoxLayout;
    weightLayout->setAlignment(Qt::AlignLeft);
    auto *weightLabel = new QLabel("Total Weight (kg):");
    weightLabel->setStyleSheet(
        "QLabel {"
        "    color: #0F172A;"
        "    font-family: 'Gotham Rounded';"
        "    font-size: 14px;"
        "    font-weight: Light;"
        "}"
        );
    QFont wlFont("Gotham Rounded", 12, QFont::Bold);
    wlFont.setPointSize(11);
    weightLabel->setFont(wlFont);
    m_weightSpin = new QDoubleSpinBox;
    m_weightSpin->setRange(0.01, 9999.0);
    m_weightSpin->setDecimals(2);
    m_weightSpin->setValue(0.0);
    m_weightSpin->setFixedWidth(150);
    m_weightSpin->setStyleSheet(R"(
    QDoubleSpinBox {
        background-color: transparent;
        border-bottom: 1px solid #0F172A;
        color: black;
        font-size: 14px;
        padding-right: 14px;
    }
    QDoubleSpinBox:hover {
        border-bottom: 1px solid #0F172A;
    }
    QDoubleSpinBox:focus {
        border-bottom: 1px solid #2563EB;
        color: black;
    }
    QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
        subcontrol-origin: border;
        width: 14px;
        border: none;
        background-color: transparent;
    }
    QDoubleSpinBox::up-button { subcontrol-position: top right; }
    QDoubleSpinBox::down-button { subcontrol-position: bottom right; }
    QDoubleSpinBox::up-arrow {
        width: 0; height: 0;
        border-left: 4px solid transparent;
        border-right: 4px solid transparent;
        border-bottom: 5px solid #64748B;
    }
    QDoubleSpinBox::down-arrow {
        width: 0; height: 0;
        border-left: 4px solid transparent;
        border-right: 4px solid transparent;
        border-top: 5px solid #64748B;
    }
    QDoubleSpinBox::up-arrow:hover, QDoubleSpinBox::down-arrow:hover {
        border-bottom-color: #0F172A;
        border-top-color: #0F172A;
    }
    )");

    weightLayout->addWidget(weightLabel);
    weightLayout->addWidget(m_weightSpin);
    weightLayout->addStretch();
    mainLayout->addLayout(weightLayout);

    // Billing summary
    auto *billGroup = new QGroupBox("Billing Summary");
    auto *billLayout = new QFormLayout(billGroup);

    billGroup->setStyleSheet(
        "QGroupBox {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #0F172A;"
        "    border-radius: 10px;"
        "    margin-top: 12px;"
        "    font-size: 14px;"
        "    font-weight: Light;"
        "    color: #1E293B;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 15px;"
        "    padding: 0 8px;"
        "    color: #0F172A;"
        "    background-color: rgb(248, 250, 252);"
        "}"
        );

    QString billinglabelStyle =
        "QLabel {"
        "    color: #1E293B;"
        "    font-size: 12px;"
        "    font-weight: Light;"
        "    padding: 5px;"
        "    font-family: Gotham Rounded;"
        "}";
    m_subtotalLabel = new QLabel("NRs0.00");
    m_subtotalLabel->setStyleSheet(
        "QLabel {"
        "    color: #1E293B;"
        "    font-size: 12px;"
        "    font-weight: Light;"
        "    padding: 5px;"
        "}"
        );

    auto *subtotalText = new QLabel("Subtotal:");
    subtotalText->setStyleSheet(billinglabelStyle);
    billLayout->addRow(subtotalText, m_subtotalLabel);

    auto *discountLayout = new QHBoxLayout;
    m_discountEdit = new QLineEdit("0.00");
    m_discountEdit->setStyleSheet(
        "QLineEdit {"
        "    background-color: transparent;"
        "    color: #0F172A;"
        "    border: 1px solid #0F172A05;"
        "    border-radius: 8px;"
        "    padding: 5px 12px;"
        "    font-size: 12px;"
        "    font-weight: Light;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #0F172A;"
        "}"
        );
    m_discountEdit->setFixedWidth(120);

    auto *dollarLabel = new QLabel("NRs");
    dollarLabel->setStyleSheet(billinglabelStyle);
    discountLayout->addWidget(dollarLabel);
    discountLayout->addWidget(m_discountEdit);
    discountLayout->addStretch();

    auto *discountText = new QLabel("Discount:");
    discountText->setStyleSheet(billinglabelStyle);
    billLayout->addRow(discountText, discountLayout);

    m_vatLabel = new QLabel("NRs0.00");
    m_vatLabel->setStyleSheet(billinglabelStyle);

    auto *vatText = new QLabel("VAT (12%):");
    vatText->setStyleSheet(billinglabelStyle);
    billLayout->addRow(vatText, m_vatLabel);

    QFont totalFont("Gotham Rounded", 14, QFont::Bold);
    totalFont.setPointSize(14);
    totalFont.setBold(true);
    m_totalLabel = new QLabel("NRs0.00");
    m_totalLabel->setStyleSheet(billinglabelStyle);
    m_totalLabel->setFont(totalFont);
    m_totalLabel->setStyleSheet("color: #4CAF50;");
    auto *finalpriceText = new QLabel("Final Price:");
    finalpriceText->setStyleSheet(billinglabelStyle);
    billLayout->addRow(finalpriceText, m_totalLabel);

    mainLayout->addWidget(billGroup);

    // Status + Save
    m_statusLabel = new QLabel;
    m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    m_statusLabel->setAlignment(Qt::AlignCenter);

    m_saveBtn = new QPushButton("Save Order");
    m_saveBtn->setFixedHeight(45);
    QFont saveFont("Gotham Rounded", 32, QFont::Bold);
    saveFont.setPointSize(14);
    saveFont.setBold(true);
    m_saveBtn->setFont(saveFont);
    m_saveBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border-radius: 8px; }"
        "QPushButton:hover { background-color: #388E3C; }"
        );

    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addWidget(m_saveBtn);

    connect(m_addItemBtn, &QPushButton::clicked, this, &NewOrder::onAddItemClicked);
    connect(m_removeItemBtn, &QPushButton::clicked, this, &NewOrder::onRemoveItemClicked);
    connect(m_serviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewOrder::onRecalculate);
    connect(m_weightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &NewOrder::onRecalculate);
    connect(m_discountEdit, &QLineEdit::editingFinished, this, &NewOrder::onRecalculate);
    connect(m_saveBtn, &QPushButton::clicked, this, &NewOrder::onSaveOrderClicked);
    connect(m_backBtn, &QPushButton::clicked, this, &NewOrder::onBackClicked);

    setupForm();


}

void NewOrder::setupForm() {
    m_serviceCombo->clear();
    m_serviceTypes.clear();

    QVariantList services = m_mainWindow->db().getServiceTypes();
    for (const auto &s : services) {
        QVariantMap sm = s.toMap();
        m_serviceCombo->addItem(sm["name"].toString());
        if (sm["id"].toInt() == 1) m_serviceTypes.append(ServiceType::WashOnly);
        else if (sm["id"].toInt() == 2) m_serviceTypes.append(ServiceType::FullService);
        else m_serviceTypes.append(ServiceType::DryCleaning);
    }
}

void NewOrder::activate(const QVariantMap &) {
    clearForm();
    m_billNumberLabel->setText("Bill #: " + m_mainWindow->db().generateBillNumber());
    onAddItemClicked();
}

void NewOrder::clearForm() {
    m_customerNameEdit->clear();
    if (m_serviceCombo->count() > 0) m_serviceCombo->setCurrentIndex(0);
    m_itemsTable->setRowCount(0);
    m_weightSpin->setValue(0.0);
    m_discountEdit->setText("0.00");
    m_statusLabel->clear();
    recalculate();
}

void NewOrder::onAddItemClicked() {
    int row = m_itemsTable->rowCount();
    m_itemsTable->insertRow(row);

    auto *nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("e.g. T-shirt");
    nameEdit->setStyleSheet(
        "QLineEdit {"
        "    background-color: transparent;"
        "    color: #0F172A;"
        "    border: 0px;"
        "    font-size: 14px;"
        "    height: 18px;"
        "}"
        "QLineEdit:focus {"
        "    border: 0px;"
        "    background-color: transparent;"
        "}"
        "QLineEdit::placeholder {"
        "    color: #94A3B8;"
        "}"
        );
    m_itemsTable->setCellWidget(row, 0, nameEdit);

    auto *qtySpin = new QSpinBox;
    qtySpin->setRange(1, 999);
    qtySpin->setValue(1);

    qtySpin->setStyleSheet(R"(
    QSpinBox {
        background-color: transparent;
        border: 0px;
        color: black;
        font-size: 14px;
        padding-right: 14px;
    }
    QSpinBox:hover {
        border: 0px;
    }
    QSpinBox:focus {
        border: 0px;
        color: black;
    }
    QSpinBox::up-button, QSpinBox::down-button {
        subcontrol-origin: border;
        width: 14px;
        border: none;
        background-color: transparent;
    }
    QSpinBox::up-button { subcontrol-position: top right; }
    QSpinBox::down-button { subcontrol-position: bottom right; }
    QSpinBox::up-arrow {
        width: 0; height: 0;
        border-left: 4px solid transparent;
        border-right: 4px solid transparent;
        border-bottom: 5px solid #64748B;
    }
    QSpinBox::down-arrow {
        width: 0; height: 0;
        border-left: 4px solid transparent;
        border-right: 4px solid transparent;
        border-top: 5px solid #64748B;
    }
    QSpinBox::up-arrow:hover, QSpinBox::down-arrow:hover {
        border-bottom-color: #0F172A;
        border-top-color: #0F172A;
    }
    )");

    m_itemsTable->setCellWidget(row, 1, qtySpin);
}

void NewOrder::onRemoveItemClicked() {
    int row = m_itemsTable->currentRow();
    if (row >= 0)
        m_itemsTable->removeRow(row);
}

void NewOrder::onRecalculate() {
    recalculate();
}

void NewOrder::recalculate() {
    int svcIndex = m_serviceCombo->currentIndex();
    if (svcIndex < 0 || svcIndex >= m_serviceTypes.size()) {
        m_subtotalLabel->setText("NRs0.00");
        m_vatLabel->setText("NRs0.00");
        m_totalLabel->setText("NRs0.00");
        return;
    }

    double rate = serviceRate(m_serviceTypes[svcIndex]);
    double weight = m_weightSpin->value();
    double subtotal = rate * weight;

    bool ok;
    double discount = m_discountEdit->text().toDouble(&ok);
    if (!ok || discount < 0) discount = 0.0;
    if (discount > subtotal) discount = subtotal;

    double vatAmount = subtotal * 0.12;
    double finalPrice = subtotal + vatAmount - discount;

    m_subtotalLabel->setText(QString("NRs%1").arg(subtotal, 0, 'f', 2));
    m_vatLabel->setText(QString("NRs%1").arg(vatAmount, 0, 'f', 2));
    m_totalLabel->setText(QString("NRs%1").arg(finalPrice, 0, 'f', 2));
}

void NewOrder::onSaveOrderClicked() {
    QString customerName = m_customerNameEdit->text().trimmed();
    QFont errorFont("Gotham Rounded",14,QFont::Light);
    m_statusLabel->setFont(errorFont);
    if (customerName.isEmpty()) {
        m_statusLabel->setText("Customer name is required.");
        return;
    }

    if (m_itemsTable->rowCount() == 0) {
        m_statusLabel->setText("Add at least one clothing item.");
        return;
    }

    double weight = m_weightSpin->value();
    if (weight <= 0) {
        m_statusLabel->setText("Weight must be greater than 0.");
        return;
    }

    // Collect items
    QList<QPair<QString,int>> items;
    for (int i = 0; i < m_itemsTable->rowCount(); ++i) {
        auto *nameW = qobject_cast<QLineEdit*>(m_itemsTable->cellWidget(i, 0));
        auto *qtyW = qobject_cast<QSpinBox*>(m_itemsTable->cellWidget(i, 1));
        if (!nameW || !qtyW) continue;
        QString name = nameW->text().trimmed();
        int qty = qtyW->value();
        if (name.isEmpty()) {
            m_statusLabel->setText(QString("Item name required in row %1.").arg(i + 1));
            return;
        }
        items.append({name, qty});
    }

    int svcIndex = m_serviceCombo->currentIndex();
    if (svcIndex < 0 || svcIndex >= m_serviceTypes.size()) return;

    int serviceTypeId = static_cast<int>(m_serviceTypes[svcIndex]);
    double rate = serviceRate(m_serviceTypes[svcIndex]);
    double subtotal = rate * weight;

    bool ok;
    double discount = m_discountEdit->text().toDouble(&ok);
    if (!ok || discount < 0) discount = 0.0;
    if (discount > subtotal) discount = subtotal;

    double vatAmount = subtotal * 0.12;
    double finalPrice = subtotal + vatAmount - discount;

    int customerId = m_mainWindow->db().getOrCreateCustomer(customerName);
    if (customerId < 0) {
        m_statusLabel->setText("Failed to create/find customer.");
        return;
    }

    bool success = m_mainWindow->db().createOrder(
        customerId, m_mainWindow->currentOwnerId(), serviceTypeId,
        weight, subtotal, discount, vatAmount, finalPrice,
        "Not Washed", items);

    if (success) {
        QMessageBox::information(this, "Success", "ORDER ADDED SUCCESSFULLY");
        m_mainWindow->switchToScreen(Screen::OwnerMainMenu);
    } else {
        m_statusLabel->setText("Failed to save order.");
    }
}

void NewOrder::onBackClicked() {
    m_mainWindow->switchToScreen(Screen::OwnerMainMenu);
}