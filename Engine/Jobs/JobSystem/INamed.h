#pragma once

#include <string>
namespace Engine
{
	namespace JobSystem
	{
		class INamed
		{
		public:
			INamed(const char* i_pName = nullptr) :
				m_Name(i_pName ? i_pName : "")
			{}

			void					SetName(const char* i_pName)
			{
				m_Name = i_pName;
			}
			const char* GetName(void) const
			{
				return m_Name.c_str();
			}

		private:
			std::string				m_Name;
		};

	} // namespace JobSystem
} // namespace Engine