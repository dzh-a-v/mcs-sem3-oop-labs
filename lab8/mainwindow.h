#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void onAddContact();
    void onEditContact();
    void onDeleteContact();
    void onSaveToFile();
    void onLoadFromFile();
    void onSearchTextChanged(const QString& text);
    void onHeaderClicked(int logicalIndex); // ← новое

private:
    void updateTable();
    void saveContactsToJson(const QString& filename);
    void loadContactsFromJson(const QString& filename);
    bool validateContact(const Contact& c, QString& error);
    QString normalizePhoneNumber(const QString& input);

    QList<Contact> contacts;
    QTableWidget* table;
    QLineEdit* searchBox;
    QString currentFilePath = "phonebook.json";

    // Для двунаправленной сортировки
    int currentSortColumn = -1;
    Qt::SortOrder currentSortOrder = Qt::AscendingOrder;
};

#endif // MAINWINDOW_H