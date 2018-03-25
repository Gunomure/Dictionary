#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datarepository.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void showData2TableView();
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    //void on_pushButton_2_clicked();
    
    /* К СЛОТу по редактировнию записи
     * добавляем СЛОТ по удалению записи.
     * Также добавляем СЛОТ для обработки вызова контекстного меню
     * */
    void slotEditRecord();
    void slotRemoveRecord();
    void slotCustomMenuRequested(QPoint pos);
    
    void on_radioButton_clicked(bool checked);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::MainWindow *ui;
    DataRepository *dataRepository;
    bool addedMode;
    Container *container2Change;
};

#endif // MAINWINDOW_H
