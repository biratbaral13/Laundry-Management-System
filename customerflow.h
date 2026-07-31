#ifndef CUSTOMERFLOW_H
#define CUSTOMERFLOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>

class MainWindow;

class CustomerFlow : public QWidget {
    Q_OBJECT
public:
    explicit CustomerFlow(MainWindow *mainWindow, QWidget *parent = nullptr);
    void activate(const QVariantMap &args = QVariantMap());

private slots:
    void onSearchClicked();
    void onViewBillClicked();
    void onBackClicked();

private:
    void displayResults(const QVariantList &results);

    MainWindow *m_mainWindow;

    QLineEdit *m_searchEdit;
    QPushButton *m_searchBtn;
    QPushButton *m_backBtn;
    QPushButton *m_viewBillBtn;
    QTableWidget *m_resultsTable;
    QLabel *m_statusLabel;
    QLabel *m_selectedStatusLabel;
};

#endif // CUSTOMERFLOW_H
