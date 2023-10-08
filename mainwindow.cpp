#include "additemdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "updateitemdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <cstdlib>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //spajanje svake metode sa odredjenim ui objektom i odrediti signal
    //konekcije
    connect(ui->menuNewProduct, &QAction::triggered,
            this, &MainWindow::handleMenuItemNew);

    connect(ui->btnRemove, &QPushButton::clicked,
            this, &MainWindow::removeSelectedProduct);

    connect(ui->lstProducts, &QListWidget::itemClicked,
            this, &MainWindow::handleItemClick);

    connect(ui->menuEditSelectedProduct, &QAction::triggered,
            this, &MainWindow::handleMenuItemEdit);

    connect(ui->menuSaveProducts, &QAction::triggered,
            this, &MainWindow::handleSaveItems);

    connect(ui->menuLoadProducts, &QAction::triggered,
            this, &MainWindow::handleLoadItems);

    connect(ui->btnBuy, &QPushButton::clicked,
            this, &MainWindow::buySelectedProduct);

    connect(ui->btnSell, &QPushButton::clicked,
            this, &MainWindow::sellSelectedProduct);
}

MainWindow::~MainWindow()
{
    //oslobadja memoriju liste artikala
    for(Item* product : listaArtikala)
    {
        delete product;
    }
    listaArtikala.clear();

    delete ui;
}

//------------------------------------------------------------------------------------------------------------
//funkcionalnost metoda

void MainWindow::handleMenuItemNew()
{
    Item* newItem = nullptr;
    AddItemDialog addItemDialog(newItem, nullptr);

    addItemDialog.setModal(true);
    addItemDialog.exec();

    if(newItem != nullptr)
    {
        listaArtikala.push_back(newItem);
        ui->lstProducts->addItem(newItem->vratiIme());
    }//kraj if

    int cijena = newItem->vratiKupovnuCijenu();
    int koliko = newItem->vratiKolicinu();
    ui->sbMoney->setValue(novac - cijena * koliko);
    novac = ui->sbMoney->value();

}//kraj handleMenuItemNew

void MainWindow::removeSelectedProduct()
{
    int index = ui->lstProducts->currentRow();

    if(index >= 0)
    {
        //ukloniti iz vektora
        Item* theItem = listaArtikala.at(index);

        int cijena = theItem->vratiKupovnuCijenu();
        int koliko = theItem->vratiKolicinu();
        ui->sbMoney->setValue(novac + cijena * koliko);
        novac = ui->sbMoney->value();

        delete theItem;
        listaArtikala.removeAt(index);

        //ukloniti iz widget liste u UI-u
        delete ui->lstProducts->currentItem();

    }//kraj if

    //postavlja sliku na none.png (razan prostor)
    QPixmap pixmap("none.png");
    ui->lblImage->setPixmap(pixmap);

    //postavlja lables na prazno
    ui->lblProductName->setText("");
    ui->lblQuantity->setText("");
    ui->lblBuyingPrice->setText("");
    ui->lblSellingPrice->setText("");

}//kraj removeSelectedproduct

void MainWindow::handleItemClick(QListWidgetItem* item)
{
    int index = item->listWidget()->currentRow();

    if(index != -1)
    {
        Item* currentItem = listaArtikala.at(index);

        if(currentItem != nullptr)
        {
            ui->lblProductName->setText(currentItem->vratiIme());
            ui->lblQuantity->setText(QString::number(currentItem->vratiKolicinu()));
            ui->lblBuyingPrice->setText(QString::number(currentItem->vratiKupovnuCijenu()));
            ui->lblSellingPrice->setText(QString::number(currentItem->vratiProdajnuCijenu()));

            QPixmap pixmap(currentItem->vratiImageFilePath());
            ui->lblImage->setPixmap(pixmap);
            ui->lblImage->setScaledContents(true);
        }//kraj unutrasnjeg if
    }//kraj if
}//kraj handleItemClick

