#ifndef _USER_OBJECT_ANALYZER_H_
#define _USER_OBJECT_ANALYZER_H_

#include "cpp_analyzer_base.h"
#include "user_object_analyzer_description.h"
#include "user_object_analyzer_db.h"
#include "stdint.h"
#include <map>
#include <fstream>

namespace osm_diff_analyzer_user_object
{
  class user_object_common_api;

  class user_object_analyzer:public osm_diff_analyzer_cpp_if::cpp_analyzer_base
  {
  public:
    user_object_analyzer(const std::string & p_name,user_object_common_api * p_api);
    ~user_object_analyzer(void);
    // Methods inherited from cpp_analyzer_if
    void init(void);
    void analyze(const std::vector<osm_api_data_types::osm_change*> & p_changes);
    const std::string & get_input_type(void)const;
    const std::string & get_output_type(void)const;
    const std::string & get_type(void)const;
    // End of inherited methods
  private:
    user_object_common_api * m_api;
    bool m_done;
    std::string m_user_name;
    user_object_analyzer_db m_db;
    static user_object_analyzer_description m_description;
  };
}
#endif
