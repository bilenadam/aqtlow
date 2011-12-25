#include "registerbutton.h"
#include "ui_registerbutton.h"

RegisterButton::RegisterButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterButton)
{
    ui->setupUi(this);
}

RegisterButton::~RegisterButton()
{
    delete ui;
}

void RegisterButton::on_pushButton_clicked()
{
    switch(TOB)
    {
    case INC:
        R->Inc(Value);
        break;
    case DEC:
        R->Dec(Value);
        break;
    case SET:
        R->Set(Value);
        break;
    }
}

void RegisterButton::Setup(int p, int r, TypeOfButton TOB, double Value, QString Text, int IconSize, int FontSize)
{
    this->p = p;
    this->r = r;
    R = &P[p].R[r];
    this->TOB = TOB;
    this->Value = Value;
    this->Text = Text;
    QSize Size = ui->pushButton->iconSize();
    Size.setWidth(IconSize);
    Size.setHeight(IconSize);
    ui->pushButton->setIconSize(Size);
    this->setMinimumHeight(IconSize * 1.5);
    this->setMinimumWidth(IconSize * 1.5);
    QFont Font = this->font();
    Font.setBold(true);
    Font.setPointSize(FontSize);
    ui->pushButton->setFont(Font);
    Refresh();
}

void RegisterButton::Refresh()
{
    if(!R->Name.length())
    {
        SetText("?");
        return;
    }
    if(R->IntfcMin == R->IntfcMax)
    {
        SetText("*");
        return;
    }
    QString Tip;
    QIcon Icon;
    bool Disable = false;
    switch(TOB)
    {
    case INC:        
        Icon.addFile(":/Pix/Icons/arrow-up.png");
        ui->pushButton->setIcon(Icon);
        ui->pushButton->setText(Text);
        Tip = "+" + QString::number(Value);
        if(R->Value == R->IntfcMax) Disable = true;
        break;
    case DEC:
        Icon.addFile(":/Pix/Icons/arrow-down.png");
        ui->pushButton->setIcon(Icon);
        ui->pushButton->setText(Text);
        Tip = "-" + QString::number(Value);
        if(R->Value == R->IntfcMin) Disable = true;
        break;
    case SET:
        ui->pushButton->setIcon(Icon);
        ui->pushButton->setText(Text);
        Tip = "Set to " + QString::number(Value);
        if(R->Value == Value) Disable = true;
        break;
    }
    ui->pushButton->setEnabled(!Disable && !R->WriteRequest);
    ui->pushButton->setToolTip(
        Tip +
        "   Name: " + R->Name +
        "   Address: P[" + QString::number(p) + "].R[" + QString::number(r) + "]" +
        "   Page: " + R->Page +
        "   " + R->Description +
        "   Low:" + QString::number(R->IntfcLow) +
        " High:" + QString::number(R->IntfcHigh) +
        " Min:" + QString::number(R->IntfcMin) +
        " Max:" + QString::number(R->IntfcMax) );
}

void RegisterButton::SetText(QString Text)
{
    QIcon Icon;
    Icon.addFile("");
    ui->pushButton->setIcon(Icon);
    ui->pushButton->setText(Text);
    ui->pushButton->setToolTip("");
    ui->pushButton->setEnabled(false);
}
