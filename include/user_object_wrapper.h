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
#ifndef _USER_OBJECT_WRAPPER_H_
#define _USER_OBJECT_WRAPPER_H_

#include "general_analyzer_if.h"
#include "module_library_if.h"
#include <inttypes.h>

namespace osm_diff_analyzer_user_object
{
  class user_object_common_api;

  class user_object_wrapper
  {
  public:
    static const char * get_api_version(void);
    static uint32_t get_api_size(void);
    static osm_diff_analyzer_if::analyzer_description_if * get_user_object_description(void);
    static osm_diff_analyzer_if::general_analyzer_if * create_user_object_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf);
    static void require_common_api(osm_diff_analyzer_if::module_library_if::t_register_function);
    static void cleanup(void);
  private:
    static user_object_common_api *m_common_api;
  };

}
#endif // _USER_OBJECT_WRAPPER_H_
//EOF
