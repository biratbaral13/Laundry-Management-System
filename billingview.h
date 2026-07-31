#ifndef BILLINGVIEW_H
#define BILLINGVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class MainWindow;

class BillingView : public QWidget {
    Q_OBJECT
public:
    explicit BillingView(MainWindow *mainWindow, QWidget *parent = nullptr);
    void activate(const QVariantMap &args = QVariantMap());
    void loadBill(const QString &billNumber);

private slots:
    void onSearchClicked();
    void onBackClicked();

private:
    void displayOrder(const QVariantMap &order);
    void clearDisplay();

    MainWindow *m_mainWindow;

    QLineEdit *m_searchEdit;
    QPushButton *m_searchBtn;
    QPushButton *m_backBtn;
    QLabel *m_statusLabel;

    // Display labels
    QLabel *m_billNumberVal;
    QLabel *m_customerVal;
    QLabel *m_serviceVal;
    QLabel *m_dateVal;
    QLabel *m_statusVal;
    QLabel *m_itemsText;
    QLabel *m_weightVal;
    QLabel *m_subtotalVal;
    QLabel *m_discountVal;
    QLabel *m_vatVal;
    QLabel *m_finalVal;

    QWidget *m_displayWidget;
    bool m_comingFromCustomer;
};

#endif // BILLINGVIEW_H
