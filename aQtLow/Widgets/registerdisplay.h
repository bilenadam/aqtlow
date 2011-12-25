#ifndef REGISTERDISPLAY_H
#define REGISTERDISPLAY_H

#include <QWidget>

#include "globals.h"

namespace Ui {
    class RegisterDisplay;    
}

class RegisterDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterDisplay(QWidget *parent = 0);
    ~RegisterDisplay();
    REGISTER *R;
    int p, r;
    bool AllowEdit;
    int Decimals;

private:
    Ui::RegisterDisplay *ui;

public slots:
    void Setup(int p, int r, bool AllowEdit, int FontSize, int Decimals);
    void Refresh();
    void SetText(QString Text);

private slots:
    void on_lineEdit_editingFinished();
};

#endif // REGISTERDISPLAY_H
