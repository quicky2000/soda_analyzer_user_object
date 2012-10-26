#include "user_object_wrapper.h"
#include "user_object_analyzer_description.h"
#include "user_object_analyzer.h"
#include "user_object_common_api.h"

#include <cassert>
#include <iostream>

namespace osm_diff_analyzer_user_object
{
  //----------------------------------------------------------------------------
  const char * user_object_wrapper::get_api_version(void)
  {
    return MODULE_LIBRARY_IF_VERSION;
  }

  //----------------------------------------------------------------------------
  uint32_t user_object_wrapper::get_api_size(void)
  {
    return MODULE_LIBRARY_IF_API_SIZE;
  }

  //----------------------------------------------------------------------------
  osm_diff_analyzer_if::analyzer_description_if * user_object_wrapper::get_user_object_description(void)
  {
    return new user_object_analyzer_description();
  }

  //----------------------------------------------------------------------------
  osm_diff_analyzer_if::general_analyzer_if * user_object_wrapper::create_user_object_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf)
  {
    return new user_object_analyzer(p_conf,*m_common_api);
  }

  //----------------------------------------------------------------------------
  void user_object_wrapper::require_common_api(osm_diff_analyzer_if::module_library_if::t_register_function p_func)
  {
    m_common_api = new user_object_common_api(p_func);
  }

  //----------------------------------------------------------------------------
  void user_object_wrapper::cleanup(void)
  {
    delete m_common_api;
  }
  user_object_common_api * user_object_wrapper::m_common_api = NULL;

  extern "C"
  {
    void register_module(void** p_api,uint32_t p_api_size)
    {
      assert(p_api_size == MODULE_LIBRARY_IF_API_SIZE);
      std::cout << "Registration of user_object analyzer API " << std::endl ;
      p_api[osm_diff_analyzer_if::module_library_if::GET_API_VERSION] = (void*)user_object_wrapper::get_api_version;
      p_api[osm_diff_analyzer_if::module_library_if::GET_API_SIZE] = (void*)user_object_wrapper::get_api_size;
      p_api[osm_diff_analyzer_if::module_library_if::GET_DESCRIPTION] = (void*)user_object_wrapper::get_user_object_description;
      p_api[osm_diff_analyzer_if::module_library_if::CREATE_ANALYZER] = (void*)user_object_wrapper::create_user_object_analyzer;
      p_api[osm_diff_analyzer_if::module_library_if::REQUIRE_COMMON_API] = (void*)user_object_wrapper::require_common_api;
      p_api[osm_diff_analyzer_if::module_library_if::CLEAN_UP] = (void*)user_object_wrapper::cleanup;
    }
  }
}
//EOF
