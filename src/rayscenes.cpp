#include "rayscenes.hpp"

#include <deque>
#include <unordered_map>
#include <string>
#include <any>
#include <functional>
#include <tuple>

namespace rayscenes {
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
		void rayscenemanager::rayscene::call(rayscenemanager &rsm, bool isTop) { if (funcSet) func(rsm, isTop); }


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
		std::unordered_map<std::string, rayscenemanager::rayscene>& rayscenemanager::getAllScenes() { 
			return scenes; 
		}
		void                                                        rayscenemanager::setAllScenes(std::unordered_map<std::string, rayscene> newScenes) { 
			for (auto &[key, scene] : newScenes) scenes[key] = scene; 
		}
		rayscenemanager::rayscene&                                  rayscenemanager::getScene(std::string id) { 
			return scenes[id]; 
		}
		void                                                        rayscenemanager::addScene(std::string id, rayscene scene) { 
			scenes[id] = scene; 
		}
		void                                                        rayscenemanager::addScene(std::string id, rayscenefunc func) { 
			scenes[id] = rayscene(func);
		}
		void                                                        rayscenemanager::removeScene(std::string id) { 
			scenes.erase(id); 
		}

		//
		// Active scene management
		//
		std::deque<std::string>& rayscenemanager::getActiveScenes() { return activeScenes; }
		void                     rayscenemanager::setActiveScenes(std::deque<std::string> ids) {
			clear_deque(activeScenes);
			for (auto &id : ids)
				activeScenes.push_back(id);
		}
		void                     rayscenemanager::setActiveScene(std::string id) {
			clear_deque(activeScenes);
			activeScenes.push_back(id);
		}
		void                     rayscenemanager::addActiveScene(std::string id) { 
			activeScenes.push_back(id); 
		}
		void                     rayscenemanager::addActiveScenes(std::deque<std::string> ids) { 
			for (auto &id : ids) 
				activeScenes.push_back(id); 
		}
		void                     rayscenemanager::removeActiveScene(std::string id) { 
			activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); 
		}
		void                     rayscenemanager::removeActiveScenes(std::deque<std::string> ids) {
			for (auto &id : ids)
				activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end());
		}
		void                     rayscenemanager::clearActiveScenes(std::string id) { 
			activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); 
		}

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
