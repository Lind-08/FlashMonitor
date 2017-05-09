#ifndef BASEFORM_H
#define BASEFORM_H

#include <QWidget>

namespace Ui {
class BaseForm;
}

class BaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit BaseForm(QWidget *parent = 0);
    ~BaseForm();

private:
    Ui::BaseForm *ui;
};

#endif // BASEFORM_H
