/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FtpListItem.hpp"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ImageView>
#include <bb/cascades/Image>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

FtpListItem::FtpListItem(Container *parent)
    : CustomControl(parent)
{
    Container *contents = new Container();
    StackLayout *contentsLayout = new StackLayout();
    contentsLayout->setLayoutDirection(LayoutDirection::LeftToRight);
    contents->setLayout(contentsLayout);

    // A background Container that will hold a background image and a item content Container.
    Container *backgroundContainer = new Container();
    backgroundContainer->setPreferredHeight(141);
    DockLayout *backgroundLayout = new DockLayout();
    backgroundContainer->setLayout(backgroundLayout);
    backgroundContainer->setLayoutProperties(DockLayoutProperties::create().horizontal(HorizontalAlignment::Fill));

    m_highlighContainer = new ImageView();
    m_highlighContainer->setImage(Image(QUrl("asset:///images/highlighted.png")));
    m_highlighContainer->setOpacity(0.0);

    m_itemImage = new ImageView();
    m_itemImage->setLayoutProperties(StackLayoutProperties::create());
    m_itemImage->setMinWidth(128.0f);
    m_itemImage->setMinHeight(128.0f);

    Container *textContents = new Container();
    m_itemTitle = Label::create();
    m_itemTitle->textStyle()->setBase(SystemDefaults::TextStyles::bigText());
    m_itemTitle->textStyle()->setColor(Color::Black);

    m_itemDescription = Label::create();
    m_itemDescription->textStyle()->setBase(SystemDefaults::TextStyles::smallText());
    m_itemDescription->textStyle()->setColor(Color::Black);
    m_itemDescription->textStyle()->setSize(25.0f);

    contents->add(m_itemImage);
    contents->add(textContents);
    textContents->add(m_itemTitle);
    textContents->add(m_itemDescription);

    backgroundContainer->add(m_highlighContainer);
    backgroundContainer->add(contents);

    setRoot(backgroundContainer);
}

void FtpListItem::setImage(const Image &image)
{
    m_itemImage->setImageSource(image.source());
}

void FtpListItem::setTitleText(const QString &title)
{
    m_itemTitle->setText(title);
}

void FtpListItem::setStatusText(const QString &status)
{
    m_itemDescription->setText(status);
}

void FtpListItem::select(bool select)
{
    // When an item is selected show the colored highlight Container-�,
    if (select) {
        m_highlighContainer->setOpacity(1.0f);
    } else {
        m_highlighContainer->setOpacity(0.0f);
    }
}

void FtpListItem::reset(bool selected, bool activated)
{
    Q_UNUSED(activated);

    // Since items are recycled the reset function is where we have
    // to make sure that item state, defined by the arguments, is correct.
    select(selected);
}

void FtpListItem::activate(bool activate)
{
    // There is no special activate state, select and activated looks the same.
    select(activate);
}

