#pragma once
#include <tuple>


// This class stores a method that is invoked whenever OnCallback is called.
// The method to be invoked can have any return type and any amount of parameters.
// It can be used to implement buttons or any other thing requiring this functionality.
template<class ReturnType, class Class, class... Params>
class Callback
{
	// Typedef our method pointer. 
	typedef ReturnType(Class::*Method)(Params...);

public:
	// A reference to the method and any parameters that should be used when the button is clicked must be supplied thorugh the constructor. 
    Callback(Class& class_ref, Method method, Params&... params_ref) :
        m_class(class_ref),
        m_method(method),
        m_params_tuple(std::forward_as_tuple(params_ref...)) // use std::forward_as_tuple since we then can allow references.
    {};

    ~Callback() {};

    // When this method is called we invoke the stored method.
    void OnCallback()
    {
        EventNotify(m_params_tuple, std::index_sequence_for<Params...>());
    }



private:
    Callback() {};

    // Invoke the stored method.
	template<std::size_t... Is>
	void EventNotify(const std::tuple<Params...>& tuple, std::index_sequence<Is...>)
	{
		(m_class.*m_method)(std::get<Is>(tuple)...);
	}


	Class& m_class;
	Method m_method;
	std::tuple<Params...> m_params_tuple;


};

