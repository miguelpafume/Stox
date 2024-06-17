#pragma once

#include <QMessageBox>
#include <QRegularExpression>

static QRegularExpression RE_NOT_NUMDOTMINUS("[^\\d]");
static QRegularExpression RE_NOT_NUMDOT("[^\\d|-]");
static QRegularExpression RE_NOT_NUM("[^\\.|\\d|,]");

void SimpleMessageBox(QString &&str, const short unsigned int i);
