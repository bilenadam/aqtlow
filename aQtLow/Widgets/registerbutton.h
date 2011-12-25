#ifndef REGISTERBUTTON_H
#define REGISTERBUTTON_H

#include <QWidget>

#include "globals.h"

namespace Ui {
    class RegisterButton;
}

class RegisterButton : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterButton(QWidget *parent = 0);
    ~RegisterButton();
    REGISTER *R;
    int p, r;
    double Value;
    QString Text;
    enum TypeOfButton{
        INC, DEC, SET
    }TOB;

private:
    Ui::RegisterButton *ui;

public slots:
    void Setup(int p, int r, TypeOfButton TOB, double Value, QString Text, int IconSize, int FontSize);
    void SetText(QString Text);
    void Refresh();

private slots:
    void on_pushButton_clicked();
};

#endif // REGISTERBUTTON_H
