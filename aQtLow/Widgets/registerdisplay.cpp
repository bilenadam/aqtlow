#include "registerdisplay.h"
#include "ui_registerdisplay.h"

RegisterDisplay::RegisterDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterDisplay)
{
    ui->setupUi(this);
}

RegisterDisplay::~RegisterDisplay()
{
    delete ui;
}

void RegisterDisplay::Setup(int p, int r, bool AllowEdit, int FontSize, int Decimals)
{
    this->p = p;
    this->r = r;
    R = &P[p].R[r];
    this->AllowEdit = AllowEdit;
    QFont Font = this->font();
    Font.setBold(true);
    Font.setPointSize(FontSize);
    ui->lineEdit->setFont(Font);
    this->Decimals = Decimals;
    Refresh();
}

void RegisterDisplay::Refresh()
{
    if(!R->Name.length())
    {
        SetText("?");
        return;
    }
    QString Tip =
        "Name: " + R->Name +
        "   Address: P[" + QString::number(p) + "].R[" + QString::number(r) + "]" +
        "   Page: " + R->Page +
        "   " + R->Description +
        "   Low:" + QString::number(R->IntfcLow) +
        " High:" + QString::number(R->IntfcHigh);
    if(R->IntfcMax > R->IntfcMin)
    {
        Tip.append(
        " Min:" + QString::number(R->IntfcMin) +
        " Max:" + QString::number(R->IntfcMax) );
    }
    ui->lineEdit->setToolTip(Tip);
    if(!AllowEdit || R->IntfcMin==R->IntfcMax) ui->lineEdit->clearFocus();
    if(ui->lineEdit->hasFocus())
    {
        //If editing and the string is not valid as a number,
        // rewrite it without the units.
        bool ok;
        double num = ui->lineEdit->text().toDouble(&ok);
        if(!ok)
        {
            ui->lineEdit->setText(R->TextRounded(Decimals));
            ui->lineEdit->selectAll();
            num++; //UnusedVariableInfo==Tiresome
        }
    }
    else
    {
        if(!R->WriteRequest) ui->lineEdit->setText(R->TextRounded(Decimals)+" "+R->Units);
    }
}

void RegisterDisplay::SetText(QString Text)
{
    ui->lineEdit->setText(Text);
    ui->lineEdit->setToolTip("");
    AllowEdit = false;
}

void RegisterDisplay::on_lineEdit_editingFinished()
{
    if(AllowEdit) R->Set(ui->lineEdit->text().toDouble());
}
