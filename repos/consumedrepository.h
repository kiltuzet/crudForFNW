#pragma once
#include "BaseRepository.h"

class ConsumedRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit ConsumedRepository(QObject *parent = nullptr);

    // Специфичные методы
    int createConsumedEntry(QVariantMap values);

    QVariantList getConsumedEntriesByUser(int userId, const QString &date);
};
