#ifndef CUSTOMERDETAILS_H
#define CUSTOMERDETAILS_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class MainWindow;

class CustomerDetails : public QWidget {
    Q_OBJECT
public:
    explicit CustomerDetails(MainWindow *mainWindow, QWidget *parent = nullptr);
    void activate(const QVariantMap &args = QVariantMap());

private slots:
    void onFilterChanged();
    void onAdvanceStatusClicked();
    void onBackClicked();

private:
    void loadOrders(const QString &filter = QString());

    MainWindow *m_mainWindow;
    QTableWidget *m_ordersTable;
    QComboBox *m_filterCombo;
    QPushButton *m_advanceBtn;
    QPushButton *m_backBtn;
    QLabel *m_statusLabel;
};

#endif // CUSTOMERDETAILS_H
