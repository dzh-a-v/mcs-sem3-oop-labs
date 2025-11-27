#pragma once

#include <QMainWindow>
#include <QList>
#include <QTableWidget>
#include <QLineEdit>

class Contact;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    QTableWidget* table; // was private, but now I need to resize rows after start

private slots:
    void onAddContact();
    void onEditContact();
    void onDeleteContact();
    void onSaveToFile();
    void onLoadFromFile();
    void onSearchTextChanged(const QString& text);
    void onHeaderClicked(int logicalIndex);

private:
    void updateTable();
    void saveContactsToJson(const QString& filename);
    void loadContactsFromJson(const QString& filename);
    bool validateContact(const Contact& c, QString& error);
    QString normalizePhoneNumber(const QString& input);

    QList<Contact> contacts;
    QLineEdit* searchBox;
    QString currentFilePath = "phonebook.json";

    int currentSortColumn = -1;
    Qt::SortOrder currentSortOrder = Qt::AscendingOrder;
};