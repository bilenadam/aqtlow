#ifndef COILDISPLAY_H
#define COILDISPLAY_H

#include <QWidget>

#include "globals.h"

namespace Ui {
    class CoilDisplay;
}

class CoilDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit CoilDisplay(QWidget *parent = 0);
    ~CoilDisplay();
    COIL *C;
    int p, c;

private:
    Ui::CoilDisplay *ui;
    QString OffIndicator, OnIndicator, OffText, OnText;

public slots:
    void Setup(int p, int c,
                 QString OffIndicator, QString OnIndicator, int IndicatorSize,
                 QString OffText, QString OnText, int FontSize);
    void Refresh();
    void SetText(QString Text);
};

#endif // COILDISPLAY_H
