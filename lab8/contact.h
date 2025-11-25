#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QList>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>

class Contact
{
public:
    QString firstName;
    QString lastName;
    QString patronymic;
    QString address;
    QDate birthDate;
    QString email;
    QList<QString> phoneNumbers;

    Contact();
    Contact(const QString& last, const QString& first, const QString& patr,
        const QString& addr, const QDate& bdate, const QString& em,
        const QList<QString>& phones);

    QJsonObject toJson() const;
    static Contact fromJson(const QJsonObject& obj);
};

#endif // CONTACT_H