#pragma once

#define OBJ_NAME(name) QStringLiteral(#name)
#define POS_WIDGET(name,base,parent)  \
    auto name = new PositionableWidget<base>(parent); \
    name->setObjectName(QStringLiteral(#name))
#define POS_BUTTON(name,parent,text) \
    POS_WIDGET(name,QPushButton,parent); \
    name->setText(QStringLiteral(text))

#define POS_LABEL(name,parent,text) \
    POS_WIDGET(name,QLabel,parent); \
    name->setText(QStringLiteral(text))
