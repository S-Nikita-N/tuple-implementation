#include <iostream>
#include <string>



using namespace std;


template <typename... Types>
class Tuple {
public:
    template<typename... Types2>
    Tuple<>& operator = (const Tuple<Types2...>&){
        return *this;
    }
};

template<typename... Types>
class mytuple_size;

template<unsigned idx, typename... Types>
class mytuple_element;

template<unsigned idx, typename T>
class GetHelper {};

//#pragma pack(push,1)
template<typename T, typename... Types>
class Tuple<T, Types...> {
public:
    
    T current;
    Tuple<Types...> other;
    
    
public:
    
    Tuple(const T& c, const Types&... t): current(c), other(t...){}
    
    Tuple(const Tuple& t): current(t.current), other(t.other){}
    
    const T& get_current () const {

        return current;
    }

    const Tuple<Types...>& get_other () const {

        return other;
    }
    
    Tuple(){}



    Tuple<T, Types...>& operator = (const Tuple<T, Types...>& src){

        this->other = src.other;
        this->current = src.current;
        return *this;
    }
    
    template<typename... Types2>
    Tuple& operator = (Tuple<T,Types2...>& src){

        this->other = src.other;
        this->current = src.current;
        return *this;
    }
    
    Tuple& operator = (Tuple<>& src){
        return *this;
    }
    
        
};




//#pragma pack(pop)

// ----------- tuple_size -------------

template<typename... Types>
class mytuple_size<Tuple<Types...>>{
public:
    static const unsigned int value;
};

template<typename... Types>
const unsigned int mytuple_size<Tuple<Types...>> :: value = sizeof...(Types);

// --------------- end ---------------


// ---------- tuple_element ----------

template<unsigned idx,typename T, typename... Types>
class mytuple_element<idx,Tuple<T, Types...>>{
public:
    using type = typename mytuple_element<idx-1,Tuple<Types...>>::type;
};

template<typename T, typename... Types>
class mytuple_element<0,Tuple<T, Types...>>{
public:
    using type = T;
};

// --------------- end ---------------

// -------------- cout ---------------

template<typename T, typename... Types>
std::ostream& operator<<(std::ostream& strm, Tuple<T, Types...> const& t){
    
    strm<<t.get_current()<<" "<<t.get_other();
    return strm;
}

template<typename... Types>
std::ostream& operator<<(std::ostream& strm, Tuple<Types...> const& t){
    
  return strm;
}

// --------------- end ---------------

// ------------- fabric --------------
template <typename... Types>
Tuple<Types...> make_mytuple(Types&&... t){
    
    return Tuple<Types...> (t...);
}

// --------------- end ---------------

// ------------- my_tie --------------

template <typename... Types>
Tuple<Types&...> my_tie(Types&... t){
    return Tuple<Types&...> (t...);
}

// --------------- end ---------------

// -------------- Get ----------------

template<unsigned idx,typename... Types>
auto get(Tuple<Types...> &t) {
    return GetHelper<idx, Tuple<Types...>>::get(t);
}

template<unsigned idx, typename T, typename... Types>
class GetHelper<idx, Tuple<T, Types...>> {
public:
    static auto get(Tuple<T, Types...> &t) {
        return GetHelper<idx - 1, Tuple<Types...>>::get(t.get_other());
    }
};

template<typename T,typename... Types>
class GetHelper<0, Tuple<T, Types...>> {
public:
    static T const& get(Tuple<T, Types...> const& t) {
        return t.get_current();
    }
};

// --------------- end ---------------

// ----------- tuple_cat -------------

class __trashholder{};

template <typename... Types1, typename... Types2>
auto cat(const Tuple<Types1...>&  t_1, const Types2&... rest){
    return CatHelper(t_1, rest..., __trashholder());
}

template <typename... Types1, typename... Types2>
auto CatHelper(const Tuple<Types1...>&  t_1, const Types2&... rest)  {

    return CatHelper(t_1.get_other(), rest..., t_1.get_current());
}

template <typename... Types1, typename... Types2>
auto CatHelper(const Tuple<> ignore, const Tuple<Types1...>& t_1, const Types2&... rest)  {

    return CatHelper(t_1.get_other(), rest..., t_1.get_current());
}


template <typename... Types>
auto CatHelper(const Tuple<> ignore ,__trashholder, const Types&... rest)  {
    return make_mytuple(rest...);
}

// --------------- end ---------------



int main() {
    
    auto tuple_1 = make_mytuple(9, 3.5);
    cout<<"Second element of tuple_1: "<<get<1>(tuple_1)<<endl;
    
    auto tuple_2 = make_mytuple((const char*)"aaa",true);
    auto tuple_3 = make_mytuple((const char*)"ooo",false);
    tuple_2 = tuple_3;
    cout<<"tuple_2 is: "<<tuple_2<<endl;
    
    typedef Tuple<int,char,Tuple<bool,double>> TU;
    cout<<"Size of Tuple<int,char,double> : "<<mytuple_size<TU>::value<<endl;
    
    cout<<typeid(mytuple_element<0, TU>::type).name()<<" "<<typeid(mytuple_element<1, TU>::type).name()<<" "<<typeid(mytuple_element<2, TU>::type).name()<<endl;
    

    cout<<endl<<endl;
    
    cout<<sizeof(Tuple<>)<<endl; // 1
    cout<<sizeof(Tuple<bool>)<<endl; // 2 1-1
    cout<<sizeof(Tuple<bool,bool>)<<endl; // 3 1-1-1
    cout<<sizeof(Tuple<bool,bool,bool>)<<endl; // 4 1-1-1-1
    cout<<sizeof(Tuple<bool,bool,int>)<<endl; // 16 1(3) - 1(3) - 4 - 1(3)
    cout<<sizeof(Tuple<int>)<<endl; // 8
    cout<<sizeof(Tuple<int,int>)<<endl; // 12
    cout<<sizeof(Tuple<int,int,int>)<<endl; // 16
    cout<<sizeof(Tuple<bool,int>)<<endl; // 12
    cout<<sizeof(Tuple<bool,int,int>)<<endl; // 16
    cout<<sizeof(Tuple<bool,int,int,bool>)<<endl; // 16
    cout<<sizeof(Tuple<bool,int,int,bool,bool>)<<endl; // 16
    cout<<sizeof(Tuple<double, int, bool,short>)<<endl; // 24
    //
    
    
    auto tuple_4  = cat(make_mytuple(tuple_1), tuple_2,tuple_3);
    cout<<tuple_4<<endl;
    
    
    auto tuple_7 = make_mytuple(90,8.8, 'd');
    auto tuple_8 = make_mytuple(7,1.2);

    tuple_7 = tuple_8;
    cout<<tuple_7<<endl;
    
    auto tuple_9 = make_mytuple('a',2,3.8);
    auto tuple_10 = make_mytuple('c',6);
    
    tuple_10 = tuple_9;
    cout<<tuple_10<<endl;
}
