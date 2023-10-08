#include "updateitemdialog.h"
#include "ui_updateitemdialog.h"
#include <QFileDialog>
#include <QMessageBox>

UpdateItemDialog::UpdateItemDialog(Item* currentItem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateItemDialog)
{
    ui->setupUi(this);

    //postavljamo varijablu currentItem na vec postojecu varijablu currentItem
    this->currentItem = currentItem;

    //neke osnovne informacije
    if(currentItem != nullptr)
    {
        ui->lblItemName->setText(currentItem->vratiIme());
        ui->sbQuantity->setValue(currentItem->vratiKolicinu());
        ui->sbBuyingPrice->setValue(currentItem->vratiKupovnuCijenu());
        ui->sbSellingPrice->setValue(currentItem->vratiProdajnuCijenu());

        QPixmap pixmap(currentItem->vratiImageFilePath());
        ui->lblImage->setPixmap(pixmap);
        ui->lblImage->setScaledContents(true);
        imageFilePath = currentItem->vratiImageFilePath();
    }

    //konekcije
    connect(ui->btnConfirmEdit, &QPushButton::clicked,
            this, &UpdateItemDialog::confirmUpdate);

    connect(ui->btnLoadItemImage, &QPushButton::clicked,
            this, &UpdateItemDialog::loadItemImage);
}

UpdateItemDialog::~UpdateItemDialog()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------------------
//funkcionalnost metoda

void UpdateItemDialog::confirmUpdate()
{
    int kolicina = ui->sbQuantity->value();
    int kupovnaCijena = ui->sbBuyingPrice->value();
    int prodajnaCijena = ui->sbSellingPrice->value();

    if(kolicina >= 1)
    {
        currentItem->postaviKolicinu(kolicina);
        currentItem->postaviKupovnuCijenu(kupovnaCijena);
        currentItem->postaviProdajnuCijenu(prodajnaCijena);
        currentItem->postaviImageFilePath(imageFilePath);
        this->close();
    }
    else
    {
        QMessageBox mb;
        mb.setText("Kolicina mora biti najmanje 1.");
        mb.exec();
    }    
}//kraj confirmUpdate

void UpdateItemDialog::loadItemImage()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this,
                                            "Open Image", "./",
                                            "Image Files (*.png *.jpg)");

    if(filename != "")
    {
        int lastSlash = filename.lastIndexOf("/");
        QString shortName = filename.right(filename.size() - lastSlash - 1);

        QFile::copy(filename, "./images/" + shortName);
        QPixmap pixmap("./images/" + shortName);

        ui->lblImage->setPixmap(pixmap);
        ui->lblImage->setScaledContents(true);

        //azurira unutarnje podatke
        imageFilePath = "./images/" + shortName;
    }//kraj if
}//kraj loadItemImage
