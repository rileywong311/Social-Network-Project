#ifndef SOCIALNETWORKUI_H
#define SOCIALNETWORKUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SocialNetworkUI; }
QT_END_NAMESPACE

class SocialNetworkUI : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkUI(QWidget *parent = nullptr);
    ~SocialNetworkUI();

private:
    Ui::SocialNetworkUI *ui;
};
#endif // SOCIALNETWORKUI_H
