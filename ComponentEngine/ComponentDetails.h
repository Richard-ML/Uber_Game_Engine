#ifndef _cComponentDetails_HG_
#define _cComponentDetails_HG_
#include "stdafx.h"
#include <iostream>
#include <map>
#include <string>
#include <utility>
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cComponentDetails.h - Component Information Registry ----*- C++ -*-===//
Description: Contains core component details
//===----------------------------------------------------------------------===//
Author(s):
Name: Richard Mills-Laursen
Role(s): Student
E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca

Name: Michael Feeney
E-mail: MFeeney@FanshaweC.ca
Role(s): Coordinator & Professor
Course(s):
INFO-6044-01 - Game Engine Framework/Patterns
INFO-6028-01 - Graphics 1
INFO-6019-01 - Physics & Simulation 1
INFO-6045-01 - Animation
INFO-6023-01 - Game Algorithms & Gems
INFO-6020-01 - Graphics 2

Name: Ben McIlmoyle
E-Mail: B_McIlmoyle@FanshaweOnline.ca
Role: Professor
Course: INFO-6016-01 - Network Programming

Name: James Lucas
E-Mail: J_Lucas@FanshaweOnline.ca
Role: Professor
Course(s):
INFO-6017-01 - Artificial Intelligence
INFO-6022-01 - Physics & Simulation 2

Name: Oscar Lara
E-Mail: O_Lara@FanshaweOnline.ca
Role: Professor
Course(s):
INFO-6046-01 - Media Fundamentals
INFO-6025-01 - Configuration & Deployment

Name: Lucas Gustafson
E-Mail: L_Gustafson@FanshaweOnline.ca
Role: Professor
Course: INFO-6024-01 - Game Component Integration
//===----------------------------------------------------------------------===//
Status: Version 1.8 Alpha
(c) Copyright(s): Fanshawe College
//===----------------------------------------------------------------------===//
*/
class cComponent; // Forward deceleration
namespace ComponentManager {
	namespace ComponentDetails {
		typedef cComponent *(*func_createComponent)();
		typedef std::map<std::string, func_createComponent> map_ComponentRegistry;

		inline map_ComponentRegistry &getComponentRegistry() {
			static map_ComponentRegistry reg;
			return reg;
		}
		template <class T> cComponent *createComponent() { return new T; }

		template <class T> struct ComponentEntry {
		public:
			static ComponentEntry<T> &instance(const std::string &componentName) {
				static ComponentEntry<T> instance(componentName); // Singleton
				return instance;
			}

		private:

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Constructor. </summary>
			///
			/// <remarks>	Richard, 4/12/2017. </remarks>
			///
			/// <param name="name">	The name. </param>
			///-------------------------------------------------------------------------------------------------

			ComponentEntry(const std::string &name) {
				map_ComponentRegistry &reg = getComponentRegistry();
				func_createComponent func = createComponent<T>;

				std::pair<map_ComponentRegistry::iterator, bool> ret =
					reg.insert(map_ComponentRegistry::value_type(name, func));
				if (ret.second == false) {
					// Component already exists with this name..
					std::cout << "COMPONENT ALREADY EXISTS! \n";
				}
			}
			ComponentEntry(const ComponentEntry<T> &) =
				delete; // Remove copy constructors
			ComponentEntry &operator=(const ComponentEntry<T> &) = delete;
		};
	} // namespace cComponentDetails
} // namespace cComponentManager
#endif