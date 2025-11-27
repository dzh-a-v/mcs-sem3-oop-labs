#include "mainwindow.h"
#include "contact.h"

#include <QApplication>
#include <QHeaderView>
#include <QInputDialog>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <algorithm>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , currentSortColumn(-1)
    , currentSortOrder(Qt::AscendingOrder)
{
    setWindowTitle("Телефонный справочник");
    resize(900, 600);

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* vLayout = new QVBoxLayout(central);

    // Search box
    searchBox = new QLineEdit;
    searchBox->setPlaceholderText("Поиск по всем полям...");
    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    vLayout->addWidget(searchBox);

    // Table
    table = new QTableWidget(0, 7);
    table->setHorizontalHeaderLabels({
        "Фамилия", "Имя", "Отчество", "Адрес", "Дата рождения", "Email", "Телефоны"
        });
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSortingEnabled(false);
    table->setWordWrap(true);
    table->resizeRowsToContents();
    //table->verticalHeader()->setDefaultSectionSize(30);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSortIndicatorShown(false);

    connect(table->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderClicked);

    vLayout->addWidget(table);

    // Buttons
    auto* hLayout = new QHBoxLayout;
    auto* btnAdd = new QPushButton("Добавить");
    auto* btnEdit = new QPushButton("Редактировать");
    auto* btnDelete = new QPushButton("Удалить");
    auto* btnSave = new QPushButton("Сохранить");
    auto* btnLoad = new QPushButton("Загрузить");

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddContact);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditContact);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteContact);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveToFile);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadFromFile);

    hLayout->addWidget(btnAdd);
    hLayout->addWidget(btnEdit);
    hLayout->addWidget(btnDelete);
    hLayout->addStretch();
    hLayout->addWidget(btnSave);
    hLayout->addWidget(btnLoad);
    vLayout->addLayout(hLayout);

    // Load default file
    loadContactsFromJson(currentFilePath);
    updateTable();
    table->resizeRowsToContents();
}

MainWindow::~MainWindow() = default;

void MainWindow::updateTable()
{
    table->setRowCount(0);
    table->setRowCount(contacts.size());
    for (int i = 0; i < contacts.size(); ++i) {
        const auto& c = contacts[i];
        table->setItem(i, 0, new QTableWidgetItem(c.lastName));
        table->setItem(i, 1, new QTableWidgetItem(c.firstName));
        table->setItem(i, 2, new QTableWidgetItem(c.patronymic));
        table->setItem(i, 3, new QTableWidgetItem(c.address));
        table->setItem(i, 4, new QTableWidgetItem(c.birthDate.toString("dd.MM.yyyy")));
        table->setItem(i, 5, new QTableWidgetItem(c.email));

        QStringList displays;
        for (const auto& phone : c.phoneNumbers) {
            displays << phone.display();
        }
        table->setItem(i, 6, new QTableWidgetItem(displays.join("\n")));
        table->item(i, 0)->setData(Qt::UserRole, i); // UPD: for deleting
    }

    table->resizeRowsToContents();
}

void MainWindow::onHeaderClicked(int logicalIndex)
{
    Qt::SortOrder newOrder;
    if (logicalIndex == currentSortColumn) {
        newOrder = (currentSortOrder == Qt::AscendingOrder)
            ? Qt::DescendingOrder
            : Qt::AscendingOrder;
    }
    else {
        newOrder = Qt::AscendingOrder;
    }

    currentSortColumn = logicalIndex;
    currentSortOrder = newOrder;

    std::sort(contacts.begin(), contacts.end(), [logicalIndex, newOrder](const Contact& a, const Contact& b) {
        bool less = false;
        switch (logicalIndex) {
        case 0: less = a.lastName < b.lastName; break;
        case 1: less = a.firstName < b.firstName; break;
        case 2: less = a.patronymic < b.patronymic; break;
        case 3: less = a.address < b.address; break;
        case 4: less = a.birthDate < b.birthDate; break;
        case 5: less = a.email < b.email; break;
        case 6: {
            QStringList aPhones, bPhones;
            for (const auto& p : a.phoneNumbers) aPhones << p.display();
            for (const auto& p : b.phoneNumbers) bPhones << p.display();
            less = aPhones.join("; ") < bPhones.join("; ");
            break;
        }
        default: less = false;
        }
        return (newOrder == Qt::AscendingOrder) ? less : !less;
        });

    updateTable();
    table->horizontalHeader()->setSortIndicatorShown(true);
    table->horizontalHeader()->setSortIndicator(logicalIndex, newOrder);
}

