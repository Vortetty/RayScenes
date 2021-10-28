#include <deque>
#include <unordered_map>
#include <string>
#include <any>
#include <functional>
#include <tuple>
#include <variant>
#include <typeindex>

/**
 * @brief Create a rayscenemanager
 * 
 */
#define create_rsm() rayscenes::rayscenemanager rsm;
/**
 * @brief Create a rayscenemanager with a custom variable name
 * 
 */
#define create_rsm_with_name(name) rayscenes::rayscenemanager name;

/**
 * @brief Creates a rayscene and registers it
 * 
 */
#define register_rayscene(funcName, sceneName, ...) \
auto funcName = [](rayscenes::rayscenemanager &rsm, bool isTop) { \
  __VA_ARGS__ \
}; \
rsm.addScene(sceneName, funcName);
/**
 * @brief Creates a rayscene and registers it with a custom scene manager
 * 
 */
#define register_rayscene_custom_manager(sceneManager, funcName, sceneName, ...) \
auto funcName = [](rayscenes::rayscenemanager &rsm, bool isTop) { \
  __VA_ARGS__ \
}; \
sceneManager.addScene(sceneName, funcName);
/**
 * @brief Sets the active scene
 * 
 */
#define set_active_scene(sceneName) rsm.setActiveScene(sceneName);
/**
 * @brief Sets the active scene with a custom scene manager
 * 
 */
#define set_active_scene_custom_manager(sceneManager, sceneName) sceneManager.setActiveScene(sceneName);
/**
 * @brief Runs all active scenes
 * 
 */
#define run_active_scenes() rsm.renderActiveScenes();
/**
 * @brief Runs all active scenes with a custom scene manager
 * 
 */
#define run_active_scenes_custom_manager(sceneManager) sceneManager.renderActiveScenes();

/**
 * @brief RayScenes namespace
 * 
 */
namespace rayscenes {
	namespace {
		template<size_t N>
		struct StringLiteral {
			constexpr StringLiteral(const char (&str)[N]) {
				std::copy_n(str, N, value);
			}
			
			char value[N];
		};
	}

	/**
	 * @brief 
	 * 
	 */
	class rayscenevar {
	public:
		rayscenevar(void* _ptr) : ptr(_ptr) {}
		rayscenevar() {}

		template<typename T>
		T* getVal() { return (T*)ptr; }

		void* getVal() { return ptr; }

	private:
		void *ptr;
	};

	/**
	 * @brief RaySceneManager class, manages rayscenes
	 * 
	 */
	class rayscenemanager {
	public:

		/**
		 * @brief Type for the function for a rayscene, expands to `void (*)(rayscenes::rayscenemanager&, bool)`
		 * 
		 */
		using rayscenefunc = std::variant<void(*)(rayscenes::rayscenemanager&, bool), std::function<void(rayscenes::rayscenemanager&, bool)>>;

		/**
		 * @brief RayScene class, manages a single scene, it is mostly an abstraction for the scene functions
		 * 
		 */
        class rayscene {
        public:
			/**
			 * @brief Construct a new rayscene object
			 * 
			 * @param _func The function to call when the scene is active and should be rendered
			 */
            rayscene(rayscenefunc _func);

			/**
			 * @brief Construct a new rayscene object
			 * 
			 */
            rayscene();


			/**
			 * @brief Set/replace the function
			 * 
			 * @param _func The function to call when the scene is active and should be rendered
			 */
            void setFunc(rayscenefunc _func);

			/**
			 * @brief 
			 * 
			 * @param rsm The current rayscenemanager
			 * @param isTop If the scene is the top scene(the last one to be rendered)
			 */
            void call(rayscenemanager &rsm, bool isTop);

        private:
			/**
			 * @brief If the function has been set, prevents calling a null function
			 * 
			 */
            bool funcSet;

			/**
			 * @brief The function that will be called when the scene is active and should be rendered
			 * 
			 */
            rayscenefunc func;
        };

		/**
		 * @brief Construct a new rayscenemanager object
		 * 
		 */
		rayscenemanager();

		/**
		 * @brief Construct a new rayscenemanager object
		 * 
		 * @param r An old rayscenemanager to copy
		 */
		rayscenemanager(const rayscenemanager& r);


		/**
		 * @brief Get the map containing the scenes
		 * 
		 * @return std::unordered_map<std::string, rayscene>& Reference to map containing the scenes
		 */
		std::unordered_map<std::string, rayscene>& getAllScenes();

		/**
		 * @brief Replaces the map of scenes with a new one
		 * 
		 * @param newScenes The new map of scenes
		 */
		void setAllScenes(std::unordered_map<std::string, rayscene> newScenes);

