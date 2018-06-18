#include "json.hpp"


//static
glm::vec3 JSON::loadVector3(const nlohmann::json& _obj, const std::string& _vectName, const glm::vec3& _default)
{
	try
	{
		auto vect = _obj.find(_vectName);
		
		if(vect != _obj.end())
		if(vect->is_array())
		{
			glm::vec3 result(0.f);
			auto it = vect->begin();
			result.x = (it++)->get<float>();
			result.y = (it++)->get<float>();
			result.z = (it++)->get<float>();
			return result;
		}
	}
	catch(...)
	{}

	return _default;
}