void MainWindow::onSearchTextChanged(const QString& text)
{
    QString input = text.trimmed();
    if (input.isEmpty()) {
        for (int row = 0; row < table->rowCount(); ++row)
            table->setRowHidden(row, false);
        return;
    }

    QStringList words = input.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    for (int i = 0; i < words.size(); ++i)
        words[i] = words[i].toLower();

    for (int row = 0; row < table->rowCount(); ++row) {
        const Contact& c = contacts[row];

        QStringList fields;
        fields << c.lastName << c.firstName << c.patronymic
            << c.address << c.email
            << c.birthDate.toString("dd.MM.yyyy");

        for (const auto& phone : c.phoneNumbers) {
            fields << phone.display();
        }

        for (QString& field : fields)
            field = field.toLower();

        bool contactMatches = true;
        for (const QString& word : words) {
            bool wordFound = false;
            for (const QString& field : fields) {
                if (field.contains(word)) {
                    wordFound = true;
                    break;
                }
            }
            if (!wordFound) {
                contactMatches = false;
                break;
            }
        }
        table->setRowHidden(row, !contactMatches);
    }
}

QString MainWindow::normalizePhoneNumber(const QString& input)
{
    QString cleaned = input;
    cleaned.remove(QRegularExpression("[^\\d+]"));
    if (cleaned.contains('+')) {
        cleaned.remove('+');
        cleaned.prepend('+');
    }
    return cleaned;
}

bool MainWindow::validateContact(const Contact& c, QString& error)
{
    auto isValidName = [](const QString& s) -> bool {
        if (s.isEmpty()) return false;
        QString trimmed = s.trimmed();
        if (trimmed.isEmpty()) return false;
        if (trimmed.startsWith('-') || trimmed.endsWith('-')) {
            return false;
        }
        // https://stackoverflow.com/questions/12933034/what-does-this-regex-mean
        QRegularExpression re(R"(^[A-ZА-ЯЁ][a-zа-яё0-9]*(?:[ \-][A-ZА-ЯЁa-zа-яё0-9][a-zа-яё0-9]*)*$)");
        return re.match(trimmed).hasMatch();
        };

    if (!isValidName(c.lastName)) { error = "Некорректная фамилия"; return false; }
    if (!isValidName(c.firstName)) { error = "Некорректное имя"; return false; }
    if (!c.patronymic.isEmpty() && !isValidName(c.patronymic)) {
        error = "Некорректное отчество";
        return false;
    }

    if (c.birthDate >= QDate::currentDate()) {
        error = "Дата рождения не может быть сегодня или в будущем";
        return false;
    }

    QRegularExpression emailRe(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!emailRe.match(c.email.trimmed()).hasMatch()) {
        error = "Некорректный email";
        return false;
    }

    if (c.phoneNumbers.isEmpty()) {
        error = "Должен быть хотя бы один телефон";
        return false;
    }

    for (const PhoneEntry& phone : c.phoneNumbers) {
        if (phone.number.isEmpty()) {
            error = "Номер не может быть пустым";
            return false;
        }
        QRegularExpression phoneRe(R"(^\+?\d{7,15}$)");
        if (!phoneRe.match(phone.number).hasMatch()) {
            error = "Некорректный номер телефона";
            return false;
        }
    }

    return true;
}

