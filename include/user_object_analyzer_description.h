#ifndef _USER_OBJECT_ANALYZER_DESCRIPTION_H_
#define _USER_OBJECT_ANALYZER_DESCRIPTION_H_

#include "analyzer_description_if.h"

namespace osm_diff_analyzer_user_object
{
  class user_object_analyzer_description: public osm_diff_analyzer_if::analyzer_description_if
  {
  public:
    const std::string & get_input_type(void)const;
    const std::string & get_output_type(void)const;
    const std::string & get_type(void)const;
  private:
    static const std::string m_input_type;
    static const std::string m_output_type;
    static const std::string m_type;
  };
}
#endif // _USER_OBJECT_ANALYZER_DESCRIPTION_H_
//EOF
