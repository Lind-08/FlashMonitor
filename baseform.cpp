#include "baseform.h"
#include "ui_baseform.h"

BaseForm::BaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseForm)
{
    ui->setupUi(this);
}

BaseForm::~BaseForm()
{
    delete ui;
}
