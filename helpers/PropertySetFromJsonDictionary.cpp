//
//  PropertySetFromJsonDictionary.cpp
//  Y2K
//
//  Created by Allen Ussher on 2/5/17.
//  Copyright © 2017 Allen Ussher. All rights reserved.
//

#include <assert.h>
#include <string>
#include "PropertySetFromJsonDictionary.h"

PropertySet PropertySetFromJsonDictionary(const Json::Value & document, const std::vector<std::string> & ignore_property_names) {
	PropertySet propertySet;
	
	Json::Value::Members propertyNames( document.getMemberNames() );
	for ( Json::Value::Members::iterator it = propertyNames.begin();
		 it != propertyNames.end();
		 ++it ) {
		const std::string &propertyName = *it;
		
		if (find(ignore_property_names.begin(), ignore_property_names.end(), propertyName) == ignore_property_names.end()) {
			const Json::Value value = document[propertyName];
			
			if (value.isInt()) {
				propertySet.SetProperty(propertyName, IntProperty(value.asInt()));
			} else if (value.isDouble()) {
				propertySet.SetProperty(propertyName, DoubleProperty(value.asDouble()));
			} else if  (value.isString()) {
				propertySet.SetProperty(propertyName, StringProperty(value.asString()));
			} else if  (value.isBool()) {
				propertySet.SetProperty(propertyName, BoolProperty(value.asBool()));
			} else {
				assert(false);
			}
		}
	}

	return propertySet;
}
