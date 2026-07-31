#ifndef ROLESCREEN_H
#define ROLESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class MainWindow;

class RoleSelection : public QWidget {
    Q_OBJECT
public:
    explicit RoleSelection(MainWindow *mainWindow, QWidget *parent = nullptr);

private slots:
    void onOwnerClicked();
    void onCustomerClicked();

private:
    MainWindow *m_mainWindow;
    QPushButton *m_ownerBtn;
    QPushButton *m_customerBtn;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
};

#endif // ROLESCREEN_H
