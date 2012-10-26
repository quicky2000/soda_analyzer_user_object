#include "user_object_analyzer.h"
#include "user_object_common_api.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

namespace osm_diff_analyzer_user_object
{
  //------------------------------------------------------------------------------
  user_object_analyzer::user_object_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf,user_object_common_api & p_api):
    osm_diff_analyzer_cpp_if::cpp_analyzer_base("user_analyser",p_conf->get_name(),""),
    m_api(p_api),
    m_done(false),
    m_user_name("")
  {
    const std::map<std::string,std::string> & l_conf_parameters = p_conf->get_parameters();
    std::map<std::string,std::string>::const_iterator l_iter = l_conf_parameters.find("user_name");
    if(l_iter == l_conf_parameters.end())
      {
	std::cout << "ERROR : missing mandatory \"user_name\" parameter" << std::endl ;
	exit(-1);
      }
    std::cout << "parameter[\"user_name\"]=\"" << l_iter->second << "\"" << std::endl ;
    delete p_conf;
  }

  //------------------------------------------------------------------------------
  user_object_analyzer::~user_object_analyzer(void)
  {
  }

  //------------------------------------------------------------------------------
  void user_object_analyzer::init(void)
  {
  }

  //------------------------------------------------------------------------------
  void user_object_analyzer::analyze(const std::vector<osm_api_data_types::osm_change*> & p_changes)
  {
    uint32_t m_deletion = 0;
    uint32_t m_creation = 0;
    uint32_t m_modification = 0;

    for(std::vector<osm_api_data_types::osm_change*>::const_iterator l_iter = p_changes.begin();
        l_iter != p_changes.end();
        ++l_iter)
      {
        const osm_api_data_types::osm_core_element * const l_element = (*l_iter)->get_core_element();
        assert(l_element);

        switch(l_element->get_core_type())
          {
          case osm_api_data_types::osm_core_element::NODE :
              generic_analyze<osm_api_data_types::osm_node>(l_element);
            break;
          case osm_api_data_types::osm_core_element::WAY :
              generic_analyze<osm_api_data_types::osm_way>(l_element);
            break;
          case osm_api_data_types::osm_core_element::RELATION :
              generic_analyze<osm_api_data_types::osm_relation>(l_element);
            break;
          case osm_api_data_types::osm_core_element::INTERNAL_INVALID:
            std::cout << "ERROR : unexpected core type value \"" << osm_api_data_types::osm_core_element::get_osm_type_str(l_element->get_core_type()) << "\"" << std::endl ;
            exit(-1);
            break;
          }

        switch((*l_iter)->get_type())
          {
          case osm_api_data_types::osm_change::CREATION :
            ++m_creation;
            break;
          case osm_api_data_types::osm_change::MODIFICATION :
            ++m_modification;
            break;
          case osm_api_data_types::osm_change::DELETION :
            ++m_deletion;
            break;
          case osm_api_data_types::osm_change::INTERNAL_INVALID :
            std::cout << "Unexpected change type" << std::endl ;
            exit(-1);
            break;
          }
      }
    std::cout << "--------------------------" << std::endl ;
    std::cout << "Creation : " << m_creation << std::endl ;
    std::cout << "Modification : " << m_modification << std::endl ;
    std::cout << "Deletion : " << m_deletion << std::endl ;
    std::cout << "--------------------------" << std::endl ;
  }

  //------------------------------------------------------------------------------
  const std::string & user_object_analyzer::get_input_type(void)const
  {
    return m_description.get_input_type();
  }

  //------------------------------------------------------------------------------
  const std::string & user_object_analyzer::get_output_type(void)const
  {
    return m_description.get_output_type();
  }

  //------------------------------------------------------------------------------
  const std::string & user_object_analyzer::get_type(void)const
  {
    return m_description.get_type();
  }

  user_object_analyzer_description user_object_analyzer::m_description;
}
//EOF
