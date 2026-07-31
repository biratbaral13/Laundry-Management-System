#ifndef NEWORDER_H
#define NEWORDER_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include "enums.h"

class MainWindow;

class NewOrder : public QWidget {
    Q_OBJECT
public:
    explicit NewOrder(MainWindow *mainWindow, QWidget *parent = nullptr);
    void activate(const QVariantMap &args = QVariantMap());

private slots:
    void onAddItemClicked();
    void onRemoveItemClicked();
    void onRecalculate();
    void onSaveOrderClicked();
    void onBackClicked();

private:
    void setupForm();
    void recalculate();
    void clearForm();

    MainWindow *m_mainWindow;

    QLineEdit *m_customerNameEdit;
    QComboBox *m_serviceCombo;
    QTableWidget *m_itemsTable;
    QDoubleSpinBox *m_weightSpin;
    QLabel *m_subtotalLabel;
    QLineEdit *m_discountEdit;
    QLabel *m_vatLabel;
    QLabel *m_totalLabel;
    QLabel *m_billNumberLabel;
    QLabel *m_statusLabel;
    QPushButton *m_saveBtn;
    QPushButton *m_backBtn;
    QPushButton *m_addItemBtn;
    QPushButton *m_removeItemBtn;

    QList<ServiceType> m_serviceTypes;
};

#endif // NEWORDER_H
