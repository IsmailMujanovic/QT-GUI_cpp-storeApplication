#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Item.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //sve metode
    void handleMenuItemNew();
    void removeSelectedProduct();
    void handleItemClick(QListWidgetItem* item);
    void handleMenuItemEdit();
    void handleSaveItems();
    void handleLoadItems();
    void buySelectedProduct();
    void sellSelectedProduct();

private:
    Ui::MainWindow *ui;
    QVector<Item*> listaArtikala;
    int novac = 0;
};
#endif // MAINWINDOW_H