void MainWindow::handleMenuItemEdit()
{
    int index = ui->lstProducts->currentRow();

    if(index != -1)
    {
        Item* currentItem = listaArtikala.at(index);

        if(currentItem != nullptr)
        {
            UpdateItemDialog updateItemDialog(currentItem, nullptr);
            updateItemDialog.exec();

            //treba biti siguran da je ui azuriran
            ui->lblProductName->setText(currentItem->vratiIme());
            ui->lblQuantity->setText(QString::number(currentItem->vratiKolicinu()));
            ui->lblBuyingPrice->setText(QString::number(currentItem->vratiKupovnuCijenu()));
            ui->lblSellingPrice->setText(QString::number(currentItem->vratiProdajnuCijenu()));

            QPixmap pixmap(currentItem->vratiImageFilePath());
            ui->lblImage->setPixmap(pixmap);
            ui->lblImage->setScaledContents(true);

            //ovo ispod ucitava promjenu novca nakon edita            
            int ukupnaCijena = 0;
            int count = ui->lstProducts->count();
            for(int i = 0; i < count; i++)
            {
                Item* currentItem = listaArtikala.at(i);

                int cijena = currentItem->vratiKupovnuCijenu();
                int koliko = currentItem->vratiKolicinu();

                ukupnaCijena +=  cijena * koliko;
            }
            if(ui->sbMoney->value() < 0)
                novac = 0;
            ui->sbMoney->setValue(novac - ukupnaCijena);
            novac = ui->sbMoney->value();
        }
    }
}//kraj handleMenuItemEdit

void MainWindow::handleSaveItems()
{
    QFile outputFile("products.txt");

    outputFile.open(QIODevice::WriteOnly |
                    QIODevice::Text);

    QTextStream out(&outputFile);

    for(Item* product : listaArtikala)
    {
        out<<product->vratiIme()<<",";
        out<<product->vratiKolicinu()<<",";
        out<<product->vratiKupovnuCijenu()<<",";
        out<<product->vratiProdajnuCijenu()<<",";
        out<<product->vratiImageFilePath()<<Qt::endl;
    }//kraj for

    out.flush();
    outputFile.close();
}//kraj handleSaveItems

void MainWindow::handleLoadItems()
{
    QFile inputFile("products.txt");

    inputFile.open(QIODevice::ReadOnly |
                   QIODevice::Text);

    QTextStream in(&inputFile);

    //ocistiti postojecu listu i vektor
    for(Item* temp : listaArtikala)
    {
        delete temp;
    }//kraj for

    listaArtikala.clear();      //data model
    ui->lstProducts->clear();   //view / ui

    novac = 0;
    int ukupnaCijena = 0;
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList info = line.split(",");

        //handle list of products UI
        ui->lstProducts->addItem(info.at(0));

        //handle vector
        Item* product = new Item(info.at(0),
                                 info.at(1).toInt(),
                                 info.at(2).toInt(),
                                 info.at(3).toInt(),
                                 info.at(4));
        listaArtikala.push_back(product);

        int cijena = product->vratiKupovnuCijenu();
        int koliko = product->vratiKolicinu();

        ukupnaCijena +=  cijena * koliko;

    }//kraj while
    ui->sbMoney->setValue(novac - ukupnaCijena);
    novac = ui->sbMoney->value();

    in.flush();
    inputFile.close();

    //postavlja sliku na none.png (razan prostor)
    QPixmap pixmap("none.png");
    ui->lblImage->setPixmap(pixmap);

    //postavlja lables na prazno
    ui->lblProductName->setText("");
    ui->lblQuantity->setText("");
    ui->lblBuyingPrice->setText("");
    ui->lblSellingPrice->setText("");
}//kraj handleLoadItems

void MainWindow::buySelectedProduct()
{
    int index = ui->lstProducts->currentRow();

    if(index >= 0)
    {
        //ukloniti iz vektora
        Item* theItem = listaArtikala.at(index);

        int number = ui->sbBuy->value();
        int kolicina = theItem->vratiKolicinu();
        kolicina += number;
        theItem->postaviKolicinu(kolicina);
        ui->lblQuantity->setText(QString::number(theItem->vratiKolicinu()));

        int cijena = theItem->vratiKupovnuCijenu();
        ui->sbMoney->setValue(novac - cijena * number);
        novac = ui->sbMoney->value();
    }
}//kraj buySelectedProduct

void MainWindow::sellSelectedProduct()
{
    int index = ui->lstProducts->currentRow();

    if(index >= 0)
    {
        //ukloniti iz vektora
        Item* theItem = listaArtikala.at(index);

        int number = ui->sbSell->value();
        if(number >= theItem->vratiKolicinu())
        {
            QMessageBox mb;
            mb.setText("Ne mozete prodati vise od onoga sto imate.");
            mb.exec();
        }
        else
        {
            int kolicina = theItem->vratiKolicinu();
            kolicina -= number;
            theItem->postaviKolicinu(kolicina);
            ui->lblQuantity->setText(QString::number(theItem->vratiKolicinu()));

            int cijena = theItem->vratiProdajnuCijenu();
            ui->sbMoney->setValue(novac + cijena * number);
            novac = ui->sbMoney->value();
        }
    }
}//kraj sellSelectedProduct
