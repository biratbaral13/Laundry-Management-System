#include "mainwindow.h"
#include "rolescreen.h"
#include "ownersignup.h"
#include "ownerlogin.h"
#include "forgotpassword.h"
#include "ownermainmenu.h"
#include "neworder.h"
#include "customerdetails.h"
#include "billingview.h"
#include "customerflow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_db.initialize();
    setupUi();
    createScreens();
    switchToScreen(Screen::RoleSelection);
}

void MainWindow::setupUi() {
    setWindowTitle("LaundryF - Laundry Management System");
    setMinimumSize(800, 600);
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);
}

void MainWindow::createScreens() {
    auto addScreen = [&](QWidget *w) -> int {
        int idx = m_nextIndex++;
        m_stack->addWidget(w);
        return idx;
    };

    m_roleSelection   = new RoleSelection(this);
    m_indices[Screen::RoleSelection] = addScreen(m_roleSelection);

    m_ownerSignUp     = new OwnerSignUp(this);
    m_indices[Screen::OwnerSignUp] = addScreen(m_ownerSignUp);

    m_ownerLogin      = new OwnerLogin(this);
    m_indices[Screen::OwnerLogin] = addScreen(m_ownerLogin);

    m_forgotPassword  = new ForgotPassword(this);
    m_indices[Screen::ForgotPassword] = addScreen(m_forgotPassword);

    m_ownerMainMenu   = new OwnerMainMenu(this);
    m_indices[Screen::OwnerMainMenu] = addScreen(m_ownerMainMenu);

    m_newOrder        = new NewOrder(this);
    m_indices[Screen::NewOrder] = addScreen(m_newOrder);

    m_customerDetails = new CustomerDetails(this);
    m_indices[Screen::CustomerDetails] = addScreen(m_customerDetails);

    m_billingView     = new BillingView(this);
    m_indices[Screen::BillingView] = addScreen(m_billingView);

    m_customerFlow    = new CustomerFlow(this);
    m_indices[Screen::CustomerFlow] = addScreen(m_customerFlow);
}

void MainWindow::switchToScreen(Screen screen, const QVariantMap &args) {
    if (!m_indices.contains(screen)) return;
    m_stack->setCurrentIndex(m_indices[screen]);

    switch (screen) {
        case Screen::OwnerLogin:    m_ownerLogin->activate(args); break;
        case Screen::NewOrder:      m_newOrder->activate(args); break;
        case Screen::CustomerDetails: m_customerDetails->activate(args); break;
        case Screen::BillingView:   m_billingView->activate(args); break;
        case Screen::CustomerFlow:  m_customerFlow->activate(args); break;
        case Screen::OwnerMainMenu: m_ownerMainMenu->activate(args); break;
        case Screen::ForgotPassword: m_forgotPassword->activate(args); break;
        default: break;
    }
}

DatabaseManager& MainWindow::db() { return m_db; }

void MainWindow::setCurrentOwner(int ownerId, const QString &ownerName) {
    m_currentOwnerId = ownerId;
    m_currentOwnerName = ownerName;
}

int MainWindow::currentOwnerId() const { return m_currentOwnerId; }
QString MainWindow::currentOwnerName() const { return m_currentOwnerName; }

void MainWindow::clearSession() {
    m_currentOwnerId = -1;
    m_currentOwnerName.clear();
}
