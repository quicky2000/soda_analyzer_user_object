/*
    This file is part of osm_diff_analyzer_user_object, Openstreetmap
    diff analyzer based on CPP diff representation. It's aim is to survey
    objects edited by parametered used and to generate an alert in case of 
    edition on these objects
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _USER_OBJECT_ANALYZER_H_
#define _USER_OBJECT_ANALYZER_H_

#include "cpp_analyzer_base.h"
#include "user_object_analyzer_description.h"
#include "user_object_analyzer_db.h"
#include "user_object_common_api.h"
#include "module_configuration.h"
#include "stdint.h"
#include <map>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <set>
#include <iomanip>

namespace osm_diff_analyzer_user_object
{
  class user_object_common_api;

  class user_object_analyzer:public osm_diff_analyzer_cpp_if::cpp_analyzer_base
  {
  public:
    user_object_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf,user_object_common_api & p_api);
    ~user_object_analyzer(void);
    // Methods inherited from cpp_analyzer_if
    void init(const osm_diff_analyzer_if::osm_diff_state * p_diff_state);
    void analyze(const std::vector<osm_api_data_types::osm_change*> & p_changes);
    const std::string & get_input_type(void)const;
    const std::string & get_output_type(void)const;
    const std::string & get_type(void)const;
    // End of inherited methods
  private:
    inline void compare(std::ofstream & p_stream,
			const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
			const osm_api_data_types::osm_node & p_node2);
    inline void compare(std::ofstream & p_stream,
			const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
			const osm_api_data_types::osm_way & p_way2);
    inline void compare(std::ofstream & p_stream,
			const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
			const osm_api_data_types::osm_relation & p_relation2);
    inline void compare(std::ofstream & p_stream,
	       const osm_api_data_types::osm_core_element & p_core_element1,
	       const osm_api_data_types::osm_core_element & p_core_element2);
    
    template <class T>
      void generic_analyze(const osm_api_data_types::osm_core_element * const p_object,
			   const osm_api_data_types::osm_change::t_osm_change_type & p_change);
    user_object_common_api & m_api;
    bool m_done;
    std::string m_user_name;
    user_object_analyzer_db m_db;
    std::ofstream m_report;
    static user_object_analyzer_description m_description;
  };

  //------------------------------------------------------------------------------
  template <class T>
    void user_object_analyzer::generic_analyze(const osm_api_data_types::osm_core_element * const p_object,
					       const osm_api_data_types::osm_change::t_osm_change_type & p_change)
  {

    const T * const l_casted_object = dynamic_cast<const T * const>(p_object);
    if(l_casted_object==NULL)
      {
        std::cout << "ERROR : invalid " << T::get_type_str() << " cast for object id " << p_object->get_id() << std::endl ;
        exit(-1);
      }
    if(l_casted_object->get_user() == m_user_name && !m_db.contains(l_casted_object))
      {
        m_db.insert(l_casted_object);
	m_api.cache(*l_casted_object);
      }
    else if(m_db.contains(l_casted_object) && l_casted_object->get_user() != m_user_name)
      {
	std::stringstream l_id_str;
	l_id_str << l_casted_object->get_id();
	bool l_modified = p_change == osm_api_data_types::osm_change::MODIFICATION;
	osm_api_data_types::osm_core_element::t_osm_version l_previous_version = l_casted_object->get_version() - 1;

	std::string l_object_url = "http://www.openstreetmap.org/browse/"+l_casted_object->get_core_type_str()+"/"+l_id_str.str();
	if(!l_modified)
	  {
	    std::stringstream l_version_str;
	    l_version_str << l_previous_version;
	    l_object_url += "/"+l_version_str.str();
	  }
	std::stringstream l_changeset_str;
	l_changeset_str << l_casted_object->get_changeset();
	std::string l_changeset_url = "http://www.openstreetmap.org/browse/changeset/"+l_changeset_str.str();
	std::string l_user_url = "http://www.openstreetmap.org/user/"+l_casted_object->get_user();
	

	m_report << "<A HREF=\"" << l_object_url << "\">" << l_casted_object->get_core_type_str() << " " << l_casted_object->get_id() << "</A>" ;
	m_report << " has been " << (l_modified  ? "modified":"deleted") << " " ;
	m_report << " by user ";
	m_report << "<A HREF=\"" << l_user_url << "\">" << l_casted_object->get_user() << "</A>" ;
	m_report << " in " ;
	m_report << "<A HREF=\"" << l_changeset_url << "\">" << "changeset " << l_casted_object->get_changeset() << "</A><BR>" << std::endl ;
	if(l_modified)
	  {
	    compare(m_report,l_previous_version,*l_casted_object);
	  }
	m_report << "<HR/>" << std::endl ;
      }

   
  }

  //------------------------------------------------------------------------------
  void user_object_analyzer::compare(std::ofstream & p_stream,
				     const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
				     const osm_api_data_types::osm_node & p_node2)
  {
    const osm_api_data_types::osm_node * const l_node1 = m_api.get_node_version(p_node2.get_id(),p_previous_version);
    p_stream << "<ul>" << std::setprecision(9);
    if(l_node1->get_lat() != p_node2.get_lat() && l_node1->get_lon() != p_node2.get_lon())
      {
	p_stream << "<li>Coordinates has changed from (" << l_node1->get_lat() << "," << l_node1->get_lon() << ") to (" << p_node2.get_lat() << "," << p_node2.get_lon() << ")</li>" << std::endl ;
      }
    else if(l_node1->get_lat() != p_node2.get_lat())
      {
	p_stream << "<li>Latitude has changed from " << l_node1->get_lat() << " to " << p_node2.get_lat() << "</li>" << std::endl ;
      }
    else if(l_node1->get_lon() != p_node2.get_lon())
      {
	p_stream << "<li>Longitude has changed from " << l_node1->get_lon() << " to " << p_node2.get_lon() << "</li>" << std::endl ;
      }
    compare(p_stream,*((const osm_api_data_types::osm_core_element *)l_node1),*((const osm_api_data_types::osm_core_element *)&p_node2));
    p_stream << "</ul>" ;
    delete l_node1;
  }

  //------------------------------------------------------------------------------
  void user_object_analyzer::compare(std::ofstream & p_stream,
				     const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
				     const osm_api_data_types::osm_way & p_way2)
  {
    const osm_api_data_types::osm_way * const l_way1 = m_api.get_way_version(p_way2.get_id(),p_previous_version);
    p_stream << "<ul>" ;
    compare(p_stream,*((const osm_api_data_types::osm_core_element *)l_way1),*((const osm_api_data_types::osm_core_element *)&p_way2));
    std::set<osm_api_data_types::osm_object::t_osm_id> l_nodes_way1;
    std::set<osm_api_data_types::osm_object::t_osm_id> l_nodes_way2;
    for(std::vector<osm_api_data_types::osm_object::t_osm_id>::const_iterator l_iter = l_way1->get_node_refs().begin();
	l_iter != l_way1->get_node_refs().end();
	++l_iter)
      {
	l_nodes_way1.insert(*l_iter);
      }
    for(std::vector<osm_api_data_types::osm_object::t_osm_id>::const_iterator l_iter = p_way2.get_node_refs().begin();
	l_iter != p_way2.get_node_refs().end();
	++l_iter)
      {
	l_nodes_way2.insert(*l_iter);
	if(l_nodes_way1.find(*l_iter)==l_nodes_way1.end())
	  {
	    p_stream << "<li><A HREF=\"http://www.openstreetmap.org/browse/node/" << *l_iter << "\">Node " << *l_iter << "</A> has been added to <A HREF=\"http://www.openstreetmap.org/browse/way/" << p_way2.get_id() << "\">Way " << p_way2.get_id() << "</A></li>" << std::endl ;
	  }
      }
    for(std::vector<osm_api_data_types::osm_object::t_osm_id>::const_iterator l_iter = l_way1->get_node_refs().begin();
	l_iter != l_way1->get_node_refs().end();
	++l_iter)
      {
	if(l_nodes_way2.find(*l_iter)==l_nodes_way2.end())
	  {
	    p_stream << "<li><A HREF=\"http://www.openstreetmap.org/browse/node/" << *l_iter << "\">Node " << *l_iter << "</A> has been removed from <A HREF=\"http://www.openstreetmap.org/browse/way/" << p_way2.get_id() << "\">Way " << p_way2.get_id() << "</A></li>" << std::endl ;
	  }
      }
    p_stream << "</ul>" ;
    delete l_way1;
  }

  //------------------------------------------------------------------------------
  void user_object_analyzer::compare(std::ofstream & p_stream,
				     const osm_api_data_types::osm_core_element::t_osm_version p_previous_version,
				     const osm_api_data_types::osm_relation & p_relation2)
  {
    const osm_api_data_types::osm_relation * const l_relation1 = m_api.get_relation_version(p_relation2.get_id(),p_previous_version);
    p_stream << "<ul>" ;
    compare(p_stream,*((const osm_api_data_types::osm_core_element *)l_relation1),*((const osm_api_data_types::osm_core_element *)&p_relation2));
    std::map<osm_api_data_types::osm_object::t_osm_id,osm_api_data_types::osm_relation_member*> l_members_relation1;
    std::map<osm_api_data_types::osm_object::t_osm_id,osm_api_data_types::osm_relation_member*> l_members_relation2;
    for(std::vector<osm_api_data_types::osm_relation_member*>::const_iterator l_iter = l_relation1->get_members().begin();
	l_iter != l_relation1->get_members().end();
	++l_iter)
      {
	l_members_relation1.insert(std::map<osm_api_data_types::osm_object::t_osm_id,osm_api_data_types::osm_relation_member*>::value_type((*l_iter)->get_object_ref(),*l_iter));
      }
    for(std::vector<osm_api_data_types::osm_relation_member*>::const_iterator l_iter = p_relation2.get_members().begin();
	l_iter != p_relation2.get_members().end();
	++l_iter)
      {
	const osm_api_data_types::osm_object::t_osm_id l_member_id = (*l_iter)->get_object_ref();
	l_members_relation2.insert(std::map<osm_api_data_types::osm_object::t_osm_id,osm_api_data_types::osm_relation_member*>::value_type(l_member_id,*l_iter));
	std::map<osm_api_data_types::osm_object::t_osm_id,osm_api_data_types::osm_relation_member*>::const_iterator l_member_iter = l_members_relation1.find(l_member_id);
	if(l_member_iter==l_members_relation1.end())
	  {
	    std::string l_type_str(osm_api_data_types::osm_core_element::get_osm_type_str((*l_iter)->get_type()));
	    p_stream << "<li><A HREF=\"http://www.openstreetmap.org/browse/"<< l_type_str <<"/" << l_member_id << "\">" << l_type_str << " " << l_member_id << "</A> has been added with role \"" << (*l_iter)->get_role() << "\"</li>" << std::endl ;
	  }
	else if(l_member_iter->second->get_role() != (*l_iter)->get_role())
	  {
	    std::string l_type_str(osm_api_data_types::osm_core_element::get_osm_type_str(l_member_iter->second->get_type()));
	    p_stream << "<li>Role of <A HREF=\"http://www.openstreetmap.org/browse/"<< l_type_str <<"/" << l_member_id << "\">" << l_type_str << " " << l_member_id << "</A> has changed from \"" << l_member_iter->second->get_role() << "\" to \"" << (*l_iter)->get_role() << "\"</li>" << std::endl ;
	  }
      }
    for(std::vector<osm_api_data_types::osm_relation_member*>::const_iterator l_iter = l_relation1->get_members().begin();
	l_iter != l_relation1->get_members().end();
	++l_iter)
      {
	const osm_api_data_types::osm_object::t_osm_id l_member_id = (*l_iter)->get_object_ref();
	std::map<osm_api_data_types::osm_object::t_osm_id,osm_api_data_types::osm_relation_member*>::const_iterator l_member_iter = l_members_relation1.find(l_member_id);
	if(l_member_iter==l_members_relation2.end())
	  {
	    std::string l_type_str(osm_api_data_types::osm_core_element::get_osm_type_str((*l_iter)->get_type()));
	    p_stream << "<li><A HREF=\"http://www.openstreetmap.org/browse/"<< l_type_str <<"/" << l_member_id << "\">" << l_type_str << " " << l_member_id << "</A> has been removed</li>" << std::endl ;
	  }
      }
    p_stream << "</ul>" ;
    delete l_relation1;
  }

  //------------------------------------------------------------------------------
  void user_object_analyzer::compare(std::ofstream & p_stream,
				     const osm_api_data_types::osm_core_element & p_core_element1,
				     const osm_api_data_types::osm_core_element & p_core_element2)
  {
    for(std::map<std::string,std::string>::const_iterator l_iter = p_core_element1.get_tags().begin();
	l_iter != p_core_element1.get_tags().end();
	++l_iter)
      {
	if(p_core_element2.contains_tag(l_iter->first))
	  {
	    std::string l_new_value = p_core_element2.get_tag_value(l_iter->first);
	    if(l_new_value != l_iter->second)
	      {
		p_stream << "<li>Tag \"" << l_iter->first << "\" value has changed from \"" << l_iter->second << "\" to \"" << l_new_value << "\"</li>" << std::endl ;
	      }
	  }
	else
	  {
	    p_stream << "<li>Tag \"" << l_iter->first << "\" with value \"" << l_iter->second << "\" has been removed</li>" << std::endl ;
	  }
      }
    for(std::map<std::string,std::string>::const_iterator l_iter = p_core_element2.get_tags().begin();
	l_iter != p_core_element2.get_tags().end();
	++l_iter)
      {
	if(!p_core_element1.contains_tag(l_iter->first))
	  {
	    p_stream << "<li>Tag \"" << l_iter->first << "\" with value \"" << l_iter->second << "\" has been added</li>" << std::endl ;
	  }
      }
    
  }
}
#endif
