#include "wobuy.h"
#include "ui_wobuy.h"
#include "CustomFunc.h"

#include <QtSql>

WOBuy::WOBuy(QWidget *parent) : QDialog(parent), ui(new Ui::WOBuy) {
    ui->setupUi(this);
}

WOBuy::~WOBuy() {
    ClearInputFields();
    delete db;
    delete ui;
}

void WOBuy::SetDatabase(QSqlDatabase *db) {
    this->db = db;
    db->open();

    // Product table
    QSqlQueryModel *product_model = new QSqlQueryModel;
    product_model->setQuery("SELECT name, ean, amount FROM debug_product");

    product_model->setHeaderData(0, Qt::Horizontal, tr("NOME"));
    product_model->setHeaderData(1, Qt::Horizontal, tr("EAN"));
    product_model->setHeaderData(2, Qt::Horizontal, tr("QTD."));

    ui->product_table->setModel(product_model);

    // Client table
    QSqlQueryModel *client_model = new QSqlQueryModel;
    client_model->setQuery("SELECT name, company FROM debug_client");

    client_model->setHeaderData(0, Qt::Horizontal, tr("NOME"));
    client_model->setHeaderData(1, Qt::Horizontal, tr("EMPRESA"));

    ui->client_table->setModel(client_model);

    db->close();
}

void WOBuy::on_product_ean_textChanged() {
    if (ui->product_ean->toPlainText().contains(RE_NOT_NUMDOTMINUS)) {
        QString newStr = ui->product_ean->toPlainText();
        newStr.remove(RE_NOT_NUMDOTMINUS);
        ui->product_ean->setPlainText(newStr);

        QTextCursor cursor(ui->product_ean->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_ean->setTextCursor(cursor);
    }

    if (ui->product_ean->toPlainText().length() > 13) {
        QString newStr = ui->product_ean->toPlainText();
        newStr.chop(ui->product_ean->toPlainText().length() - 13);
        ui->product_ean->setPlainText(newStr);

        QTextCursor cursor(ui->product_ean->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_ean->setTextCursor(cursor);
    }
}

void WOBuy::ClearInputFields() {
    ui->product_name->clear();
    ui->product_amount->clear();
    ui->product_ean->clear();

    ui->client_name->clear();
    ui->client_company->clear();
}

void WOBuy::on_product_amount_textChanged() {
    if (ui->product_amount->toPlainText().contains(RE_NOT_NUMDOT)) {
        QString newStr = ui->product_amount->toPlainText();
        newStr.remove(RE_NOT_NUMDOT);
        ui->product_amount->setPlainText(newStr);

        QTextCursor cursor(ui->product_amount->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_amount->setTextCursor(cursor);
    }
}

void WOBuy::on_product_table_doubleClicked(const QModelIndex &index) {
    QAbstractItemModel *model = ui->product_table->model();
    ui->product_name->setPlainText(model->index(index.row(), 0).data().toString());
    ui->product_ean->setPlainText(model->index(index.row(), 1).data().toString());
    // ui->product_amount->setPlainText(model->index(index.row(), 2).data().toString());
}

void WOBuy::on_client_table_doubleClicked(const QModelIndex &index) {
    QAbstractItemModel *model = ui->client_table->model();
    ui->client_name->setPlainText(model->index(index.row(), 0).data().toString());
    ui->client_company->setPlainText(model->index(index.row(), 1).data().toString());
}

void WOBuy::on_buy_cancel_clicked() {
    ClearInputFields();
    WOBuy::close();
}

void WOBuy::on_buy_confirm_clicked() {
    db->open();

    if (ui->product_name->toPlainText() == "") {
        SimpleMessageBox("Nome vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    if (ui->product_ean->toPlainText() == "") {
        SimpleMessageBox("EAN vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    if (ui->product_amount->toPlainText() == "") {
        SimpleMessageBox("Quantidade vazia.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    if (ui->client_name->toPlainText() == "") {
        SimpleMessageBox("Nome vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    if (ui->client_company->toPlainText() == "") {
        SimpleMessageBox("Fornecedor vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    QSqlQuery qry;

    qry.prepare("SELECT cost FROM debug_product WHERE ean = ?");
    qry.addBindValue(ui->product_ean->toPlainText());
    qry.exec();

    qry.next();
    double product_cost = qry.value(0).toDouble();

    qry.prepare("INSERT INTO debug_workorder (product, amount, client, company, wo, reason, ean, revenue)"
                "VALUES (?, ?, ?, ?, 0, 'Compra', ?, ?)");

    qry.addBindValue(ui->product_name->toPlainText().toUpper());
    qry.addBindValue(ui->product_amount->toPlainText().toInt());
    qry.addBindValue(ui->client_name->toPlainText());
    qry.addBindValue(ui->client_company->toPlainText());
    qry.addBindValue(ui->product_ean->toPlainText());
    qry.addBindValue(ui->product_amount->toPlainText().toDouble() * product_cost * -1);


    qry.exec();

    db->close();
    ClearInputFields();
    WOBuy::close();
}