void MainWindow::onAddContact()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Новый контакт");

    auto* layout = new QFormLayout(&dialog);

    auto* leLast = new QLineEdit;
    auto* leFirst = new QLineEdit;
    auto* lePatr = new QLineEdit;
    auto* leAddr = new QLineEdit;
    auto* deBirth = new QDateEdit;
    deBirth->setDate(QDate::currentDate().addYears(-20));
    deBirth->setCalendarPopup(true);
    auto* leEmail = new QLineEdit;

    auto* phonesList = new QListWidget;
    phonesList->setMinimumHeight(100);

    auto* btnWork = new QPushButton("Рабочий");
    auto* btnHome = new QPushButton("Домашний");
    auto* btnMobile = new QPushButton("Служебный");
    btnWork->setCheckable(true);
    btnHome->setCheckable(true);
    btnMobile->setCheckable(true);
    btnWork->setChecked(true);

    auto* typeGroup = new QButtonGroup(&dialog);
    typeGroup->addButton(btnWork, 0);
    typeGroup->addButton(btnHome, 1);
    typeGroup->addButton(btnMobile, 2);

    auto* typeLayout = new QHBoxLayout;
    typeLayout->addWidget(btnWork);
    typeLayout->addWidget(btnHome);
    typeLayout->addWidget(btnMobile);

    auto* lePhoneInput = new QLineEdit;
    lePhoneInput->setPlaceholderText("Введите номер телефона");
    auto* btnAddPhone = new QPushButton("Добавить телефон");
    connect(btnAddPhone, &QPushButton::clicked, [&]() {
        QString raw = lePhoneInput->text().trimmed();
        if (raw.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Введите номер телефона");
            return;
        }

        QRegularExpression inputRe(R"(^[\d+\-\(\)\s]+$)");
        if (!inputRe.match(raw).hasMatch()) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер может содержать только цифры, +, -, (, ), пробелы");
            return;
        }

        QString normalized = normalizePhoneNumber(raw);
        if (normalized.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер не содержит цифр");
            return;
        }

        QString digitsOnly = normalized;
        if (digitsOnly.startsWith('+')) digitsOnly = digitsOnly.mid(1);
        if (digitsOnly.length() < 7 || digitsOnly.length() > 15) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер должен содержать от 7 до 15 цифр");
            return;
        }

        QString type;
        if (btnHome->isChecked()) type = "домашний";
        else if (btnMobile->isChecked()) type = "служебный";
        else type = "рабочий";

        QListWidgetItem* item = new QListWidgetItem(type + ": " + normalized, phonesList);
        QVariantMap data;
        data["number"] = normalized;
        data["type"] = type;
        item->setData(Qt::UserRole, data);

        lePhoneInput->clear();
        lePhoneInput->setFocus();
        });

    layout->addRow("Фамилия*", leLast);
    layout->addRow("Имя*", leFirst);
    layout->addRow("Отчество*", lePatr);
    layout->addRow("Адрес*", leAddr);
    layout->addRow("Дата рождения*", deBirth);
    layout->addRow("Email*", leEmail);
    layout->addRow("Телефоны*", phonesList);
    layout->addRow("Тип*", typeLayout);
    layout->addRow("Номер*", lePhoneInput);
    layout->addRow("", btnAddPhone);

    auto* btnBox = new QHBoxLayout;
    auto* okBtn = new QPushButton("OK");
    auto* cancelBtn = new QPushButton("Отмена");
    btnBox->addStretch();
    btnBox->addWidget(okBtn);
    btnBox->addWidget(cancelBtn);
    layout->addRow(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    connect(okBtn, &QPushButton::clicked, [&]() {
        Contact c;
        c.lastName = leLast->text().trimmed();
        c.firstName = leFirst->text().trimmed();
        c.patronymic = lePatr->text().trimmed();
        c.address = leAddr->text().trimmed();
        c.birthDate = deBirth->date();
        c.email = leEmail->text().trimmed();

        c.phoneNumbers.clear();
        for (int i = 0; i < phonesList->count(); ++i) {
            QVariant data = phonesList->item(i)->data(Qt::UserRole);
            if (data.isValid()) {
                QVariantMap map = data.toMap();
                PhoneEntry entry;
                entry.number = map["number"].toString();
                entry.type = map["type"].toString();
                c.phoneNumbers.append(entry);
            }
        }

        QString error;
        if (validateContact(c, error)) {
            contacts.append(c);
            updateTable();
            dialog.accept();
        }
        else {
            QMessageBox::warning(&dialog, "Ошибка", error);
        }
        });

    dialog.exec();
}

