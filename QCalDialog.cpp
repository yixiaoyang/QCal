#include "QCalDialog.h"
#include "ui_QCalDialog.h"

#include <QPushButton>
#include <QLineEdit>
#include <QKeyEvent>

#define STYLE_ERR   "QLineEdit{color:red;}"
#define STYLE_OK    "QLineEdit{color:#888;}"

QCalDialog::QCalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCalDialog)
{
    ui->setupUi(this);
    entered = false;

    QObject* objects[] = {
        ui->pushButton0,
        ui->pushButton1,
        ui->pushButton2,
        ui->pushButton3,
        ui->pushButton4,
        ui->pushButton5,
        ui->pushButton6,
        ui->pushButton7,
        ui->pushButton8,
        ui->pushButton9,
        ui->pushButton_plus,
        ui->pushButton_minus,
        ui->pushButton_pow,
        ui->pushButton_div,
        ui->pushButton_enter,
        ui->pushButton_clear,
        ui->lineEdit
    };
    for(QObject* obj : objects){
        obj->installEventFilter(this);
    }
}

QCalDialog::~QCalDialog()
{
    delete ui;
}

void QCalDialog::input(char ch)
{
    if(entered){
        on_pushButton_clear_clicked();
        entered = false;
    }
    if(engine.input(ch)){
        ui->lineEdit->setStyleSheet(STYLE_OK);
    }else{
        ui->lineEdit->setStyleSheet(STYLE_ERR);
    }
    ui->lineEdit->setText(engine.getExpression());
}

void QCalDialog::on_pushButton_enter_clicked()
{
    input('=');
    entered = true;
    if(engine.cal()){
        ui->lineEdit->setStyleSheet(STYLE_OK);
    }else{
        ui->lineEdit->setStyleSheet(STYLE_ERR);
    }
    ui->lineEdit->setText(engine.getExpression()+engine.getResult().token);
}

void QCalDialog::on_pushButton_dot_clicked(){           input('.');}
void QCalDialog::on_pushButton_plus_clicked(){          input('+');}
void QCalDialog::on_pushButton_minus_clicked(){         input('-');}
void QCalDialog::on_pushButton_pow_clicked(){           input('*');}
void QCalDialog::on_pushButton_div_clicked(){           input('/');}
void QCalDialog::on_pushButton_leftQuote_clicked(){     input('(');}
void QCalDialog::on_pushButton_rightQuote_clicked(){    input(')');}

void QCalDialog::pushButton_num_clicked(int i){ input('0'+i);}
void QCalDialog::on_pushButton0_clicked(){pushButton_num_clicked(0);}
void QCalDialog::on_pushButton1_clicked(){pushButton_num_clicked(1);}
void QCalDialog::on_pushButton2_clicked(){pushButton_num_clicked(2);}
void QCalDialog::on_pushButton3_clicked(){pushButton_num_clicked(3);}
void QCalDialog::on_pushButton4_clicked(){pushButton_num_clicked(4);}
void QCalDialog::on_pushButton5_clicked(){pushButton_num_clicked(5);}
void QCalDialog::on_pushButton6_clicked(){pushButton_num_clicked(6);}
void QCalDialog::on_pushButton7_clicked(){pushButton_num_clicked(7);}
void QCalDialog::on_pushButton8_clicked(){pushButton_num_clicked(8);}
void QCalDialog::on_pushButton9_clicked(){pushButton_num_clicked(9);}

void QCalDialog::on_pushButton_clear_clicked()
{
    engine.clear();
    ui->lineEdit->setStyleSheet(STYLE_OK);
    ui->lineEdit->clear();
#if TEST
    QString calStr("9+(3-1)*3+10/2");
    //QString calStr("5+((1+2)*4)-3");

    for(int i = 0 ; i < calStr.size(); i++){
        input(calStr.at(i).toLatin1());
    }
    ui->lineEdit->setText(calStr);
#endif
}


bool QCalDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int keys[] = {
            Qt::Key_0,
            Qt::Key_1,
            Qt::Key_2,
            Qt::Key_3,
            Qt::Key_4,
            Qt::Key_5,
            Qt::Key_6,
            Qt::Key_7,
            Qt::Key_8,
            Qt::Key_9,

            Qt::Key_Plus,
            Qt::Key_Minus,
            Qt::Key_Asterisk,
            Qt::Key_Slash,

            Qt::Key_Enter,
            Qt::Key_Return,
            Qt::Key_Backspace,
            Qt::Key_Period,

            Qt::Key_ParenRight,
            Qt::Key_ParenLeft
        };
        for(uint i = 0 ; i < sizeof(keys)/sizeof(int); i++){
            if(keys[i] == keyEvent->key()){
                QPushButton* buttons[] = {
                    ui->pushButton0,
                    ui->pushButton1,
                    ui->pushButton2,
                    ui->pushButton3,
                    ui->pushButton4,
                    ui->pushButton5,
                    ui->pushButton6,
                    ui->pushButton7,
                    ui->pushButton8,
                    ui->pushButton9,

                    ui->pushButton_plus,
                    ui->pushButton_minus,
                    ui->pushButton_pow,
                    ui->pushButton_div,

                    ui->pushButton_enter,
                    ui->pushButton_enter,
                    ui->pushButton_clear,
                    ui->pushButton_dot,

                    ui->pushButton_rightQuote,
                    ui->pushButton_leftQuote
                };
                buttons[i]->click();
                return true;
            }
        }
    }
    return QDialog::eventFilter(watched,event);
}
