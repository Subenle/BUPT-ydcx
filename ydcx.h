#ifndef YDCX_H
#define YDCX_H

#include <QDialog>
class QNetworkReply;
class QLabel;
class QLineEdit;
class QPushButton;
class QNetworkAccessManager;
namespace Ui {
class ydcx;
}

class ydcx : public QDialog
{
    Q_OBJECT

public:
    explicit ydcx(QWidget *parent = 0);
    ~ydcx();

protected slots:
    void bt_search_clicked();
    void request_website();
    void replyFinished(QNetworkReply *);
    void localAnalyze();

private:
    Ui::ydcx *ui;
    QNetworkAccessManager *manager;
};

#endif // YDCX_H
