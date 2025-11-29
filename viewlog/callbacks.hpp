#ifndef CALLBACKMANAGEMENT
#define CALLBACKMANAGEMENT

#include <list>


std::list<std::function<void (GLFWwindow* , int , int , int , int )> > key_callbacks;

void register_key_callback(std::function<void (GLFWwindow* , int , int , int , int )> f) {
  key_callbacks.push_back(f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  for (auto f : key_callbacks)
    f(window, key, scancode, action, mods);
}

std::list<std::function<void (GLFWwindow* , int , int )> > mouse_pos_callbacks;

std::list<std::function<void (GLFWwindow* , int , int )> >::iterator register_mouse_pos_callback(std::function<void (GLFWwindow* , int , int )> f) {
  auto iter = mouse_pos_callbacks.insert(mouse_pos_callbacks.begin(), f);
  return iter;
}


template <typename T>
bool is_iterator_in_list(const std::list<T>& list, 
                             const typename std::list<T>::const_iterator& it_original) {
    for (auto it_new_traversal = list.cbegin(); 
         it_new_traversal != list.cend(); 
         ++it_new_traversal) {
        
        if (it_new_traversal == it_original) {
            return true;
        }
    }

    return false;
}

void unregister_mouse_pos_callback(std::list<std::function<void (GLFWwindow* , int , int )> >::iterator f_handle) {
  if (is_iterator_in_list(mouse_pos_callbacks, f_handle))
    mouse_pos_callbacks.erase(f_handle);
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
  for (auto f : mouse_pos_callbacks)
    f(window, xpos, ypos);
}


std::list<std::function<void (GLFWwindow* , int , int, int )> > mouse_button_callbacks;

void register_mouse_button_callback(std::function<void (GLFWwindow* , int , int, int )> f) {
  mouse_button_callbacks.push_back(f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  for (auto f : mouse_button_callbacks)
    f(window, button, action, mods);
}

#endif