void MainWindow::onEditContact()
{
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Инфо", "Выберите контакт для редактирования");
        return;
    }

    Contact c = contacts[row];

    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать контакт");

    auto* layout = new QFormLayout(&dialog);

    auto* leLast = new QLineEdit(c.lastName);
    auto* leFirst = new QLineEdit(c.firstName);
    auto* lePatr = new QLineEdit(c.patronymic);
    auto* leAddr = new QLineEdit(c.address);
    auto* deBirth = new QDateEdit(c.birthDate);
    deBirth->setCalendarPopup(true);
    auto* leEmail = new QLineEdit(c.email);

    auto* phonesList = new QListWidget;
    phonesList->setMinimumHeight(100);
    for (const PhoneEntry& p : c.phoneNumbers) {
        QListWidgetItem* item = new QListWidgetItem(p.display(), phonesList);
        QVariantMap data;
        data["number"] = p.number;
        data["type"] = p.type;
        item->setData(Qt::UserRole, data);
    }

    auto* btnWork = new QPushButton("Рабочий");
    auto* btnHome = new QPushButton("Домашний");
    auto* btnMobile = new QPushButton("Служебный");
    btnWork->setCheckable(true);
    btnHome->setCheckable(true);
    btnMobile->setCheckable(true);
    btnWork->setChecked(true);

    auto* typeGroup = new QButtonGroup(&dialog);
    typeGroup->addButton(btnWork, 0);
    typeGroup->addButton(btnHome, 1);
    typeGroup->addButton(btnMobile, 2);

    auto* typeLayout = new QHBoxLayout;
    typeLayout->addWidget(btnWork);
    typeLayout->addWidget(btnHome);
    typeLayout->addWidget(btnMobile);

    auto* lePhoneInput = new QLineEdit;
    lePhoneInput->setPlaceholderText("Введите номер телефона");
    auto* btnAddPhone = new QPushButton("Добавить телефон");
    connect(btnAddPhone, &QPushButton::clicked, [&]() {
        QString raw = lePhoneInput->text().trimmed();
        if (raw.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Введите номер телефона");
            return;
        }

        QRegularExpression inputRe(R"(^[\d+\-\(\)\s]+$)");
        if (!inputRe.match(raw).hasMatch()) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер может содержать только цифры, +, -, (, ), пробелы");
            return;
        }

        QString normalized = normalizePhoneNumber(raw);
        if (normalized.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер не содержит цифр");
            return;
        }

        QString digitsOnly = normalized;
        if (digitsOnly.startsWith('+')) digitsOnly = digitsOnly.mid(1);
        if (digitsOnly.length() < 7 || digitsOnly.length() > 15) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер должен содержать от 7 до 15 цифр");
            return;
        }

        QString type;
        if (btnHome->isChecked()) type = "домашний";
        else if (btnMobile->isChecked()) type = "служебный";
        else type = "рабочий";

        QListWidgetItem* item = new QListWidgetItem(type + ": " + normalized, phonesList);
        QVariantMap data;
        data["number"] = normalized;
        data["type"] = type;
        item->setData(Qt::UserRole, data);

        lePhoneInput->clear();
        lePhoneInput->setFocus();
        });

    QPushButton* removePhoneBtn = new QPushButton("Удалить выбранный телефон");
    connect(removePhoneBtn, &QPushButton::clicked, [=]() {
        auto* item = phonesList->currentItem();
        if (item) delete item;
        });

    layout->addRow("Фамилия*", leLast);
    layout->addRow("Имя*", leFirst);
    layout->addRow("Отчество*", lePatr);
    layout->addRow("Адрес*", leAddr);
    layout->addRow("Дата рождения*", deBirth);
    layout->addRow("Email*", leEmail);
    layout->addRow("Телефоны*", phonesList);
    layout->addRow("Тип*", typeLayout);
    layout->addRow("Номер*", lePhoneInput);
    layout->addRow("", btnAddPhone);
    layout->addRow("", removePhoneBtn);

    auto* btnBox = new QHBoxLayout;
    auto* okBtn = new QPushButton("OK");
    auto* cancelBtn = new QPushButton("Отмена");
    btnBox->addStretch();
    btnBox->addWidget(okBtn);
    btnBox->addWidget(cancelBtn);
    layout->addRow(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    connect(okBtn, &QPushButton::clicked, [&, row]() {
        Contact updated;
        updated.lastName = leLast->text().trimmed();
        updated.firstName = leFirst->text().trimmed();
        updated.patronymic = lePatr->text().trimmed();
        updated.address = leAddr->text().trimmed();
        updated.birthDate = deBirth->date();
        updated.email = leEmail->text().trimmed();

        updated.phoneNumbers.clear();
        for (int i = 0; i < phonesList->count(); ++i) {
            QVariant data = phonesList->item(i)->data(Qt::UserRole);
            if (data.isValid()) {
                QVariantMap map = data.toMap();
                PhoneEntry entry;
                entry.number = map["number"].toString();
                entry.type = map["type"].toString();
                updated.phoneNumbers.append(entry);
            }
        }

        QString error;
        if (validateContact(updated, error)) {
            contacts[row] = updated;
            updateTable();
            dialog.accept();
        }
        else {
            QMessageBox::warning(&dialog, "Ошибка", error);
        }
        });

    dialog.exec();
}

