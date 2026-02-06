#pragma once

// TypesPack
template <unsigned dev_i, template <typename> class DevTemplate, typename... RegRefTs>
struct IndexedDevPack;

template <unsigned dev_i, template <typename> class DevTemplate, typename FirstRef, typename... RestRefs>
struct IndexedDevPack<dev_i, DevTemplate, FirstRef, RestRefs...> {
    static_assert(dev_i > 0, "Index out of bounds");
    struct Dev : public IndexedDevPack<dev_i, DevTemplate, RestRefs...>::Dev {};
};

template <template <typename> class DevTemplate, typename FirstRef, typename... RestRefs>
struct IndexedDevPack<0, DevTemplate, FirstRef, RestRefs...> {
    struct Dev : public DevTemplate<FirstRef> {};
};

template <template <typename> class DevTemplate, typename... RegRefTs>
struct DevPack {
    template<unsigned dev_i>
    struct DevByIndex : public IndexedDevPack<dev_i, DevTemplate, RegRefTs...>::Dev {};
};
