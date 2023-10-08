#include "Item.h"

Item::Item(QString ime, int kolicina, int kupovnaCijena, int prodajnaCijena, QString imageFilePath)
{
    this->ime = ime;
    this->kolicina = kolicina;
    this->kupovnaCijena = kupovnaCijena;
    this->prodajnaCijena = prodajnaCijena;
    this->imageFilePath = imageFilePath;
}


void Item::postaviIme(QString ime)
{
    this->ime = ime;
}
QString Item::vratiIme() const
{
    return ime;
}


void Item::postaviKolicinu(int kolicina)
{
    this->kolicina = kolicina;
}
int Item::vratiKolicinu() const
{
    return kolicina;
}


void Item::postaviKupovnuCijenu(int kupovnaCijena)
{
    this->kupovnaCijena = kupovnaCijena;
}
int Item::vratiKupovnuCijenu() const
{
    return kupovnaCijena;
}

void Item::postaviProdajnuCijenu(int prodajnaCijena)
{
    this->prodajnaCijena = prodajnaCijena;
}
int Item::vratiProdajnuCijenu() const
{
    return prodajnaCijena;
}


void Item::postaviImageFilePath(QString imageFilePath)
{
    this->imageFilePath = imageFilePath;
}
QString Item::vratiImageFilePath() const
{
    return imageFilePath;
}
