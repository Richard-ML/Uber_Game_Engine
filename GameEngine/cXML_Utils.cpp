#include "cXML_Utils.h"
#include <locale>
cXML_Utils *cXML_Utils::instance() {
  if (!s_cXML_Utils)
    s_cXML_Utils = new cXML_Utils();
  return s_cXML_Utils;
}

bool cXML_Utils::attributeToBool(rapidxml::xml_attribute<> *attribute) {
  std::string sValue = attribute->value();
  bool result;

  // Convert string's characters to uppercase..
  std::locale loc;
  for (std::string::size_type i = 0; i < sValue.length(); ++i)
    sValue[i] = std::toupper(sValue[i], loc);

  (sValue == "1" 
	  | sValue == "TRUE")
      ? result = true
      : (sValue == "0" || sValue == "FALSE") ? result = false :
                                             // TODO: Throw things
            result = 0;

  return result;
}
