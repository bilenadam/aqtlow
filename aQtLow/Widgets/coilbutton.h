#ifndef COILBUTTON_H
#define COILBUTTON_H

#include <QWidget>

#include "globals.h"

namespace Ui {
    class CoilButton;
}

class CoilButton : public QWidget
{
    Q_OBJECT

public:
    explicit CoilButton(QWidget *parent = 0);
    ~CoilButton();
    COIL *C;
    int p, c;
    QString Text;
    enum TypeOfButton{
        SET, RST, TGL
    }TOB;

private:
    Ui::CoilButton *ui;

public slots:
    void Setup(int p, int c, TypeOfButton TOB, QString Text, int IconSize, int FontSize);
    void SetText(QString Text);
    void Refresh();

private slots:
    void on_pushButton_clicked();
};

#endif // COILBUTTON_H
