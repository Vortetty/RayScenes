#pragma once

#include <deque>
#include <unordered_map>
#include <string>
#include <any>
#include <functional>
#include <tuple>

namespace rayscenes {
	typedef void(*rayscenefunc)(rayscenes::rayscenemanager&, bool);

	class rayscenemanager {
	public:
		
        class rayscene {
        public:
            rayscene(rayscenefunc _func) { func = _func; funcSet = true; }
            rayscene() {};

            void setFunc(rayscenefunc _func) { func = _func; funcSet = true; }
            void call(rayscenemanager &rsm, bool isTop) { if (funcSet) func(rsm, isTop); }

        private:
            bool funcSet = true;
            rayscenefunc func;
        };

		rayscenemanager() {}
		rayscenemanager(const rayscenemanager& r) {
			clear_unordered_map(scenes);
			clear_deque(activeScenes);

			for (auto &[key, scene] : ((rayscenemanager)r).getAllScenes()) scenes[key] = scene;
			for (auto &scene : ((rayscenemanager)r).getActiveScenes()) activeScenes.push_back(scene);
		}

		std::unordered_map<std::string, rayscene>& getAllScenes() { return scenes; }
		void setAllScenes(std::unordered_map<std::string, rayscene> newScenes) { for (auto &[key, scene] : newScenes) scenes[key] = scene; }
		rayscene& getScene(std::string id) { return scenes[id]; }
		void addScene(std::string id, rayscene scene) { scenes[id] = scene; }
		void addScene(std::string id, rayscenefunc func) { scenes[id] = rayscene(func); }
		void removeScene(std::string id) { scenes.erase(id); }

		std::deque<std::string>& getActiveScenes() { return activeScenes; }
		void setActiveScenes(std::deque<std::string> ids) {
			clear_deque(activeScenes);
			for (auto &id : ids) 
                activeScenes.push_back(id);
		}
		void setActiveScene(std::string id) { 
			clear_deque(activeScenes);
            activeScenes.push_back(id); 
        }
		void addActiveScene(std::string id) { activeScenes.push_back(id); }
		void addActiveScenes(std::deque<std::string> ids) { for (auto &id : ids) activeScenes.push_back(id); }

		void removeActiveScene(std::string id) { activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); }
		void removeActiveScenes(std::deque<std::string> ids) { 
            for (auto &id : ids) 
                activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); 
        }
		void clearActiveScenes(std::string id) { activeScenes.erase(std::remove(activeScenes.begin(), activeScenes.end(), id), activeScenes.end()); }

		void renderActiveScenes() {
			std::deque<std::string> temp(activeScenes);
            for (auto &i : temp) {
                if (scenes.find(i) != scenes.end()) 
                    scenes[i].call(
                        *this,
                        activeScenes.back() == i
                    );
            }
		}

	private:
		std::unordered_map<std::string, rayscene> scenes = {};
		std::deque<std::string> activeScenes = {};
	
		template<typename T>
		void clear_deque( std::deque<T> &q ) {
			std::deque<T> empty;
			std::swap( q, empty );
		}
	
		template<typename K, typename V>
		void clear_unordered_map( std::unordered_map<K, V> &q ) {
			std::unordered_map<K, V> empty;
			std::swap( q, empty );
		}
	};

}