void MainWindow::onDeleteContact()
{
    QList<QTableWidgetSelectionRange> selection = table->selectedRanges();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "Инфо", "Выберите контакт(ы) для удаления");
        return;
    }

    // collect all VISUALLY selected rows
    QSet<int> visualRows;
    for (const QTableWidgetSelectionRange& range : selection) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            visualRows.insert(row);
        }
    }

    // get real contacts indexes
    QList<int> contactIndices;
    for (int visualRow : visualRows) {
        QTableWidgetItem* item0 = table->item(visualRow, 0);
        if (item0) {
            int idx = item0->data(Qt::UserRole).toInt();
            if (idx >= 0 && idx < contacts.size()) {
                contactIndices.append(idx);
            }
        }
    }

    if (contactIndices.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось определить выбранные контакты");
        return;
    }

    // sorting for correct deletion
    std::sort(contactIndices.begin(), contactIndices.end());
    contactIndices.erase(std::unique(contactIndices.begin(), contactIndices.end()), contactIndices.end());
    std::reverse(contactIndices.begin(), contactIndices.end());

    if (QMessageBox::question(this, "Подтверждение",
        QString("Удалить %1 контакт(ов)?").arg(contactIndices.size())) == QMessageBox::Yes) {
        for (int idx : contactIndices) {
            contacts.removeAt(idx);
        }
        updateTable();
    }
}

void MainWindow::onSaveToFile()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Сохранить справочник", currentFilePath, "JSON Files (*.json)"
    );
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".json"))
            filename += ".json";
        saveContactsToJson(filename);
        currentFilePath = filename;
        QMessageBox::information(this, "Успех", "Справочник сохранён!");
    }
}

void MainWindow::onLoadFromFile()
{
    QString filename = QFileDialog::getOpenFileName(
        this, "Загрузить справочник", "", "JSON Files (*.json)"
    );
    if (!filename.isEmpty()) {
        loadContactsFromJson(filename);
        currentFilePath = filename;
        updateTable();
        QMessageBox::information(this, "Успех", "Справочник загружен!");
    }
}

void MainWindow::saveContactsToJson(const QString& filename)
{
    QJsonArray array;
    for (const Contact& c : contacts)
        array.append(c.toJson());

    QJsonDocument doc(array);
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::loadContactsFromJson(const QString& filename)
{
    QFile file(filename);
    if (!file.exists()) {
        contacts.clear();
        updateTable();
        return;
    }
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isArray()) {
            QMessageBox::warning(this, "Ошибка", "Неверный формат файла");
            return;
        }
        contacts.clear();
        QJsonArray array = doc.array();
        for (const QJsonValue& val : array) {
            if (val.isObject())
                contacts.append(Contact::fromJson(val.toObject()));
        }
        updateTable();
    }
}