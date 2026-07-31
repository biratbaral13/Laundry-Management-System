#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>
#include <QCoreApplication>

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::initialize() {
    QString path = QCoreApplication::applicationDirPath() + "/laundry.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qDebug() << "DB open error:" << m_db.lastError().text();
        return false;
    }

    createTables();
    seedServiceTypes();
    return true;
}

QSqlDatabase DatabaseManager::connection() const {
    return m_db;
}

void DatabaseManager::createTables() {
    QSqlQuery q(m_db);

    q.exec("CREATE TABLE IF NOT EXISTS owner ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "full_name TEXT NOT NULL,"
           "login_id TEXT UNIQUE NOT NULL,"
           "pin TEXT NOT NULL,"
           "security_question TEXT NOT NULL,"
           "security_answer TEXT NOT NULL,"
           "is_locked INTEGER DEFAULT 0)");

    q.exec("CREATE TABLE IF NOT EXISTS service_type ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name TEXT NOT NULL,"
           "rate_per_kg REAL NOT NULL)");

    q.exec("CREATE TABLE IF NOT EXISTS customer ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "full_name TEXT NOT NULL UNIQUE)");

    q.exec("CREATE TABLE IF NOT EXISTS orders ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "bill_number TEXT UNIQUE NOT NULL,"
           "customer_id INTEGER NOT NULL,"
           "owner_id INTEGER NOT NULL,"
           "service_type_id INTEGER NOT NULL,"
           "total_weight_kg REAL NOT NULL,"
           "subtotal REAL NOT NULL,"
           "discount REAL DEFAULT 0,"
           "vat_amount REAL NOT NULL,"
           "final_price REAL NOT NULL,"
           "status TEXT NOT NULL DEFAULT 'Not Washed',"
           "created_at TEXT DEFAULT (datetime('now','localtime')),"
           "FOREIGN KEY (customer_id) REFERENCES customer(id),"
           "FOREIGN KEY (owner_id) REFERENCES owner(id),"
           "FOREIGN KEY (service_type_id) REFERENCES service_type(id))");

    q.exec("CREATE TABLE IF NOT EXISTS order_detail ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "order_id INTEGER NOT NULL,"
           "item_name TEXT NOT NULL,"
           "quantity INTEGER NOT NULL,"
           "FOREIGN KEY (order_id) REFERENCES orders(id) ON DELETE CASCADE)");
}

void DatabaseManager::seedServiceTypes() {
    QSqlQuery q(m_db);
    q.exec("SELECT COUNT(*) FROM service_type");
    if (q.next() && q.value(0).toInt() == 0) {
        q.prepare("INSERT INTO service_type (name, rate_per_kg) VALUES (?, ?)");
        q.addBindValue(QStringLiteral("Wash Only"));
        q.addBindValue(120.00);
        q.exec();

        q.prepare("INSERT INTO service_type (name, rate_per_kg) VALUES (?, ?)");
        q.addBindValue(QStringLiteral("Full Service (wash + dry + iron)"));
        q.addBindValue(240.00);
        q.exec();

        q.prepare("INSERT INTO service_type (name, rate_per_kg) VALUES (?, ?)");
        q.addBindValue(QStringLiteral("Dry Cleaning"));
        q.addBindValue(400.00);
        q.exec();
    }
}

// ---- Owner ----

bool DatabaseManager::addOwner(const QString &fullName, const QString &loginId,
                                const QString &pin, const QString &secQuestion,
                                const QString &secAnswer) {
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO owner (full_name, login_id, pin, security_question, security_answer) "
              "VALUES (?, ?, ?, ?, ?)");
    q.addBindValue(fullName);
    q.addBindValue(loginId);
    q.addBindValue(pin);
    q.addBindValue(secQuestion);
    q.addBindValue(secAnswer.toLower());
    if (!q.exec()) {
        qDebug() << "addOwner error:" << q.lastError().text();
        return false;
    }
    return true;
}

QVariantMap DatabaseManager::getOwner(const QString &loginId) {
    QSqlQuery q(m_db);
    q.prepare("SELECT id, full_name, login_id, pin, security_question, security_answer, is_locked "
              "FROM owner WHERE login_id = ?");
    q.addBindValue(loginId);
    QVariantMap result;
    if (q.exec() && q.next()) {
        result["id"] = q.value("id");
        result["full_name"] = q.value("full_name");
        result["login_id"] = q.value("login_id");
        result["pin"] = q.value("pin");
        result["security_question"] = q.value("security_question");
        result["security_answer"] = q.value("security_answer");
        result["is_locked"] = q.value("is_locked");
    }
    return result;
}

bool DatabaseManager::updateOwnerPin(const QString &loginId, const QString &newPin) {
    QSqlQuery q(m_db);
    q.prepare("UPDATE owner SET pin = ? WHERE login_id = ?");
    q.addBindValue(newPin);
    q.addBindValue(loginId);
    return q.exec();
}

