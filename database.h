#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>
#include <QVariantMap>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool initialize();
    QSqlDatabase connection() const;

    // Owner
    bool addOwner(const QString &fullName, const QString &loginId,
                  const QString &pin, const QString &secQuestion, const QString &secAnswer);
    QVariantMap getOwner(const QString &loginId);
    bool updateOwnerPin(const QString &loginId, const QString &newPin);
    bool isOwnerLocked(const QString &loginId);
    bool lockOwner(const QString &loginId);

    // Customer
    int getOrCreateCustomer(const QString &fullName);

    // Orders
    QString generateBillNumber();
    bool createOrder(int customerId, int ownerId, int serviceTypeId,
                     double totalWeight, double subtotal, double discount,
                     double vatAmount, double finalPrice, const QString &status,
                     const QList<QPair<QString,int>> &items);
    QVariantList getAllOrders(const QString &statusFilter = QString());
    QVariantMap getOrderById(int orderId);
    QVariantMap getOrderByBillNumber(const QString &billNumber);
    QVariantList searchOrders(const QString &query);
    bool updateOrderStatus(int orderId, const QString &newStatus);
    QVariantList getOrdersByOwner(int ownerId);

    // Service types
    QVariantList getServiceTypes();

private:
    QSqlDatabase m_db;
    void createTables();
    void seedServiceTypes();
};

#endif // DATABASE_H
