#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item
{
    public:
        Item(QString ime, int kolicina, int kupovnaCijena, int prodajnaCijena, QString imageFilePath = "none.png");

        void postaviIme(QString ime);
        QString vratiIme() const;

        void postaviKolicinu(int kolicina);
        int vratiKolicinu() const;

        void postaviKupovnuCijenu(int kupovnaCijena);
        int vratiKupovnuCijenu() const;

        void postaviProdajnuCijenu(int prodajnaCijena);
        int vratiProdajnuCijenu() const;

        void postaviImageFilePath(QString imageFilePath);
        QString vratiImageFilePath() const;

    private:
        QString ime;
        int kolicina;
        int kupovnaCijena;
        int prodajnaCijena;
        QString imageFilePath;
};

#endif // ITEM_H
