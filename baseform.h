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

private slots:
    void on_addButton_clicked();

    void on_changeButton_clicked();

    void on_delButton_clicked();

private:
    Ui::BaseForm *ui;
    void updateTable();
};

#endif // BASEFORM_H
