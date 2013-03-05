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
    void register_module(uintptr_t* p_api,uint32_t p_api_size)
    {
      if(p_api_size != MODULE_LIBRARY_IF_API_SIZE)
	{
	  std::stringstream l_stream;
	  l_stream << "p_api_size != MODULE_LIBRARY_IF_API_SIZE : " << p_api_size << " < " << MODULE_LIBRARY_IF_API_SIZE << ". Please use a newver version of saoda";
	  throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	}
#ifdef DEBUG
      std::cout << "Registration of user_object analyzer API " << std::endl ;
#endif
      p_api[osm_diff_analyzer_if::module_library_if::GET_API_VERSION] = (uintptr_t)user_object_wrapper::get_api_version;
      p_api[osm_diff_analyzer_if::module_library_if::GET_API_SIZE] = (uintptr_t)user_object_wrapper::get_api_size;
      p_api[osm_diff_analyzer_if::module_library_if::GET_DESCRIPTION] = (uintptr_t)user_object_wrapper::get_user_object_description;
      p_api[osm_diff_analyzer_if::module_library_if::CREATE_ANALYZER] = (uintptr_t)user_object_wrapper::create_user_object_analyzer;
      p_api[osm_diff_analyzer_if::module_library_if::REQUIRE_COMMON_API] = (uintptr_t)user_object_wrapper::require_common_api;
      p_api[osm_diff_analyzer_if::module_library_if::CLEAN_UP] = (uintptr_t)user_object_wrapper::cleanup;
    }
  }
}
//EOF
