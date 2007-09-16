/** 
 * @file StreamReader.cpp
 * @brief Base class of the output stream reader.
 * @author The vle Development Team
 * @date 2007-07-15
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

#include <vle/oov/StreamReader.hpp>
#include <vle/oov/Plugin.hpp>
#include <vle/utils/Path.hpp>
#include <vle/utils/Debug.hpp>

namespace vle { namespace oov {

PluginPtr StreamReader::plugin()
{
    Assert(utils::InternalError, m_plugin.get(),
            "Plugin are not loaded and cannot respond to the StreamReader");

    return m_plugin;
}

void StreamReader::init_plugin(const std::string& plugin,
                               const std::string& location)
{
    utils::Path::PathList lst(utils::Path::path().getStreamDirs());
    utils::Path::PathList::const_iterator it;

    std::string error((boost::format(
                "Error opening oov plugin '%1%' in:") % plugin).str());

    for (it = lst.begin(); it != lst.end(); ++it) {
        try {
            PluginFactory pfd(plugin, *it);
            m_plugin = pfd.build(location);
            return;
        } catch (const std::exception& e) {
            error += e.what();
        }
    }
}

StreamReader::PluginFactory::PluginFactory(const std::string& plugin,
                                           const std::string& pathname) :
    module__(0),
    plugin__(plugin)
{
    std::string file(Glib::Module::build_path(pathname, plugin));
    module__ = new Glib::Module(file);
    if (not (*module__)) {
        delete module__;
        module__ = 0;
        Throw(utils::InternalError, boost::format(
                "\n[%1%]: %2%") % pathname % Glib::Module::get_last_error());
    }
    module__->make_resident();
}

StreamReader::PluginFactory::~PluginFactory()
{
    delete module__;
}

PluginPtr StreamReader::PluginFactory::build(const std::string& location)
{
    Plugin* call = 0;
    void*   makeNewOovPlugin = 0;

    if (not module__->get_symbol("makeNewOovPlugin", makeNewOovPlugin)) {
        Throw(utils::InternalError, boost::format(
                "Error when searching makeNewOovPlugin function in plugin %1%")
            % plugin__);
    }

    call = ((Plugin*(*)(const std::string&))(makeNewOovPlugin))(location);
    if (not call) {
        Throw(utils::InternalError, boost::format(
                "Error when calling makeNewOovPlugin function in plugin %1%")
            % plugin__);
    }

    PluginPtr plugin(call);
    return plugin;
}

}} // namespace vle oov
