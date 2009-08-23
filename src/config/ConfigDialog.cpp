/*
 Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#include "ConfigDialog.h"

#include <QtGui>

namespace Config
{

    ConfigDialog::ConfigDialog():
            QDialog()
    {
        contentsWidget = new QListWidget;
        contentsWidget->setViewMode(QListView::IconMode);
        contentsWidget->setIconSize(QSize(96, 84));
        contentsWidget->setMovement(QListView::Static);
        contentsWidget->setMaximumWidth(128);
        contentsWidget->setSpacing(12);

        pagesWidget = new QStackedWidget;
        pagesWidget->addWidget(new QWidget);
        pagesWidget->addWidget(new QWidget);
        pagesWidget->addWidget(new QWidget);

        QPushButton *closeButton = new QPushButton(tr("Close"));

        createIcons();
        contentsWidget->setCurrentRow(0);

        connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

        QHBoxLayout *horizontalLayout = new QHBoxLayout;
        horizontalLayout->addWidget(contentsWidget);
        horizontalLayout->addWidget(pagesWidget, 1);

        QHBoxLayout *buttonsLayout = new QHBoxLayout;
        buttonsLayout->addStretch(1);
        buttonsLayout->addWidget(closeButton);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(horizontalLayout);
        mainLayout->addStretch(1);
        mainLayout->addSpacing(12);
        mainLayout->addLayout(buttonsLayout);
        setLayout(mainLayout);

        setWindowTitle(tr("Config Dialog"));
    }

    void ConfigDialog::createIcons()
    {
        QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
        configButton->setIcon(QIcon(":/images/config.png"));
        configButton->setText(tr("Configuration"));
        configButton->setTextAlignment(Qt::AlignHCenter);
        configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
        updateButton->setIcon(QIcon(":/images/update.png"));
        updateButton->setText(tr("Update"));
        updateButton->setTextAlignment(Qt::AlignHCenter);
        updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
        queryButton->setIcon(QIcon(":/images/query.png"));
        queryButton->setText(tr("Query"));
        queryButton->setTextAlignment(Qt::AlignHCenter);
        queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        connect(contentsWidget,
                SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
                this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
    }

    void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
    {
        if (!current)
            current = previous;

        pagesWidget->setCurrentIndex(contentsWidget->row(current));
    }

}
