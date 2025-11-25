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
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm> // ← для std::sort

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
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
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSortIndicatorShown(false);

    // ✅ Подключаем клик по заголовку для сортировки
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
        table->setItem(i, 6, new QTableWidgetItem(c.phoneNumbers.join(", ")));
    }
}

void MainWindow::onHeaderClicked(int logicalIndex)
{
    Qt::SortOrder newOrder;

    if (logicalIndex == currentSortColumn) {
        // Клик по тому же столбцу — меняем направление
        newOrder = (currentSortOrder == Qt::AscendingOrder)
            ? Qt::DescendingOrder
            : Qt::AscendingOrder;
    }
    else {
        // Клик по новому столбцу — сортируем по возрастанию
        newOrder = Qt::AscendingOrder;
    }

    // Обновляем состояние
    currentSortColumn = logicalIndex;
    currentSortOrder = newOrder;

    // Сортируем contacts
    std::sort(contacts.begin(), contacts.end(), [logicalIndex, newOrder](const Contact& a, const Contact& b) {
        bool less = false;
        switch (logicalIndex) {
        case 0: less = a.lastName < b.lastName; break;
        case 1: less = a.firstName < b.firstName; break;
        case 2: less = a.patronymic < b.patronymic; break;
        case 3: less = a.address < b.address; break;
        case 4: less = a.birthDate < b.birthDate; break;
        case 5: less = a.email < b.email; break;
        case 6: less = a.phoneNumbers.join(", ") < b.phoneNumbers.join(", "); break;
        default: less = false;
        }

        return (newOrder == Qt::AscendingOrder) ? less : !less;
        });

    updateTable();

    // Устанавливаем индикатор сортировки в заголовке
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
        const Contact& c = contacts[row]; // ← теперь безопасно!

        QStringList fields;
        fields << c.lastName << c.firstName << c.patronymic
            << c.address << c.email
            << c.birthDate.toString("dd.MM.yyyy")
            << c.phoneNumbers;

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
    if (cleaned.startsWith('+'))
        cleaned = cleaned.mid(1);
    return cleaned;
}

bool MainWindow::validateContact(const Contact& c, QString& error)
{
    auto isValidName = [](const QString& s) -> bool {
        if (s.isEmpty()) return false;
        QString trimmed = s.trimmed();
        if (trimmed.isEmpty()) return false;
        if (trimmed.startsWith('-') || trimmed.endsWith('-'))
            return false;
        QRegularExpression re(R"(^[A-ZА-ЯЁ][a-zа-яё0-9]*(?:[\s\-][A-ZА-ЯЁ][a-zа-яё0-9]*)*$)");
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

    for (const QString& phone : c.phoneNumbers) {
        if (phone.isEmpty() || phone.length() < 7 || phone.length() > 15) {
            error = "Некорректный номер телефона (длина)";
            return false;
        }
        if (!phone.contains(QRegularExpression("^[0-9]+$"))) {
            error = "Номер телефона должен содержать только цифры";
            return false;
        }
    }

    return true;
}

// --- onAddContact, onEditContact, onDeleteContact — без изменений (остаются как в твоём коде) ---

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
    auto* lePhoneInput = new QLineEdit;
    lePhoneInput->setPlaceholderText("Введите номер телефона");
    auto* btnAddPhone = new QPushButton("Добавить телефон");
    connect(btnAddPhone, &QPushButton::clicked, [&]() {
        QString raw = lePhoneInput->text().trimmed();
        if (raw.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Введите номер телефона");
            return;
        }

        QString normalized = normalizePhoneNumber(raw);
        if (normalized.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер не содержит цифр");
            return;
        }

        if (normalized.length() < 7 || normalized.length() > 15) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер должен содержать от 7 до 15 цифр");
            return;
        }

        if (!normalized.contains(QRegularExpression("^[0-9]+$"))) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер должен содержать только цифры");
            return;
        }

        phonesList->addItem(normalized);
        lePhoneInput->clear();
        lePhoneInput->setFocus();
        });

    layout->addRow("Фамилия*", leLast);
    layout->addRow("Имя*", leFirst);
    layout->addRow("Отчество", lePatr);
    layout->addRow("Адрес", leAddr);
    layout->addRow("Дата рождения*", deBirth);
    layout->addRow("Email*", leEmail);
    layout->addRow("Телефоны*", phonesList);
    layout->addRow("Номер", lePhoneInput);
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

        for (int i = 0; i < phonesList->count(); ++i)
            c.phoneNumbers.append(phonesList->item(i)->text());

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

    Contact c = contacts[row]; // ← теперь всегда корректно

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
    for (const QString& p : c.phoneNumbers)
        phonesList->addItem(p);

    auto* lePhoneInput = new QLineEdit;
    lePhoneInput->setPlaceholderText("Введите номер телефона");
    auto* btnAddPhone = new QPushButton("Добавить телефон");
    connect(btnAddPhone, &QPushButton::clicked, [&]() {
        QString raw = lePhoneInput->text().trimmed();
        if (raw.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Введите номер телефона");
            return;
        }

        QString normalized = normalizePhoneNumber(raw);
        if (normalized.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер не содержит цифр");
            return;
        }

        if (normalized.length() < 7 || normalized.length() > 15) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер должен содержать от 7 до 15 цифр");
            return;
        }

        if (!normalized.contains(QRegularExpression("^[0-9]+$"))) {
            QMessageBox::warning(&dialog, "Ошибка", "Номер должен содержать только цифры");
            return;
        }

        phonesList->addItem(normalized);
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
    layout->addRow("Отчество", lePatr);
    layout->addRow("Адрес", leAddr);
    layout->addRow("Дата рождения*", deBirth);
    layout->addRow("Email*", leEmail);
    layout->addRow("Телефоны*", phonesList);
    layout->addRow("Номер", lePhoneInput);
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
        for (int i = 0; i < phonesList->count(); ++i)
            updated.phoneNumbers.append(phonesList->item(i)->text());

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
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Инфо", "Выберите контакт для удаления");
        return;
    }

    if (QMessageBox::question(this, "Подтверждение",
        "Удалить выбранный контакт?") == QMessageBox::Yes) {
        contacts.removeAt(row);
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