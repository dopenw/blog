## 2.创建对话框

## 子类化QDialog

dialog.h:
```c++
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>


class Dialog : public QDialog
{
    Q_OBJECT //对于所有定义了信号和槽的类，再类定义开始处的Q_OB所JECT宏都是必需的
public:
    explicit Dialog(QWidget * parent = 0);
    ~Dialog();

signals:
    void findNext(const QString,Qt::CaseSensitivity cs);
    void findPrevious(const QString,Qt::CaseSensitivity cs);

private slots:
    void findClicked();
    void enableFindButton(const QString &text);

private:
    QLabel * label;
    QLineEdit * linEdit;
    QCheckBox * caseCheckBox;
    QCheckBox * backwardCheckBox;
    QPushButton * findButton;
    QPushButton * closeButton;
};

#endif // DIALOG_H
```

dialog.cpp:
```c++
#include "dialog.h"
#include <QtGui> //该头文件包含了QT GUI类的定义
#include <QHBoxLayout>
#include <QVBoxLayout>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
   label=new QLabel(tr("Find &what"));
   linEdit=new QLineEdit;
   label->setBuddy(linEdit);

   caseCheckBox=new QCheckBox(tr("Match &case"));
   //tr（）函数可以把他们翻译成其他语言的标记
   // & 来表示快捷键 Alt + 字母(例如c)
   backwardCheckBox=new QCheckBox(tr("search &Backward"));

   findButton=new QPushButton(tr("&Find"));
   findButton->setDefault(true);
   //使findbutton成为按下Enter键时能够按下对应的按钮
   findButton->setEnabled(false);

   closeButton=new QPushButton(tr("close"));

   connect(linEdit,SIGNAL(textChanged(QString)),
           this,SLOT(enableFindButton(const QString &)));

   connect(findButton,SIGNAL(clicked(bool)),this,SLOT(findClicked()));
   connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));

   QHBoxLayout * topLeftLayout=new QHBoxLayout;
   topLeftLayout->addWidget(label);
   topLeftLayout->addWidget(linEdit);

   QVBoxLayout * leftLayout=new QVBoxLayout;
   leftLayout->addLayout(topLeftLayout);
   leftLayout->addWidget(caseCheckBox);
   leftLayout->addWidget(backwardCheckBox);

   QVBoxLayout * rightLayout=new QVBoxLayout;
   rightLayout->addWidget(findButton);
   rightLayout->addWidget(closeButton);
   rightLayout->addStretch();

   QHBoxLayout * mainLayout=new QHBoxLayout;
   mainLayout->addLayout(leftLayout);
   mainLayout->addLayout(rightLayout);
   setLayout(mainLayout);

   setWindowTitle(tr("Find"));
   setFixedHeight(sizeHint().height());
   //QWidget::sizeHint()函数可以返回一个窗口不见所理想的尺寸大小
}

void Dialog::findClicked()
{
    QString text=linEdit->text();
    Qt::CaseSensitivity cs=caseCheckBox->isChecked() ? Qt::CaseSensitive :
                                                       Qt::CaseInsensitive;
    if (backwardCheckBox->isChecked())
    {
        emit findPrevious(text,cs);
        //发射信号
    }
    else
    {
        emit findNext(text,cs);
    }


}

void Dialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}

Dialog::~Dialog()
{
//按照常理来讲，new了之后，我们需要delete.
//但是在QT中，这个并不是必需的，
//因为QT会在删除父对象的时候自动删除其所属的所有子对象
}
```

main.cpp:
```c++
#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}

```

可以使用快捷键Alt+c等来触发相应的行为。可以通过Tab键来遍历这些窗口部件。默认的Tab键顺序就是创建窗口部件时的顺序。要改变这个键顺序，可以使用QWidget::setTabOrder()函数

fedora 27显示效果：
![](../images/2_creat_dialog_201711192228_1.png)


[上一级](base.md)
[上一篇](1_hello_qt.md)
