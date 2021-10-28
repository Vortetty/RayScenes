//
// Licensed under the Apache License, Version 2.0
// See the LICENSE file in the project root for more information
// © Winter/Vortetty 2021 
//

#include "rayscenes.hpp"

#include <deque>
#include <unordered_map>
#include <string>
#include <any>
#include <functional>
#include <tuple>
#include <optional>
#include <typeinfo>
#include <typeindex>

namespace rayscenes {
	namespace {
		template <class... Args>
		auto any_to_variant_cast(std::any a) -> std::variant<Args...>
		{
			if (!a.has_value())
				throw std::bad_any_cast();

			std::optional<std::variant<Args...>> v = std::nullopt;

			bool found = ((a.type() == typeid(Args) && (v = std::any_cast<Args>(std::move(a)), true)) || ...);

			if (!found)
				throw std::bad_any_cast{};

			return std::move(*v);
		}
	}

	// 
	// Class | rayscenemanager::rayscene 
	//
		//
		// Initializers
		//
		rayscenemanager::rayscene::rayscene(rayscenefunc _func) { func = _func; funcSet = true; }
		rayscenemanager::rayscene::rayscene() {};

		//
		// Utilities
		//
		void rayscenemanager::rayscene::setFunc(rayscenefunc _func) { func = _func; funcSet = true; }
		void rayscenemanager::rayscene::call(rayscenemanager &rsm, bool isTop) { 
			if (funcSet) 
				switch (func.index()) {
					case 0:
						std::get<void(*)(rayscenes::rayscenemanager&, bool)>(func)(rsm, isTop);
						break;
					case 1:
						std::get<std::function<void(rayscenes::rayscenemanager&, bool)>>(func)(rsm, isTop);
						break;
				}
		}


	//
	// Class | rayscenemanager
	//

		//
		// Initializers
		//
		rayscenemanager::rayscenemanager() {}
		rayscenemanager::rayscenemanager(const rayscenemanager& r) {
			clear_unordered_map(scenes);
			clear_deque(activeScenes);

			for (auto &[key, scene] : ((rayscenemanager)r).getAllScenes()) scenes[key] = scene;
			for (auto &scene : ((rayscenemanager)r).getActiveScenes()) activeScenes.push_back(scene);
		}

		//
		// Basic scene management
		//
		std::unordered_map<std::string, rayscenemanager::rayscene>& rayscenemanager::getAllScenes() { return scenes; }
		void                                                        rayscenemanager::setAllScenes(std::unordered_map<std::string, rayscene> newScenes) { for (auto &[key, scene] : newScenes) scenes[key] = scene; }
		rayscenemanager::rayscene&                                  rayscenemanager::getScene(std::string id) { return scenes[id]; }
		void                                                        rayscenemanager::addScene(std::string id, rayscene scene) { scenes[id] = scene; }
		void                                                        rayscenemanager::addScene(std::string id, void (*func)(rayscenes::rayscenemanager&, bool)) { scenes[id] = rayscene( rayscenefunc(func) );}
		void                                                        rayscenemanager::addScene(std::string id, std::function<void(rayscenes::rayscenemanager&, bool)>& func) { scenes[id] = rayscene( rayscenefunc(func) );}
		void                                                        rayscenemanager::removeScene(std::string id) { scenes.erase(id); }

		//
		// Active scene management
		//
		std::deque<std::string>& rayscenemanager::getActiveScenes() { return activeScenes; }
		void                     rayscenemanager::setActiveScenes(std::deque<std::string> ids) {
			clear_deque(activeScenes);
			for (auto &id : ids) activeScenes.push_back(id);
		}
		void                     rayscenemanager::setActiveScene(std::string id) {
			clear_deque(activeScenes);
			activeScenes.push_back(id);
		}
		void                     rayscenemanager::addActiveScene(std::string id) { activeScenes.push_back(id); }
		void                     rayscenemanager::addActiveScenes(std::deque<std::string> ids) { for (auto &id : ids) activeScenes.push_back(id); }
		void                     rayscenemanager::removeActiveScene(std::string id) { activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); }
		void                     rayscenemanager::removeActiveScenes(std::deque<std::string> ids) { for (auto &id : ids) activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); }
		void                     rayscenemanager::clearActiveScenes(std::string id) { activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); }

		//
		// Other functions
		//
		void rayscenemanager::renderActiveScenes() {
			std::deque<std::string> temp(activeScenes);
			for (auto &i : temp) 
				if (scenes.find(i) != scenes.end())
					scenes[i].call(*this, activeScenes.back() == i);
		}

		// 
		// Shared vars
		//
		void        rayscenemanager::addVar(std::string varName, std::type_index varType, void *varValue) { 
			accessibleVars[varName] = rayscenevar(varValue);
		}
		void        rayscenemanager::addVar(std::string varName, rayscenevar var) { 
			accessibleVars[varName] = var;
		}
		rayscenevar rayscenemanager::getVar(std::string varName) { 
			return accessibleVars[varName];
		}

		//
		// Private utility functions
		//
		template<typename T>
		void rayscenemanager::clear_deque(std::deque<T> &q) {
			std::deque<T> empty;
			std::swap(q, empty);
		}
		template<typename K, typename V>
		void rayscenemanager::clear_unordered_map(std::unordered_map<K, V> &q) {
			std::unordered_map<K, V> empty;
			std::swap(q, empty);
		}
}
