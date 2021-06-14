#ifndef PERFECTHASHSET_H
#define PERFECTHASHSET_H

#include <cstdint>
#include <random>
#include <vector>
#include <stdexcept>
#include <iostream>

const int32_t HASH_MOD = 6972593; //просте число
//const int32_t HASH_MOD = 100000;


template<typename T>
int32_t getHashValue(const T& object);

template<typename T>
T getRandomInt(T min, T max)
{
    static std::default_random_engine generator;
    static std::uniform_int_distribution<T> distribution(min, max);
    return distribution(generator);
}


/**
 * Вторинна хеш-таблиця
 */
template<typename T>
struct PerfectHashSetSecondary
{
public:
    PerfectHashSetSecondary(const std::vector<T>& set, int a, int b);

    /**
     * Повертає true, якщо такий же елемент вже знаходиться у цій таблиці
     */
    bool contains(const T& element);

    /**
     * Повертає елемент з таким значенням хеш-функції, якщо він знаходиться у цій таблиці.
     * Якщо такого елемента не існує, видає помилку.
     */
    const T& getByHash(int32_t hashValue);

    void printInfo();

private:
    std::vector<T> data;
    int a;
    int b;
    /**
     * Перетворює хеш обєкта на хеш для зберігання в цій таблиці
     */
    int32_t computeSetHash(int32_t hashValue);
};

template<typename T>
class PerfectHashSet
{
public:
    PerfectHashSet(const std::vector<T>& data);
    ~PerfectHashSet();

    bool contains(const T& object);
    const T& getByHash(int32_t hashValue);

    void printInfo();

private:
    /**
     * Створює вторинну хеш-таблицю, підбирає коефіцієнти так, щоб уникнути колізій.
     */
    PerfectHashSetSecondary<T>* createSecondarySet(std::vector<T>& data);

    int32_t a;
    int32_t b;
    int32_t n;

    /**
     * Створюємо "стандартний елмемент" без параметрів конструктора.
     * Пусті комірки у вторинних хеш-таблицях будуть зберігати такі елементи.
     */
    static T standardElement;

    PerfectHashSetSecondary<T>** secondarySets = nullptr;
    int32_t computeSetHash(int32_t hashValue);

    friend class PerfectHashSetSecondary<T>;
};

template<typename T>
T PerfectHashSet<T>::standardElement = T();


template<typename T>
PerfectHashSetSecondary<T>::PerfectHashSetSecondary(const std::vector<T>& data, int a, int b)
: data(data), a(a), b(b)
{}



template<typename T>
PerfectHashSet<T>::PerfectHashSet(const std::vector<T>& data)
{
    //Перевіряємл, чи є елементи, що співпадають
    //Якщо так, то ідеальне хешування неможливе.
    //Також перевіряємо, чи міститься серед вхідних даних "стандартний елемент".
    for(size_t i = 0; i < data.size(); i++)
    {
        if(data[i] == PerfectHashSet<T>::standardElement)
            throw std::invalid_argument("Data vector contains the standard element");
        for(size_t j = i + 1; j < data.size(); j++)
        {
            if(getHashValue(data[i]) == getHashValue(data[j]))
                throw std::invalid_argument("Data vector has repeating elements");
        }
    }

    //Параметри первинної хеш-функції
    a = 514229; //бажано, щоб а було великим і простим
    b = getRandomInt<int32_t>(1, HASH_MOD);
    n = data.size();

    //std::cout << "A = " << a << ", B = " << b << ", n = " << n << std::endl;

    //Розподіляємо дані по ріхних вторинним таблицям.
    //Для початку розділяємо їх на n підмножин в залежності від
    //значення первинної хеш-функції
    std::vector<T>* secondarySetArrays = new std::vector<T>[n];
    secondarySets = new PerfectHashSetSecondary<T>*[n];

    for(const T& element : data)
    {
        int64_t hash64 = (a * (int64_t)getHashValue(element) + b) % HASH_MOD % n;
        //std::cout << "Hash for " << element.getName() << ": " << hash64 << std::endl;
        secondarySetArrays[hash64].push_back(element);
    }

    //std::cout << "Secondary set arrays created" << std::endl;

    //Створюмо вторинні хеш-таблиці для кожної підмножини
    for(size_t i = 0; i < n; i++)
    {
        if(!secondarySetArrays[i].empty())
            secondarySets[i] = this->createSecondarySet(secondarySetArrays[i]);
        else
            secondarySets[i] = nullptr;
    }

    //std::cout << "Done!" << std::endl;

    delete[] secondarySetArrays;
}