bool DatabaseManager::isOwnerLocked(const QString &loginId) {
    QSqlQuery q(m_db);
    q.prepare("SELECT is_locked FROM owner WHERE login_id = ?");
    q.addBindValue(loginId);
    if (q.exec() && q.next())
        return q.value(0).toBool();
    return false;
}

bool DatabaseManager::lockOwner(const QString &loginId) {
    QSqlQuery q(m_db);
    q.prepare("UPDATE owner SET is_locked = 1 WHERE login_id = ?");
    q.addBindValue(loginId);
    return q.exec();
}

// ---- Customer ----

int DatabaseManager::getOrCreateCustomer(const QString &fullName) {
    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM customer WHERE full_name = ?");
    q.addBindValue(fullName);
    if (q.exec() && q.next())
        return q.value(0).toInt();

    q.prepare("INSERT INTO customer (full_name) VALUES (?)");
    q.addBindValue(fullName);
    if (q.exec())
        return q.lastInsertId().toInt();
    return -1;
}

// ---- Orders ----

QString DatabaseManager::generateBillNumber() {
    QString dateStr = QDate::currentDate().toString("yyyyMMdd");
    QString prefix = "INV-" + dateStr + "-";

    QSqlQuery q(m_db);
    q.prepare("SELECT COUNT(*) FROM orders WHERE bill_number LIKE ?");
    q.addBindValue(prefix + "%");
    if (q.exec() && q.next()) {
        int count = q.value(0).toInt() + 1;
        return prefix + QString::number(count).rightJustified(4, '0');
    }
    return prefix + "0001";
}

