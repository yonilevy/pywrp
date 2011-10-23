namespace py
{

template<typename T>
ObjectPtr Object::from(const std::vector<T>& arg, bool shouldRelease /* = true */)
{
    ObjectPtr res(make_ptr(PyList_New(arg.size()), shouldRelease));
    int i = 0;
    for (auto it = arg.begin() ; it != arg.end() ; ++it, ++i) {
        PyList_SetItem(res->raw(), i, from(*it, false)->raw());
    }
    return res;
}

template<typename T> T Object::as() {
    STATIC_ASSERT_FAILED_cant_convert_to_given_type();
}

template<> std::string Object::as() {
    return asString();
}

template<> long Object::as() {
    return asLong();
}

template<> ObjectPtrList Object::as() {
    return asList();
}

template<typename Z>
Z Object::get(const std::string& name) const
{
    return get(name)->as<Z>();
}

////////////////////////////////////////////////////////////////////////////////

template<typename A>
ObjectPtr Object::call(const std::string& func, A arg1)
{
    std::vector<ObjectPtr> vec;
    vec.push_back(from(arg1));
    return callWithArgs(func, vec);
}

template<typename A, typename B>
ObjectPtr Object::call(const std::string& func, A arg1, B arg2)
{
    std::vector<ObjectPtr> vec;
    vec.push_back(from(arg1));
    vec.push_back(from(arg2));
    return callWithArgs(func, vec);
}

template<typename A, typename B, typename C>
ObjectPtr Object::call(const std::string& func, A arg1, B arg2, C arg3)
{
    std::vector<ObjectPtr> vec;
    vec.push_back(from(arg1));
    vec.push_back(from(arg2));
    vec.push_back(from(arg3));
    return callWithArgs(func, vec);
}

////////////////////////////////////////////////////////////////////////////////

template<typename Z> 
Z Object::call(const std::string& func)
{
    return call(func)->as<Z>();
}

template<typename Z, typename A> 
Z Object::call(const std::string& func, A arg1)
{
    return call(func, arg1)->as<Z>();
}

template<typename Z, typename A, typename B> 
Z Object::call(const std::string& func, A arg1, B arg2)
{
    return call(func, arg1, arg2)->as<Z>();
}

template<typename Z, typename A, typename B, typename C> 
Z Object::call(const std::string& func, A arg1, B arg2, C arg3)
{
    return call(func, arg1, arg2, arg3)->as<Z>();
}

} // namespace py
