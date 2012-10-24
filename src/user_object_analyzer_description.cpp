#include "user_object_analyzer_description.h"

namespace osm_diff_analyzer_user_object
{
  //----------------------------------------------------------------------------
  const std::string & user_object_analyzer_description::get_input_type(void)const
  {
    return m_input_type;
  }

  //----------------------------------------------------------------------------
  const std::string & user_object_analyzer_description::get_output_type(void)const
  {
    return m_output_type;
  }

  //----------------------------------------------------------------------------
  const std::string & user_object_analyzer_description::get_type(void)const
  {
    return m_type;
  }

  const std::string user_object_analyzer_description::m_input_type = "cpp";
  const std::string user_object_analyzer_description::m_output_type = "";
  const std::string user_object_analyzer_description::m_type = "user_object";

}
//EOF
