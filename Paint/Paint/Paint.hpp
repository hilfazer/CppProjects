#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Paint.h"

class Paint : public QMainWindow, public Ui::PaintClass
{
    Q_OBJECT

public:
    Paint(QWidget *parent = nullptr);
    ~Paint();

private:
     
};