bool DatabaseManager::createOrder(int customerId, int ownerId, int serviceTypeId,
                                   double totalWeight, double subtotal, double discount,
                                   double vatAmount, double finalPrice, const QString &status,
                                   const QList<QPair<QString,int>> &items) {
    m_db.transaction();
    QSqlQuery q(m_db);

    QString billNum = generateBillNumber();

    q.prepare("INSERT INTO orders (bill_number, customer_id, owner_id, service_type_id, "
              "total_weight_kg, subtotal, discount, vat_amount, final_price, status) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    q.addBindValue(billNum);
    q.addBindValue(customerId);
    q.addBindValue(ownerId);
    q.addBindValue(serviceTypeId);
    q.addBindValue(totalWeight);
    q.addBindValue(subtotal);
    q.addBindValue(discount);
    q.addBindValue(vatAmount);
    q.addBindValue(finalPrice);
    q.addBindValue(status);

    if (!q.exec()) {
        qDebug() << "createOrder error:" << q.lastError().text();
        m_db.rollback();
        return false;
    }

    int orderId = q.lastInsertId().toInt();

    for (const auto &item : items) {
        q.prepare("INSERT INTO order_detail (order_id, item_name, quantity) VALUES (?, ?, ?)");
        q.addBindValue(orderId);
        q.addBindValue(item.first);
        q.addBindValue(item.second);
        if (!q.exec()) {
            qDebug() << "createOrder detail error:" << q.lastError().text();
            m_db.rollback();
            return false;
        }
    }

    m_db.commit();
    return true;
}

QVariantList DatabaseManager::getAllOrders(const QString &statusFilter) {
    QSqlQuery q(m_db);
    QString sql = "SELECT o.id, o.bill_number, c.full_name AS customer_name, "
                  "s.name AS service_name, o.total_weight_kg, o.subtotal, o.discount, "
                  "o.vat_amount, o.final_price, o.status, o.created_at "
                  "FROM orders o "
                  "JOIN customer c ON o.customer_id = c.id "
                  "JOIN service_type s ON o.service_type_id = s.id";

    if (!statusFilter.isEmpty()) {
        sql += " WHERE o.status = ?";
        q.prepare(sql);
        q.addBindValue(statusFilter);
    } else {
        q.prepare(sql);
    }

    QVariantList results;
    if (q.exec()) {
        while (q.next()) {
            QVariantMap row;
            row["id"] = q.value("id");
            row["bill_number"] = q.value("bill_number");
            row["customer_name"] = q.value("customer_name");
            row["service_name"] = q.value("service_name");
            row["total_weight_kg"] = q.value("total_weight_kg");
            row["subtotal"] = q.value("subtotal");
            row["discount"] = q.value("discount");
            row["vat_amount"] = q.value("vat_amount");
            row["final_price"] = q.value("final_price");
            row["status"] = q.value("status");
            row["created_at"] = q.value("created_at");
            results.append(row);
        }
    }
    return results;
}

QVariantMap DatabaseManager::getOrderById(int orderId) {
    QSqlQuery q(m_db);
    q.prepare("SELECT o.id, o.bill_number, c.full_name AS customer_name, "
              "s.name AS service_name, s.rate_per_kg, o.total_weight_kg, "
              "o.subtotal, o.discount, o.vat_amount, o.final_price, o.status, o.created_at "
              "FROM orders o "
              "JOIN customer c ON o.customer_id = c.id "
              "JOIN service_type s ON o.service_type_id = s.id "
              "WHERE o.id = ?");
    q.addBindValue(orderId);

    QVariantMap result;
    if (q.exec() && q.next()) {
        result["id"] = q.value("id");
        result["bill_number"] = q.value("bill_number");
        result["customer_name"] = q.value("customer_name");
        result["service_name"] = q.value("service_name");
        result["rate_per_kg"] = q.value("rate_per_kg");
        result["total_weight_kg"] = q.value("total_weight_kg");
        result["subtotal"] = q.value("subtotal");
        result["discount"] = q.value("discount");
        result["vat_amount"] = q.value("vat_amount");
        result["final_price"] = q.value("final_price");
        result["status"] = q.value("status");
        result["created_at"] = q.value("created_at");

        QSqlQuery dq(m_db);
        dq.prepare("SELECT item_name, quantity FROM order_detail WHERE order_id = ?");
        dq.addBindValue(orderId);
        QVariantList items;
        if (dq.exec()) {
            while (dq.next()) {
                QVariantMap item;
                item["item_name"] = dq.value("item_name");
                item["quantity"] = dq.value("quantity");
                items.append(item);
            }
        }
        result["items"] = items;
    }
    return result;
}

QVariantMap DatabaseManager::getOrderByBillNumber(const QString &billNumber) {
    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM orders WHERE bill_number = ?");
    q.addBindValue(billNumber);
    if (q.exec() && q.next())
        return getOrderById(q.value(0).toInt());
    return {};
}

QVariantList DatabaseManager::searchOrders(const QString &query) {
    QSqlQuery q(m_db);
    q.prepare("SELECT o.id, o.bill_number, c.full_name AS customer_name, "
              "s.name AS service_name, o.total_weight_kg, o.subtotal, o.discount, "
              "o.vat_amount, o.final_price, o.status, o.created_at "
              "FROM orders o "
              "JOIN customer c ON o.customer_id = c.id "
              "JOIN service_type s ON o.service_type_id = s.id "
              "WHERE o.bill_number LIKE ? OR c.full_name LIKE ?");
    QString pattern = "%" + query + "%";
    q.addBindValue(pattern);
    q.addBindValue(pattern);

    QVariantList results;
    if (q.exec()) {
        while (q.next()) {
            QVariantMap row;
            row["id"] = q.value("id");
            row["bill_number"] = q.value("bill_number");
            row["customer_name"] = q.value("customer_name");
            row["service_name"] = q.value("service_name");
            row["total_weight_kg"] = q.value("total_weight_kg");
            row["subtotal"] = q.value("subtotal");
            row["discount"] = q.value("discount");
            row["vat_amount"] = q.value("vat_amount");
            row["final_price"] = q.value("final_price");
            row["status"] = q.value("status");
            row["created_at"] = q.value("created_at");
            results.append(row);
        }
    }
    return results;
}

bool DatabaseManager::updateOrderStatus(int orderId, const QString &newStatus) {
    QSqlQuery q(m_db);
    q.prepare("UPDATE orders SET status = ? WHERE id = ?");
    q.addBindValue(newStatus);
    q.addBindValue(orderId);
    return q.exec();
}

QVariantList DatabaseManager::getOrdersByOwner(int ownerId) {
    QSqlQuery q(m_db);
    q.prepare("SELECT o.id, o.bill_number, c.full_name AS customer_name, "
              "s.name AS service_name, o.total_weight_kg, o.subtotal, o.discount, "
              "o.vat_amount, o.final_price, o.status, o.created_at "
              "FROM orders o "
              "JOIN customer c ON o.customer_id = c.id "
              "JOIN service_type s ON o.service_type_id = s.id "
              "WHERE o.owner_id = ? "
              "ORDER BY o.id DESC");
    q.addBindValue(ownerId);

    QVariantList results;
    if (q.exec()) {
        while (q.next()) {
            QVariantMap row;
            row["id"] = q.value("id");
            row["bill_number"] = q.value("bill_number");
            row["customer_name"] = q.value("customer_name");
            row["service_name"] = q.value("service_name");
            row["total_weight_kg"] = q.value("total_weight_kg");
            row["subtotal"] = q.value("subtotal");
            row["discount"] = q.value("discount");
            row["vat_amount"] = q.value("vat_amount");
            row["final_price"] = q.value("final_price");
            row["status"] = q.value("status");
            row["created_at"] = q.value("created_at");
            results.append(row);
        }
    }
    return results;
}

QVariantList DatabaseManager::getServiceTypes() {
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, rate_per_kg FROM service_type ORDER BY id");
    QVariantList results;
    if (q.exec()) {
        while (q.next()) {
            QVariantMap row;
            row["id"] = q.value("id");
            row["name"] = q.value("name");
            row["rate_per_kg"] = q.value("rate_per_kg");
            results.append(row);
        }
    }
    return results;
}
