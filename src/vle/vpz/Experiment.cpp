/** 
 * @file vpz/Experiment.cpp
 * @brief 
 * @author The vle Development Team
 * @date mar, 14 fév 2006 17:40:39 +0100
 */

/*
 * Copyright (C) 2006 - The vle Development Team
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

#include <vle/vpz/Experiment.hpp>
#include <vle/utils/Debug.hpp>
#include <vle/utils/XML.hpp>
#include <glibmm/date.h>

namespace vle { namespace vpz {

using namespace vle::utils;

void Experiment::init(xmlpp::Element* elt)
{
    AssertI(elt);
    AssertI(elt->get_name() == "EXPERIMENTS");

    if (not xml::exist_children(elt, "EXPERIMENT"))
        return;

    xmlpp::Element* exp = xml::get_children(elt, "EXPERIMENT");
    setName(xml::get_attribute(exp, "NAME"));
    setDuration(xml::get_double_attribute(exp, "DURATION"));
    setDate(xml::get_attribute(exp, "DATE"));

    if (xml::exist_attribute(exp, "COMBINATION")) {
        setCombination(xml::get_attribute(exp, "COMBINATION"));
    } else {
        setCombination("total");
    }

    if (xml::exist_attribute(exp, "SEED")) {
        setSeed(utils::translate < guint32 >(
            xml::get_attribute(exp, "SEED")));
    } else {
        setSeed(0);
    }

    if (xml::exist_children(exp, "REPLICAS")) {
        xmlpp::Element* rep = xml::get_children(exp, "REPLICAS");
        m_replicas.init(rep);
    }

    if (xml::exist_children(exp, "EXPERIMENTAL_CONDITIONS")) {
        xmlpp::Element* rep = 
            xml::get_children(exp, "EXPERIMENTAL_CONDITIONS");
        m_conditions.init(rep);
    }

    if (xml::exist_children(exp, "MEASURES")) {
        xmlpp::Element* rep = xml::get_children(exp, "MEASURES");
        m_measures.init(rep);
    }
}

void Experiment::write(xmlpp::Element* elt) const
{
    AssertI(elt);

    xmlpp::Element* exps = elt->add_child("EXPERIMENTS");
    xmlpp::Element* exp = exps->add_child("EXPERIMENT");
    exp->set_attribute("NAME", m_name);
    exp->set_attribute("DURATION", utils::to_string(m_duration));
    exp->set_attribute("DATE", m_date);
    exp->set_attribute("SEED", utils::to_string(m_seed));

    m_replicas.write(exp);
    m_conditions.write(exp);
    m_measures.write(exp);
}

void Experiment::initFromExperiment(xmlpp::Element* elt)
{
    AssertI(elt);
    AssertI(elt->get_name() == "EXPERIMENT");

    if (xml::exist_children(elt, "EXPERIMENTAL_CONDITIONS")) {
        xmlpp::Element* rep = 
            xml::get_children(elt, "EXPERIMENTAL_CONDITIONS");
        m_conditions.init(rep);
    }

    if (xml::exist_children(elt, "MEASURES")) {
        xmlpp::Element* rep = xml::get_children(elt, "MEASURES");
        m_measures.init(rep);
    }
}

void Experiment::clear()
{
    m_name.clear();
    m_duration = 0;
    m_seed = 1;
    m_date.clear();

    m_replicas.clear();
    m_conditions.clear();
    m_measures.clear();
}

void Experiment::addConditions(const Conditions& c)
{
    m_conditions.addConditions(c);
}

void Experiment::addMeasures(const Measures& m)
{
    m_measures.addMeasures(m);
}

void Experiment::setName(const std::string& name)
{
    AssertI(not name.empty());

    m_name.assign(name);
}

void Experiment::setDuration(double duration)
{
    AssertI(duration > 0);

    m_duration = duration;
}

void Experiment::setDate(const std::string& date)
{
    if (not date.empty()) {
        Glib::Date d;
        d.set_parse(date);
        if (d.valid()) {
            m_date = d.format_string("%a, %d %b %Y %H:%M:%S %z");
            return;
        }
    }
    setCurrentDate();
}

void Experiment::setCurrentDate()
{
    m_date = utils::get_current_date();
}
    
void Experiment::setCombination(const std::string& name)
{
    AssertI(name == "linear" or name == "total");

    m_combination.assign(name);
}

}} // namespace vle vpz
