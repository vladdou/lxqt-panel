/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://lxqt.org
 *
 * Copyright: 2015 LXQt team
 * Authors:
 *   Paulo Lieuthier <paulolieuthier@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "configpluginswidget.h"
#include "ui_configpluginswidget.h"
#include "addplugindialog.h"
#include <HtmlDelegate>
#include "panelpluginsmodel.h"

#include <QPushButton>

ConfigPluginsWidget::ConfigPluginsWidget(LxQtPanel *panel, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ConfigPluginsWidget),
    mPanel(panel)
{
    ui->setupUi(this);

    PanelPluginsModel * plugins = mPanel->mPlugins.data();
    {
        QScopedPointer<QItemSelectionModel> m(ui->listView_plugins->selectionModel());
        QScopedPointer<QAbstractItemDelegate> d(ui->listView_plugins->itemDelegate());
    }
    ui->listView_plugins->setModel(plugins);
    ui->listView_plugins->setItemDelegate(new LxQt::HtmlDelegate(QSize(16, 16), ui->listView_plugins));

    connect(ui->listView_plugins, &QListView::activated, plugins, &PanelPluginsModel::onActivatedIndex);

    connect(ui->pushButton_moveUp, &QToolButton::clicked, plugins, &PanelPluginsModel::onMovePluginUp);
    connect(ui->pushButton_moveDown, &QToolButton::clicked, plugins, &PanelPluginsModel::onMovePluginDown);

    connect(ui->pushButton_addPlugin, &QPushButton::clicked, this, &ConfigPluginsWidget::showAddPluginDialog);
    connect(ui->pushButton_removePlugin, &QToolButton::clicked, plugins, &PanelPluginsModel::onRemovePlugin);

    connect(ui->pushButton_pluginConfig, &QToolButton::clicked, plugins, &PanelPluginsModel::onConfigurePlugin);
}

ConfigPluginsWidget::~ConfigPluginsWidget()
{
    delete ui;
}

void ConfigPluginsWidget::reset()
{

}

void ConfigPluginsWidget::showAddPluginDialog()
{
    if (mAddPluginDialog.isNull())
    {
        mAddPluginDialog.reset(new AddPluginDialog);
        connect(mAddPluginDialog.data(), &AddPluginDialog::pluginSelected,
                mPanel->mPlugins.data(), &PanelPluginsModel::addPlugin);
    }
    mAddPluginDialog->show();
    mAddPluginDialog->raise();
    mAddPluginDialog->activateWindow();
}