		/**
		 * @brief Get a scene's object by id
		 * 
		 * @param id Id of the scene you want
		 * @return rayscene& Reference to the scene
		 */
		rayscene& getScene(std::string id);

		/**
		 * @brief Add a scene to the map
		 * 
		 * @param id The target id
		 * @param scene The scene to link the id to
		 */
		void addScene(std::string id, rayscene scene);

		/**
		 * @brief Add a scene to the map given a function not a scene
		 * 
		 * @param id The target id
		 * @param func The func to link the id to
		 */
		void addScene(std::string id, void (*func)(rayscenes::rayscenemanager&, bool) );
		/**
		 * @brief Add a scene to the map given a function not a scene
		 * 
		 * @param id The target id
		 * @param func The func to link the id to
		 */
		void addScene(std::string id, std::function<void(rayscenes::rayscenemanager&, bool)>& func);

		/**
		 * @brief Remove a scene from the map by id
		 * 
		 * @param id The target id
		 */
		void removeScene(std::string id);


		/**
		 * @brief Get a deque of scene ids representing the active scenes
		 * 
		 * @return std::deque<std::string>& Reference to the deque
		 */
		std::deque<std::string>& getActiveScenes();

		/**
		 * @brief Set the current active scenes
		 * 
		 * @param ids A deque of ids to set as active
		 */
		void setActiveScenes(std::deque<std::string> ids);

		/**
		 * @brief Disables all scenes sets a new one as active
		 * 
		 * @param id The target id
		 */
		void setActiveScene(std::string id);

		/**
		 * @brief Adds a new scene on top of the others
		 * 
		 * @param id The target id
		 */
		void addActiveScene(std::string id);

		/**
		 * @brief Adds scenes on top of the others given a deque of ids
		 * 
		 * @param ids A deque of ids to set as active
		 */
		void addActiveScenes(std::deque<std::string> ids);


		/**
		 * @brief Remove a scene from the active scenes list
		 * 
		 * @param id The target id
		 */
		void removeActiveScene(std::string id);

		/**
		 * @brief Remove scenes from the active scenes list given a deque of ids
		 * 
		 * @param ids A deque of ids to set as active
		 */
		void removeActiveScenes(std::deque<std::string> ids);

		/**
		 * @brief Clears all scenes from the list, effectively stopping rendering. It is recommended you always have one scene active or some other update loop to ensure the program stays working.
		 * 
		 * @param id The target id
		 */
		void clearActiveScenes(std::string id);


		/**
		 * @brief Renders all scenes that are active, it is safe to modify the list during this running as it first creates a copy on the deque before beginning rendering.
		 * 
		 */
		void renderActiveScenes();

		/**
		 * @brief Adds a variable to the list of variables to be passed to the active scenes
		 * 
		 * @param varName Name the variable should be accessed by
		 * @param varType std::type_index of the variable
		 * @param varValue Pointer to the variable as void*
		 */
		void addVar(std::string varName, std::type_index varType, void *varValue);

		/**
		 * @brief Adds a variable to the list of variables to be passed to the active scenes
		 * 
		 * @param varName Name the variable should be accessed by
		 * @param var Pre-contructed rayscenevar to store
		 */
		void addVar(std::string varName, rayscenevar var);

		rayscenevar getVar(std::string varName);

		template<typename T>
		T getVarVal(std::string varName) { 
			return *(accessibleVars[varName].getVal<T>());
		}
		template<typename T, StringLiteral varName>
		T getVarVal() { 
			return *(accessibleVars[std::string(varName.value)].getVal<T>());
		}
	private:
		/**
		 * @brief The map of all scenes
		 * 
		 */
		std::unordered_map<std::string, rayscene> scenes;

		/**
		 * @brief The list of currently active scenes
		 * 
		 */
		std::deque<std::string> activeScenes;

		/**
		 * @brief list of variables to be passed to the active scenes
		 * 
		 */
		std::unordered_map<std::string, rayscenevar> accessibleVars;

		/**
		 * @brief Clears a deque
		 * 
		 * @tparam T The type the deque stores, in general it is able to be deduced by the compiler
		 * @param q The deque to clear
		 */
		template<typename T>
		void clear_deque( std::deque<T> &q );
	
		/**
		 * @brief Clears a map of entries
		 * 
		 * @tparam K The type used for the key, in general it is able to be deduced by the compiler
		 * @tparam V The type used for the value, in general it is able to be deduced by the compiler
		 * @param q The map to clear
		 */
		template<typename K, typename V>
		void clear_unordered_map( std::unordered_map<K, V> &q );
	};

}