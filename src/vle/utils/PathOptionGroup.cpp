/** 
 * @file PathOptionGroup.cpp
 * @brief 
 * @author The vle Development Team
 * @date 2007-09-07
 */

/*
 * Copyright (C) 2007 - The vle Development Team
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <vle/utils/PathOptionGroup.hpp>
#include <vle/utils/Path.hpp>



namespace vle { namespace utils {

PathOptionGroup::PathOptionGroup() :
    Glib::OptionGroup("pathgroup",
                      "Plugins path options",
                      "Description of the path options")
{
    {
        Glib::OptionEntry en;
        en.set_long_name("path");
        en.set_description("Add pathname for all plugins");
        add_entry(en, m_all);
    }
    {
        Glib::OptionEntry en;
        en.set_long_name("path-simulator");
        en.set_description("Add pathname for simulator plugins");
        add_entry(en, m_simulator);
    }
    {
        Glib::OptionEntry en;
        en.set_long_name("path-stream");
        en.set_description("Add pathname for stream plugins");
        add_entry(en, m_stream);
    }
    {
        Glib::OptionEntry en;
        en.set_long_name("path-translator");
        en.set_description("Add pathname for translator plugins");
        add_entry(en, m_translator);
    }
    {
        Glib::OptionEntry en;
        en.set_long_name("path-model");
        en.set_description("Add pathname for model plugins");
        add_entry(en, m_model);
    }
}

PathOptionGroup::~PathOptionGroup()
{
}

void PathOptionGroup::assignToPath()
{
    Glib::OptionGroup::vecustrings::const_iterator it;

    for (it = m_all.begin(); it != m_all.end(); ++it)
        utils::Path::path().addPluginDir(*it);
    for (it = m_simulator.begin(); it != m_simulator.end(); ++it)
        utils::Path::path().addSimulatorDir(*it);
    for (it = m_stream.begin(); it != m_stream.end(); ++it)
        utils::Path::path().addStreamDir(*it);
    for (it = m_translator.begin(); it != m_translator.end(); ++it)
        utils::Path::path().addTranslatorDir(*it);
    for (it = m_model.begin(); it != m_model.end(); ++it)
        utils::Path::path().addModelDir(*it);
}


}} // namespace vle utils