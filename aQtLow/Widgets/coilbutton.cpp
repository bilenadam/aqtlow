#include "coilbutton.h"
#include "ui_coilbutton.h"

CoilButton::CoilButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoilButton)
{
    ui->setupUi(this);
}

CoilButton::~CoilButton()
{
    delete ui;
}

void CoilButton::on_pushButton_clicked()
{
    switch(TOB)
    {
    case SET:
        C->Set();
        break;
    case RST:
        C->Reset();
        break;
    case TGL:
        C->Toggle();
        break;
    }
}

void CoilButton::Setup(int p, int c, TypeOfButton TOB, QString Text, int IconSize, int FontSize)
{
    this->p = p;
    this->c = c;
    C = &P[p].C[c];
    this->TOB = TOB;
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

void CoilButton::SetText(QString Text)
{
    QIcon Icon;
    Icon.addFile("");
    ui->pushButton->setIcon(Icon);
    ui->pushButton->setText(Text);
    ui->pushButton->setToolTip("");
    ui->pushButton->setEnabled(false);
}

void CoilButton::Refresh()
{
    if(!C->Name.length())
    {
        SetText("?");
        return;
    }
    QString Tip;
    QIcon Icon;
    bool Disable = false;
    switch(TOB)
    {
    case SET:
        Icon.addFile(":/Pix/Icons/dialog-ok-2.png");
        ui->pushButton->setIcon(Icon);
        ui->pushButton->setText(Text);
        Tip = "Set ON";
        if(C->Value) Disable = true;
        break;
    case RST:
        Icon.addFile(":/Pix/Icons/dialog-cancel-2.png");
        ui->pushButton->setIcon(Icon);
        ui->pushButton->setText(Text);
        Tip = "Reset OFF";
        if(!C->Value) Disable = true;
        break;
    case TGL:
        Icon.addFile(":/Pix/Icons/system-switch-user-3.png");
        ui->pushButton->setIcon(Icon);
        ui->pushButton->setText(Text);
        Tip = "Toggle";
        break;
    }
    ui->pushButton->setEnabled(!Disable && !C->WriteRequest);
    ui->pushButton->setToolTip(
        Tip +
        "   Name: " + C->Name +
        "   Address: P[" + QString::number(p) + "].C[" + QString::number(c) + "]" +
        "   Page: " + C->Page +
        "   " + C->Description );
}
