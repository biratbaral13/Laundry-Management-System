#ifndef ENUMS_H
#define ENUMS_H

#include <QString>

enum class Screen {
    RoleSelection,
    OwnerSignUp,
    OwnerLogin,
    ForgotPassword,
    OwnerMainMenu,
    NewOrder,
    CustomerDetails,
    BillingView,
    CustomerFlow
};

enum class ServiceType {
    WashOnly = 1,
    FullService = 2,
    DryCleaning = 3
};

inline double serviceRate(ServiceType type) {
    switch (type) {
        case ServiceType::WashOnly:    return 120;
        case ServiceType::FullService: return 240;
        case ServiceType::DryCleaning: return 400;
    }
    return 0.0;
}

inline QString serviceName(ServiceType type) {
    switch (type) {
        case ServiceType::WashOnly:    return QStringLiteral("Wash Only");
        case ServiceType::FullService: return QStringLiteral("Full Service (wash + dry + iron)");
        case ServiceType::DryCleaning: return QStringLiteral("Dry Cleaning");
    }
    return {};
}

inline QString statusName(int statusIndex) {
    switch (statusIndex) {
        case 0: return QStringLiteral("Not Washed");
        case 1: return QStringLiteral("Washed");
        case 2: return QStringLiteral("Ready to Pick");
    }
    return QStringLiteral("Not Washed");
}

inline int nextStatus(int current) {
    if (current < 2) return current + 1;
    return 2;
}

#endif // ENUMS_H
