#ifndef TMAP_H
#define TMAP_H

#include <vector>
#include <string>
#include <QDebug>

template <typename T>
class TMap : public std::vector<std::pair<T, std::string>>
{
protected:
    std::string m_name;
    T object;

    std::vector<std::pair<T, std::string>> m_data;
public:

    TMap(size_t init_size=2)
    {
        m_data.resize(init_size);
    };

    TMap(TMap&&) = default;
    TMap& operator=(TMap&&) = default;

    const T getObjectByName(const std::string& object_name)
    {
        for (const std::pair<T, std::string>& item : m_data) {
            if (item.second == object_name) {
                return item.first;
            }
        }

        qDebug() << "Object returned null";
        return T(NULL);
    }

    void push_back(T&& value, const std::string& m_name)
    {
        m_data.emplace_back(value, m_name);
    }
};

#endif // TMAP_H
