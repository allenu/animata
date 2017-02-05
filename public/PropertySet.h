
#pragma once

#include <map>
#include <string>

enum PropertyType {
    kPropertyTypeDouble = 0,
    kPropertyTypeInt,
    kPropertyTypeBool,
    kPropertyTypeRect,
    kPropertyTypeString
};

struct Property {
    PropertyType type = kPropertyTypeDouble;
    double doubleValue = 0.0;
    int intValue = 0;
    bool boolValue = false;
//    rect_t rectValue = { 0 };
	std::string stringValue;
};

struct PropertySet {
    std::map<std::string, Property> properties;

    void SetProperty(const std::string & name, const Property & property) {
        properties[name] = property;
    }

	bool BoolWithName(const std::string & name) const {
        auto it = properties.find(name);
        if (it != properties.end() && it->second.type == kPropertyTypeBool) {
            return it->second.boolValue;
        }
        return false;
	}

    double DoubleWithName(const std::string & name) const {
        auto it = properties.find(name);
        if (it != properties.end() && it->second.type == kPropertyTypeDouble) {
            return it->second.doubleValue;
        }
        return 0.0;
    }

    int IntWithName(const std::string & name) const {
        auto it = properties.find(name);
        if (it != properties.end() && it->second.type == kPropertyTypeInt) {
            return it->second.intValue;
        }
        return 0;
    }

    std::string StringWithName(const std::string & name) const {
        auto it = properties.find(name);
        if (it != properties.end() && it->second.type == kPropertyTypeString) {
            return it->second.stringValue;
        }
        return "";
    }
};

inline Property BoolProperty(bool value) {
    Property property;
    property.type = kPropertyTypeBool;
    property.boolValue = value;
    return property;
}

inline Property IntProperty(int value) {
    Property property;
    property.type = kPropertyTypeInt;
    property.intValue = value;
    return property;
}

inline Property DoubleProperty(double value) {
    Property property;
    property.type = kPropertyTypeDouble;
    property.doubleValue = value;
    return property;
}

//inline Property RectProperty(rect_t rect) {
//    Property property;
//    property.type = kPropertyTypeRect;
//    property.rectValue = rect;
//    return property;
//}

inline Property StringProperty(const std::string & value) {
    Property property;
    property.type = kPropertyTypeString;
    property.stringValue = value;
    return property;
}
