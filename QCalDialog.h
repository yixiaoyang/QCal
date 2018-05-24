#ifndef QCALDIALOG_H
#define QCALDIALOG_H

#include <QDialog>
#include "CalEngine.h"

namespace Ui {
class QCalDialog;
}

class QCalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QCalDialog(QWidget *parent = 0);
    ~QCalDialog();

private:
    CalEngine engine;
    bool entered;

    void input(char ch);
    void pushButton_num_clicked(int i);
private slots:
    void on_pushButton_dot_clicked();
    void on_pushButton_enter_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_minus_clicked();
    void on_pushButton_pow_clicked();
    void on_pushButton_div_clicked();

    void on_pushButton0_clicked();
    void on_pushButton1_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();
    void on_pushButton4_clicked();
    void on_pushButton5_clicked();
    void on_pushButton6_clicked();
    void on_pushButton7_clicked();
    void on_pushButton8_clicked();
    void on_pushButton9_clicked();
    void on_pushButton_leftQuote_clicked();
    void on_pushButton_rightQuote_clicked();
    void on_pushButton_clear_clicked();

private:
    Ui::QCalDialog *ui;

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // QCALDIALOG_H
