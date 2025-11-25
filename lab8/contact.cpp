#include "contact.h"

Contact::Contact() {}

Contact::Contact(const QString& last, const QString& first, const QString& patr,
    const QString& addr, const QDate& bdate, const QString& em,
    const QList<QString>& phones)
    : firstName(first), lastName(last), patronymic(patr),
    address(addr), birthDate(bdate), email(em), phoneNumbers(phones) {
}

QJsonObject Contact::toJson() const {
    QJsonObject obj;
    obj["firstName"] = firstName;
    obj["lastName"] = lastName;
    obj["patronymic"] = patronymic;
    obj["address"] = address;
    obj["birthDate"] = birthDate.toString(Qt::ISODate);
    obj["email"] = email;

    QJsonArray phonesArray;
    for (const QString& phone : phoneNumbers)
        phonesArray.append(phone);
    obj["phones"] = phonesArray;

    return obj;
}

Contact Contact::fromJson(const QJsonObject& obj) {
    Contact c;
    c.firstName = obj["firstName"].toString();
    c.lastName = obj["lastName"].toString();
    c.patronymic = obj["patronymic"].toString();
    c.address = obj["address"].toString();
    c.birthDate = QDate::fromString(obj["birthDate"].toString(), Qt::ISODate);
    c.email = obj["email"].toString();

    QJsonArray phonesArray = obj["phones"].toArray();
    for (const QJsonValue& val : phonesArray)
        c.phoneNumbers.append(val.toString());

    return c;
}