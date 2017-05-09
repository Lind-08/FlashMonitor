#include "baseform.h"
#include "ui_baseform.h"
#include "DB/usb.h"
#include "DB/usbrepository.h"
#include "DB/rule.h"
#include "DB/rulerepository.h"

BaseForm::BaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseForm)
{
    ui->setupUi(this);
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->hideColumn(0);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList({"Название", "Serial", "VID", "PID", "Доступ"}));

    }

    BaseForm::~BaseForm()
    {
        delete ui;
    }

    void BaseForm::on_addButton_clicked()
    {

    }

    void BaseForm::on_changeButton_clicked()
    {

    }

    void BaseForm::on_delButton_clicked()
    {

    }

    void BaseForm::updateTable()
    {
        auto usbRep = UsbRepository::Instance();
        auto ruleRep = RuleRepository::Instance();
        auto usbObjects = usbRep->GetAll();
        auto ruleObjects = ruleRep->GetAll();
    //    for (auto usb: usbObjects)
    //    {
    //        ui->tableWidget->
    //    }
    }
