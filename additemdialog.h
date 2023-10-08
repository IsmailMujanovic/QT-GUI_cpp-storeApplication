#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include "Item.h"

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(Item*& noviArtikl, QWidget* parent = nullptr);
    ~AddItemDialog();

    void potvrdiDodavanje();
    void ucitajSlikuArtikla();

private:
    Ui::AddItemDialog *ui;
    Item** noviArtikl;
    QString imageFilePath;
};

#endif // ADDITEMDIALOG_H
