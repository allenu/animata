//
//  PropertySetFromJsonDictionary.h
//  Y2K
//
//  Created by Allen Ussher on 2/5/17.
//  Copyright © 2017 Allen Ussher. All rights reserved.
//

#pragma once

#include "PropertySet.h"
#include "json.h"

PropertySet PropertySetFromJsonDictionary(const Json::Value & document, const std::vector<std::string> & ignore_property_names = std::vector<std::string>() );
