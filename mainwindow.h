#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 文件菜单
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void exitApp();

    // 编辑菜单
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void selectAll();
    void find();
    void replace();

    // 格式菜单
    void selectFont();
    // QAction::triggered 信号会自动传递一个 bool 参数
    void toggleWordWrap(bool checked);

    // 帮助菜单
    void about();

    // 文本变化
    void textChanged();

private:
    // UI 组件
    QTextEdit *textEdit;

    // 数据成员
    QString currentFile;
    bool isModified;

    // 私有方法
    void createUI();
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();

    bool save(const QString &fileName);
    bool load(const QString &fileName);
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    void updateTitle();
};

#endif // MAINWINDOW_H
