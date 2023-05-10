#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "program.h"
#include "parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_cmdLineEdit_editingFinished();
    void on_cmdLineEdit_textEdited();
    void on_btnLoadCode_clicked();
    void on_btnRunCode_clicked();
    void on_btnClearCode_clicked();

private:

    /* some helpers */

    // macro for updating code area
#define UPDATE_CODE { \
    ui->CodeDisplay->setText(QString::fromStdString(program->toString())); \
    ui->treeDisplay->setText(QString::fromStdString(program->toTree())); \
}

    // macro for updating output area
#define UPDATE_OUT(text) { \
    ui->textBrowser->append(text); \
}

    // macro for handling parsing and runtime errors
#define HANDLE(code) \
    try { \
        code \
    } catch(ParseException &e) { \
        UPDATE_CODE \
        ui->textBrowser->append(QString::fromStdString(e.what())); \
    } catch(RuntimeException &e) { \
        UPDATE_CODE \
        ui->textBrowser->append(QString::fromStdString(e.what())); \
    } catch(std::exception &e) { \
        QString error(e.what()); \
        ui->textBrowser->append("unknown error: " + error); \
    }

    // input variables directly or during runtime
    void variableInput(std::vector<token> &tokens);
    // input a new line to the code area
    void lineInput(std::vector<token> &tokens);
    // directly input and execute a statement
    void directInput(std::vector<token> &tokens);

    /* direct commands handler */
    void load();
    void run();
    void clear();
    void help();

private:

    /* variable recorder */
    std::string name;
    bool isRunning;

    Ui::MainWindow *ui;
    Program *program;

};

#endif // MAINWINDOW_H
