#pragma once
#include "BaseRepository.h"

class ConsumedRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit ConsumedRepository(QObject *parent = nullptr);

    // Специфичные методы
    int createConsumedEntry(int userId, const QString &date, const QString &productName,
                            double quantity, const QString &unit,
                            double proteins, double fats, double carbs,
                            double calories, const QString &timestamp);

    QVariantList getConsumedEntriesByUser(int userId, const QString &date);
};
