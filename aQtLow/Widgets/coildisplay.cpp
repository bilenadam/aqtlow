#include "coildisplay.h"
#include "ui_coildisplay.h"

CoilDisplay::CoilDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoilDisplay)
{
    ui->setupUi(this);
}

CoilDisplay::~CoilDisplay()
{
    delete ui;
}

void CoilDisplay::Setup(int p, int c, QString OffIndicator, QString OnIndicator, int IndicatorSize, QString OffText, QString OnText, int FontSize)
{
    this->p = p;
    this->c = c;
    C = &P[p].C[c];
    this->OffIndicator = OffIndicator;
    this->OnIndicator = OnIndicator;
    this->OffText = OffText;
    this->OnText = OnText;
    ui->label_Indicator->setVisible(OffIndicator.length() or OnIndicator.length());
    ui->label_Indicator->setMinimumSize(IndicatorSize, IndicatorSize);
    ui->label_Indicator->setMaximumSize(IndicatorSize, IndicatorSize);
    ui->label_Text->setVisible(OffText.length() or OnText.length());
    QFont Font = this->font();
    Font.setBold(true);
    Font.setPointSize(FontSize);
    ui->label_Text->setFont(Font);
    Refresh();
}

void CoilDisplay::Refresh()
{
    if(!C->Name.length())
    {
        SetText("?");
        return;
    }
    this->setToolTip(
        "Name: " + C->Name +
        "   Value:" + QString::number(C->Value) +
        "   Address: P[" + QString::number(p) + "].C[" + QString::number(c) + "]" +
        "   Page: " + C->Page +
        "   " + C->Description );
    if(C->Value)
    {
        ui->label_Indicator->setPixmap(QPixmap(":/Pix/Icons/indicator-" + OnIndicator + ".png"));
        ui->label_Text->setText(OnText);
    }
    else
    {
        ui->label_Indicator->setPixmap(QPixmap(":/Pix/Icons/indicator-" + OffIndicator + ".png"));
        ui->label_Text->setText(OffText);
    }
}

void CoilDisplay::SetText(QString Text)
{
    ui->label_Indicator->setVisible(false);
    ui->label_Text->setVisible(true);
    ui->label_Text->setText(Text);
    this->setToolTip("");
}
