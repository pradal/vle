/**
 * @file vle/gvle/TreeViewConditions.cpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <vle/gvle/ConditionBox.hpp>
#include <vle/gvle/TreeViewConditions.hpp>
#include <vle/gvle/SimpleTypeBox.hpp>
#include <iostream>

using namespace vle;

namespace vle
{
namespace gvle {

TreeViewConditions::TreeViewConditions(vpz::Conditions& cond, const vpz::AtomicModel& atom):
        mCond(&cond),
        mAtom(&atom)
{
    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    set_model(m_refTreeModel);

    //Add the TreeView's view columns:
    append_column("Name", m_Columns.m_col_name);
    append_column_editable("In", m_Columns.m_col_activ);
    {
        using namespace Gtk::Menu_Helpers;
        Gtk::Menu::MenuList* items = &mMenu.items();
        items->push_back(Gtk::Menu_Helpers::MenuElem("Insert",
                         sigc::mem_fun(*this, &TreeViewConditions::on_menu_insert)));
        items->push_back(Gtk::Menu_Helpers::MenuElem("Remove",
                         sigc::mem_fun(*this, &TreeViewConditions::on_menu_remove)));
    }
}

TreeViewConditions::~TreeViewConditions()
{
}

void TreeViewConditions::on_row_activated(const Gtk::TreeModel::Path& path,
        Gtk::TreeViewColumn* /*column*/)
{
    Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
    if (iter) {
        Gtk::TreeModel::Row row = *iter;
        Glib::ustring s = row[m_Columns.m_col_name];
        vpz::Condition& cond = mCond->get(s);
        ConditionBox cb(&cond);
        cb.run();
    }
}

bool TreeViewConditions::on_button_press_event(GdkEventButton* event)
{
    if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)) {
        mMenu.popup(event->button, event->time);
    }
    return TreeView::on_button_press_event(event);
}

std::vector<std::string>* TreeViewConditions::get_conditions_checked()
{
    std::vector<std::string>* ret = new std::vector<std::string>();
    typedef Gtk::TreeModel::Children type_children;
    type_children children = m_refTreeModel->children();
    for (type_children::iterator iter = children.begin();
            iter != children.end(); ++iter) {
        Gtk::TreeModel::Row row = *iter;
        if (row.get_value(m_Columns.m_col_activ) == true) {
            ret->push_back(row.get_value(m_Columns.m_col_name));
        }
    }
    return ret;
}

void TreeViewConditions::makeTreeView()
{
    m_refTreeModel->clear();
    const vpz::Strings& cond = mAtom->conditions();
    vpz::Strings::const_iterator f;
    vpz::ConditionList list = mCond->conditionlist();
    vpz::ConditionList::iterator it = list.begin();

    while (it != list.end()) {
        Gtk::TreeModel::Row row = *(m_refTreeModel->append());
        row[m_Columns.m_col_name] = it->first;
        f = std::find(cond.begin(), cond.end(), it->first);
        if (f != cond.end())
            row[m_Columns.m_col_activ] = true;
        else
            row[m_Columns.m_col_activ] = false;

        it++;
    }
}

void TreeViewConditions::on_menu_insert()
{
    SimpleTypeBox box("name ?");
    std::string name;
    do {
        name = box.run();
        boost::trim(name);
    } while (name == ""  || mCond->exist(name));
    mCond->add(vpz::Condition(name));
    makeTreeView();
}

void TreeViewConditions::on_menu_remove()
{
    Glib::RefPtr<Gtk::TreeView::Selection> refSelection = get_selection();
    if (refSelection) {
        Gtk::TreeModel::iterator iter = refSelection->get_selected();
        if (iter) {
            Gtk::TreeModel::Row row = *iter;
            mCond->del(row.get_value(m_Columns.m_col_name));
            makeTreeView();
        }
    }
}

}
} // namespace vle gvle
