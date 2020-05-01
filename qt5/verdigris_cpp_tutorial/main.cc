//
// Created by rick on 2020/4/26.
//
#include "wobjectcpp.h"
#include "wobjectimpl.h"
#include <QObject>
#include <QVariant>
#include <tuple>

template <typename V, typename... Tags> struct Strong { V v; };

using Name = Strong<QString,struct NameTag>;
using BirthYear = Strong<int, struct BirthYearTag>;

template<class T>
struct Wrap {};

constexpr auto getPropertyName(Wrap<Name>) {return w_cpp::viewLiteral("name");}
constexpr auto getPropertyName(Wrap<BirthYear>){return w_cpp::viewLiteral("birthYear");}

constexpr auto getPropertyChangedName(Wrap<Name>) {
    return w_cpp::viewLiteral("nameChanged");
}

constexpr auto getPropertyChangedName(Wrap<BirthYear>) {
    return w_cpp::viewLiteral("birthYearChanged");
}

template<class... Properties>
class GenericPropertyHolder: public QObject {
    W_OBJECT(GenericPropertyHolder)

    using Storage = std::tuple<Properties...>;
    Storage m_storage{};

    template<size_t I>
    using PropertyAt = std::remove_reference_t<decltype(std::get<I>(std::declval<Storage>()))>;

  public:
    GenericPropertyHolder(Properties... properties,QObject* parent = nullptr):QObject{parent},m_storate{std::move(properties)...} {}

  private:
    template<size_t I>
    void propertyChanged() {
        W_CPP_SIGNAL_IMPL(decltype(&GenericPropertyHolder::propertyChanged<I>),PropertyChangedSignals,I,0);
    }

    template<size_t I,class = std::enable_if_t<(I < sizeof...(Properties))>>
    struct PropertyChangedSignals {
        using Property = PropertyAt<I>;
        constexpr static auto signalName = getPropertyChangedName(Wrap<Property>{});
        constexpr static auto signal = w_cpp::makeSignalBuilder(signalName,&GenericPropertyHolder::propertyChanged<I>).build();
    };

    W_CPP_SIGNAL(PropertyChangedSignals)

};