template<typename T>
PerfectHashSet<T>::~PerfectHashSet()
{
    delete secondarySets;
}

template<typename T>
int32_t PerfectHashSet<T>::computeSetHash(int32_t hashValue)
{
    return (int64_t(a) * int64_t(hashValue) + b) % HASH_MOD % n;
}

template<typename T>
int32_t PerfectHashSetSecondary<T>::computeSetHash(int32_t hashValue)
{
    return (int64_t(a) * int64_t(hashValue) + b) % HASH_MOD % data.size();
}


template<typename T>
PerfectHashSetSecondary<T>* PerfectHashSet<T>::createSecondarySet(std::vector<T>& data)
{
    //std::cout << "Creating secondary set..." << std::endl;
    int32_t a;
    int32_t b;
    int32_t m = data.size() * data.size(); //розмір вторинної таблиці - n^2

    std::vector<T> hashTableElements;
    hashTableElements.reserve(m);

    //Підбираємо коефіцієнти
    while(true)
    {
    loopStart:
        hashTableElements.clear();
        for(size_t i = 0; i < m; i++)
            hashTableElements.emplace_back();

        a = getRandomInt<int32_t>(10, HASH_MOD);
        b = getRandomInt<int32_t>(0, HASH_MOD);

        //std::cout << "Trying " << a << " " << b << std::endl;

        for(const T& element : data)
        {
            int32_t setHash = (int64_t(a) * int64_t(getHashValue(element)) + b) % HASH_MOD % m;
            //std::cout << "Inserting " << element << " with hash: " << setHash << std::endl;
            //Якщо в цій комірці вже зберігається нестандартний елемент, то це колізія.
            if(hashTableElements[setHash] != PerfectHashSet<T>::standardElement)
            {
                //std::cout << "Collision: Already contains " << hashTableElements[setHash] << std::endl;
                goto loopStart;
            }
            hashTableElements[setHash] = element;
        }
        //Якщо не виникло колізій, то підбір завершено.
        break;
    }
    PerfectHashSetSecondary<T>* secondarySet = new PerfectHashSetSecondary<T>(hashTableElements, a, b);
    //secondarySet->printInfo();
    return secondarySet;
}

template<typename T>
bool PerfectHashSet<T>::contains(const T& element)
{
    int32_t setHash = computeSetHash(getHashValue(element));
    if(secondarySets[setHash] == nullptr)
        return false;
    return secondarySets[setHash]->contains(element);
}


template<typename T>
const T& PerfectHashSet<T>::getByHash(int32_t hashValue)
{
    int32_t setHash = computeSetHash(hashValue);
    if(secondarySets[setHash] == nullptr)
        throw std::invalid_argument("Set does not contain element with this hash value");
    return secondarySets[setHash]->getByHash(hashValue);
}

template<typename T>
bool PerfectHashSetSecondary<T>::contains(const T& element)
{
    int32_t setHash = computeSetHash(getHashValue(element));
    return data[setHash] == element;
}

template<typename T>
const T& PerfectHashSetSecondary<T>::getByHash(int32_t hashValue)
{
    int32_t setHash = computeSetHash(hashValue);
    if(data[setHash] == PerfectHashSet<T>::standardElement)
        throw std::invalid_argument("Set does not contain element with this hash value");
    return data[setHash];
}


template<typename T>
void PerfectHashSet<T>::printInfo()
{
    std::cout << "a = " << this->a << ", b = " << this->b << ", m1 = " << HASH_MOD << ", m2 = " << n << std::endl;
    for(size_t i = 0; i < n; i++)
    {
        std::cout << "Secondary hash set " << i << ": ";
        if(secondarySets[i] == nullptr)
            std::cout << "null" << std::endl;
        else
        {
            std::cout << std::endl;
            secondarySets[i]->printInfo();
        }
    }
}

template<typename T>
void PerfectHashSetSecondary<T>::printInfo()
{
    std::cout << "   a = " << this->a << ", b = " << this->b << ", m1 = " << HASH_MOD << ", m2 = " << this->data.size() << std::endl;
    for(size_t i = 0; i < data.size(); i++)
    {
        std::cout << "    " << i << ": " << data[i] << " ";
        std::cout << "(Hash value: " << getHashValue(data[i]) << ")" << std::endl;
    }
}

#endif // PERFECTHASHSET_H
