/********************************************************************************
** Form generated from reading ui file 'tag_factory.ui'
**
** Created: Sat Aug 8 13:45:37 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TAG_FACTORY_H
#define UI_TAG_FACTORY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFontComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include "../widget/qtcolorpicker.h"

QT_BEGIN_NAMESPACE

class Ui_TagFactory
{
public:
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QPushButton *m_newTagButton;
    QPushButton *m_newStateButton;
    QTreeWidget *m_tagsTree;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_5;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QFrame *frame_5;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QFontComboBox *m_fontText;
    QLabel *label_5;
    QtColorPicker *m_colorText;
    QLabel *label_7;
    QComboBox *m_sizeText;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *m_boldText;
    QToolButton *m_italicText;
    QToolButton *m_underlineText;
    QToolButton *m_strikeText;
    QLineEdit *m_nameTagOrState;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLabel *label_3;
    QtColorPicker *m_colorItem;
    QSpacerItem *horizontalSpacer;
    QCheckBox *m_withColorBackground;
    QToolButton *toolButton;
    QToolButton *toolButton_6;
    QFrame *line;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *m_okButton;
    QPushButton *m_quitButton;

    void setupUi(QDialog *TagFactory)
    {
        if (TagFactory->objectName().isEmpty())
            TagFactory->setObjectName(QString::fromUtf8("TagFactory"));
        TagFactory->resize(877, 478);
        verticalLayout_3 = new QVBoxLayout(TagFactory);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_4 = new QFrame(TagFactory);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame_4);
        horizontalLayout_2->setMargin(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame = new QFrame(frame_4);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMaximumSize(QSize(400, 16777215));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 1, 0, 0);
        m_newTagButton = new QPushButton(frame);
        m_newTagButton->setObjectName(QString::fromUtf8("m_newTagButton"));

        verticalLayout_2->addWidget(m_newTagButton);

        m_newStateButton = new QPushButton(frame);
        m_newStateButton->setObjectName(QString::fromUtf8("m_newStateButton"));

        verticalLayout_2->addWidget(m_newStateButton);

        m_tagsTree = new QTreeWidget(frame);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        m_tagsTree->setHeaderItem(__qtreewidgetitem);
        m_tagsTree->setObjectName(QString::fromUtf8("m_tagsTree"));

        verticalLayout_2->addWidget(m_tagsTree);

        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Plain);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setMargin(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_5 = new QPushButton(frame_3);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        horizontalLayout_3->addWidget(pushButton_5);

        pushButton_4 = new QPushButton(frame_3);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_3->addWidget(pushButton_4);

        pushButton_3 = new QPushButton(frame_3);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout_3->addWidget(pushButton_3);


        verticalLayout_2->addWidget(frame_3);


        horizontalLayout_2->addWidget(frame);

        frame_2 = new QFrame(frame_4);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy);
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Plain);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame_5 = new QFrame(frame_2);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_5);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(frame_5);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(frame_5);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        m_fontText = new QFontComboBox(groupBox_2);
        m_fontText->setObjectName(QString::fromUtf8("m_fontText"));

        gridLayout->addWidget(m_fontText, 3, 1, 1, 5);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        m_colorText = new QtColorPicker(groupBox_2);
        m_colorText->setObjectName(QString::fromUtf8("m_colorText"));

        gridLayout->addWidget(m_colorText, 4, 1, 1, 5);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        m_sizeText = new QComboBox(groupBox_2);
        m_sizeText->setObjectName(QString::fromUtf8("m_sizeText"));

        gridLayout->addWidget(m_sizeText, 5, 1, 1, 3);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 6, 1, 1);

        m_boldText = new QToolButton(groupBox_2);
        m_boldText->setObjectName(QString::fromUtf8("m_boldText"));
        m_boldText->setCheckable(true);

        gridLayout->addWidget(m_boldText, 1, 1, 1, 1);

        m_italicText = new QToolButton(groupBox_2);
        m_italicText->setObjectName(QString::fromUtf8("m_italicText"));
        m_italicText->setCheckable(true);

        gridLayout->addWidget(m_italicText, 1, 2, 1, 1);

        m_underlineText = new QToolButton(groupBox_2);
        m_underlineText->setObjectName(QString::fromUtf8("m_underlineText"));
        m_underlineText->setCheckable(true);

        gridLayout->addWidget(m_underlineText, 1, 3, 1, 1);

        m_strikeText = new QToolButton(groupBox_2);
        m_strikeText->setObjectName(QString::fromUtf8("m_strikeText"));
        m_strikeText->setCheckable(true);

        gridLayout->addWidget(m_strikeText, 1, 4, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 6, 0, 1, 3);

        m_nameTagOrState = new QLineEdit(frame_5);
        m_nameTagOrState->setObjectName(QString::fromUtf8("m_nameTagOrState"));

        gridLayout_2->addWidget(m_nameTagOrState, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 7, 0, 1, 1);

        groupBox = new QGroupBox(frame_5);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 1, 0, 1, 1);

        m_colorItem = new QtColorPicker(groupBox);
        m_colorItem->setObjectName(QString::fromUtf8("m_colorItem"));
        m_colorItem->setEnabled(false);

        gridLayout_3->addWidget(m_colorItem, 1, 2, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 4, 1, 1);

        m_withColorBackground = new QCheckBox(groupBox);
        m_withColorBackground->setObjectName(QString::fromUtf8("m_withColorBackground"));

        gridLayout_3->addWidget(m_withColorBackground, 1, 1, 1, 1);

        toolButton = new QToolButton(groupBox);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        gridLayout_3->addWidget(toolButton, 0, 1, 1, 1);

        toolButton_6 = new QToolButton(groupBox);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));

        gridLayout_3->addWidget(toolButton_6, 0, 2, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 0, 1, 4);


        verticalLayout->addWidget(frame_5);


        horizontalLayout_2->addWidget(frame_2);


        verticalLayout_3->addWidget(frame_4);

        line = new QFrame(TagFactory);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        frame_6 = new QFrame(TagFactory);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::NoFrame);
        frame_6->setFrameShadow(QFrame::Plain);
        horizontalLayout = new QHBoxLayout(frame_6);
        horizontalLayout->setMargin(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        m_okButton = new QPushButton(frame_6);
        m_okButton->setObjectName(QString::fromUtf8("m_okButton"));

        horizontalLayout->addWidget(m_okButton);

        m_quitButton = new QPushButton(frame_6);
        m_quitButton->setObjectName(QString::fromUtf8("m_quitButton"));

        horizontalLayout->addWidget(m_quitButton);


        verticalLayout_3->addWidget(frame_6);


        retranslateUi(TagFactory);

        QMetaObject::connectSlotsByName(TagFactory);
    } // setupUi

    void retranslateUi(QDialog *TagFactory)
    {
        TagFactory->setWindowTitle(QApplication::translate("TagFactory", "Dialog", 0, QApplication::UnicodeUTF8));
        m_newTagButton->setText(QApplication::translate("TagFactory", "New Tag", 0, QApplication::UnicodeUTF8));
        m_newStateButton->setText(QApplication::translate("TagFactory", "New State", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("TagFactory", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("TagFactory", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("TagFactory", "PushButton", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TagFactory", "Name:", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("TagFactory", "Fonts", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TagFactory", "Font:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TagFactory", "Color:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("TagFactory", "Size", 0, QApplication::UnicodeUTF8));
        m_sizeText->clear();
        m_sizeText->insertItems(0, QStringList()
         << QApplication::translate("TagFactory", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "11", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "12", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "14", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "18", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "22", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "24", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "26", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "28", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "36", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TagFactory", "48", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("TagFactory", "Text:", 0, QApplication::UnicodeUTF8));
        m_boldText->setText(QApplication::translate("TagFactory", "...", 0, QApplication::UnicodeUTF8));
        m_italicText->setText(QApplication::translate("TagFactory", "...", 0, QApplication::UnicodeUTF8));
        m_underlineText->setText(QApplication::translate("TagFactory", "...", 0, QApplication::UnicodeUTF8));
        m_strikeText->setText(QApplication::translate("TagFactory", "...", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TagFactory", "Appareance", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TagFactory", "Emblem:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TagFactory", "Background:", 0, QApplication::UnicodeUTF8));
        m_withColorBackground->setText(QString());
        toolButton->setText(QApplication::translate("TagFactory", "...", 0, QApplication::UnicodeUTF8));
        toolButton_6->setText(QApplication::translate("TagFactory", "...", 0, QApplication::UnicodeUTF8));
        m_okButton->setText(QApplication::translate("TagFactory", "Ok", 0, QApplication::UnicodeUTF8));
        m_quitButton->setText(QApplication::translate("TagFactory", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TagFactory);
    } // retranslateUi

};

namespace Ui {
    class TagFactory: public Ui_TagFactory {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAG_FACTORY_H
