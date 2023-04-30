#include "socialnetworkui.h"
#include "ui_socialnetworkui.h"

SocialNetworkUI::SocialNetworkUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkUI)
{
    ui->setupUi(this);
}

SocialNetworkUI::~SocialNetworkUI()
{
    delete ui;
}

