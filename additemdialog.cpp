#include "additemdialog.h"
#include "ui_additemdialog.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

AddItemDialog::AddItemDialog(Item*& noviArtikl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);

    this->noviArtikl = &noviArtikl;
    imageFilePath = "none.png"; //defaultna vrijednost

    //registrovanje događaja
    //konekcije
    connect(ui->btnConfirmAdd, &QPushButton::clicked,
            this, &AddItemDialog::potvrdiDodavanje);

    connect(ui->btnLoadImage, &QPushButton::clicked,
            this, &AddItemDialog::ucitajSlikuArtikla);

    QDir pathDir("./images");
    if(!pathDir.exists())
    {
        //ako nema slike, kreirat cemo je
        QDir().mkdir("./images");
    }

}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------------------
//funkcionalnost metoda

void AddItemDialog:: potvrdiDodavanje()
{
    QString ImeArtikla = ui->txtProductName->text();
    int kolicina = ui->sbQuantity->value();
    int kupovnaCijena = ui->sbBuyingPrice->value();
    int prodajnaCijena = ui->sbSellingPrice->value();

    if(ImeArtikla.trimmed() != "" && kolicina >= 1)
    {
        *noviArtikl = new Item(ImeArtikla, kolicina, kupovnaCijena, prodajnaCijena, imageFilePath);
        this->close();
    }
    else
    {
        QMessageBox mb;
        mb.setText("Morate imati validno ime i kolicinu najmanje 1");
        mb.exec();
    }
}//kraj potvrdiDodavanje

void AddItemDialog::ucitajSlikuArtikla()
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
}//kraj ucitajSlikuArtikla
