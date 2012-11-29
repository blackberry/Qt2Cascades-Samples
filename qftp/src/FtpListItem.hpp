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

#ifndef FTPLISTITEM_HPP
#define FTPLISTITEM_HPP

#include <bb/cascades/CustomControl>
#include <bb/cascades/ListItemListener>

using namespace bb::cascades;

namespace bb {
namespace cascades {
class ImageView;
class Image;
class Label;
class Container;
}
}

/**
 * The FtpListItem class represents one entry in the ListView that shows the FTP directory listing.
 * Like the StandardListItem it provides a title, a status text and an image, however it uses a different
 * graphical representation.
 */
class FtpListItem : public bb::cascades::CustomControl, public ListItemListener
{
    Q_OBJECT

public:
    FtpListItem(Container *parent = 0);

    /**
     * Sets the new title text on this list item to a passed in string.
     *
     *
     * @param titleText The new title text for this list item.
     */
    void setTitleText(const QString &titleText);

    /**
     * Sets the new status text on this list item to a passed in string.
     *
     *
     * @param statusText The new status text for this list item.
     */
    void setStatusText(const QString &statusText);

    /**
     * Sets the new image on this list item.
     *
     * @param image The new image for this list item.
     */
    void setImage(const Image &image);

    /**
     * ListItemListener interface function called when the select state changes.
     *
     * @param select true if the item has been selected, false if unselected
     */
    void select(bool select);

    /**
     * ListItemListener interface function called when an item needs to be reset.
     * Since items are recycled the reset function is where we have
     * to make sure that item state, defined by the arguments, is correct.
     *
     * @param selected true if the item should appear selected, false if unselected
     * @param activated true if the item should appear activated, false if deactivated
     */
    void reset(bool selected, bool activated);

    /**
     * ListItemListener interface called when an item is activated/deactivated.
     *
     * @param activate true if the item has been activated, false if inactive.
     */
    void activate(bool activate);

private:
    // Item Controls.
    ImageView *m_itemImage;
    Label *m_itemTitle;
    Label *m_itemDescription;
    ImageView *m_highlighContainer;
};

#endif
