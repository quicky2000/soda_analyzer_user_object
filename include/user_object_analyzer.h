#ifndef _USER_OBJECT_ANALYZER_H_
#define _USER_OBJECT_ANALYZER_H_

#include "cpp_analyzer_base.h"
#include "user_object_analyzer_description.h"
#include "user_object_analyzer_db.h"
#include "stdint.h"
#include <map>
#include <fstream>
#include <cstdlib>

namespace osm_diff_analyzer_user_object
{
  class user_object_common_api;

  class user_object_analyzer:public osm_diff_analyzer_cpp_if::cpp_analyzer_base
  {
  public:
    user_object_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf,user_object_common_api & p_api);
    ~user_object_analyzer(void);
    // Methods inherited from cpp_analyzer_if
    void init(void);
    void analyze(const std::vector<osm_api_data_types::osm_change*> & p_changes);
    const std::string & get_input_type(void)const;
    const std::string & get_output_type(void)const;
    const std::string & get_type(void)const;
    // End of inherited methods
  private:
    template <class T>
      void generic_analyze(const osm_api_data_types::osm_core_element * const p_object);
    user_object_common_api & m_api;
    bool m_done;
    std::string m_user_name;
    user_object_analyzer_db m_db;
    static user_object_analyzer_description m_description;
  };

  //------------------------------------------------------------------------------
  template <class T>
  void user_object_analyzer::generic_analyze(const osm_api_data_types::osm_core_element * const p_object)
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
      }
    else if(m_db.contains(l_casted_object))
      {
        std::cout << "Change detected in " << l_casted_object->get_core_type_str() << " " << l_casted_object->get_id() << " by user \"" << l_casted_object->get_user() << "\" in changeset " << l_casted_object->get_changeset() << std::endl ;
        exit(-1);
      }

   
  }

}
#endif
