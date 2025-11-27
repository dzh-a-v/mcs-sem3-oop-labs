#pragma once

#include <QString>
#include <QList>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>

struct PhoneEntry
{
    QString number;
    QString type;

    QString display() const {
        return type + ": " + number;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["number"] = number;
        obj["type"] = type;
        return obj;
    }

    static PhoneEntry fromJson(const QJsonObject& obj) {
        PhoneEntry p;
        p.number = obj["number"].toString();
        p.type = obj["type"].toString();
        if (p.type != "рабочий" && p.type != "домашний" && p.type != "служебный") {
            p.type = "рабочий";
        }
        return p;
    }
};

class Contact
{
public:
    QString firstName;
    QString lastName;
    QString patronymic;
    QString address;
    QDate birthDate;
    QString email;
    QList<PhoneEntry> phoneNumbers;

    Contact();
    Contact(const QString& last, const QString& first, const QString& patr,
        const QString& addr, const QDate& bdate, const QString& em,
        const QList<PhoneEntry>& phones);

    QJsonObject toJson() const;
    static Contact fromJson(const QJsonObject& obj);
};



/*
[
    {
        "address": "Italy",
        "birthDate": "2004-01-05",
        "email": "leonrado@da.vinci",
        "firstName": "Da Vinci",
        "lastName": "Leonardo",
        "patronymic": "The-2nd",
        "phones": [
            {
                "number": "+88005553535",
                "type": "служебный"
            }
        ]
    },
    {
        "address": "ул. Антона",
        "birthDate": "2005-11-26",
        "email": "antonov@anton.com",
        "firstName": "Антон",
        "lastName": "Антонов",
        "patronymic": "Антонович",
        "phones": [
            {
                "number": "+912353235632",
                "type": "рабочий"
            }
        ]
    },
    {
        "address": "СПб, ул. Харченко, 16",
        "birthDate": "2005-11-26",
        "email": "lurtced@yandex.ru",
        "firstName": "Арсен",
        "lastName": "Джабраилов",
        "patronymic": "Валерьевич",
        "phones": [
            {
                "number": "+79992421698",
                "type": "рабочий"
            },
            {
                "number": "89894540656",
                "type": "домашний"
            }
        ]
    },
    {
        "address": "ул Васи",
        "birthDate": "2005-11-27",
        "email": "vasya@vasya.ri",
        "firstName": "Вася",
        "lastName": "Васильев",
        "patronymic": "Васильевич",
        "phones": [
            {
                "number": "+3584579873429",
                "type": "рабочий"
            }
        ]
    }
]

*/