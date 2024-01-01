#include "widget.h"
#include "./ui_widget.h"
#include <QCompleter>

// https://blog.csdn.net/yaq_30401/article/details/133697659

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QStringList wordList;
    wordList << "出仓" << "入仓" << "获取TIP头" << "移除TIP头" << "血液预稀释" << "尿液预稀释" << "血液混匀加样" << "尿液混匀加样" << "模块检测";
    ui->comboBox->addItems(wordList);

    ui->comboBox->setEditable(true);                        //使能comboBox编辑功能
    QCompleter *pCompleter = new QCompleter(wordList, this);//创建自动填充实例
    pCompleter->setCaseSensitivity(Qt::CaseInsensitive);    //大小写设置：不区分大小写
    pCompleter->setFilterMode(Qt::MatchContains);           //匹配类型：内容匹配
    ui->comboBox->setCompleter(pCompleter);
}

Widget::~Widget()
{
    delete ui;
}
