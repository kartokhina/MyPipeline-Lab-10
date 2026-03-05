# MyPipeline-Lab-10
# Лабораторная 9
ФИО: Картохина Виктория Алексеевна 

Группа: СКБ252


## Цель работы
Реализовать шаблонный класс MyPipeline<T> - конвейер обработки данных типа T.
Конвейер состоит из шагов. Каждый шаг преобразует значение по правилу T -> T.

## Ограничения
1. Класс обязан быть шаблонным: template<class T> class MyPipeline.
2. Разрешено использовать: std::vector, std::function, стандартные исключения.
3. Запрещено использовать готовые библиотеки/классы pipeline.
4. Индекс шага отдельно не хранится. Индекс шага - это позиция шага в контейнере (0..size-1).

##  Описание алгоритма
### Представление шага
Каждый шаг должен хранить:
- name: std::string
- func: вызываемый объект, который принимает T и возвращает T (лямбда/функтор/функция)

### Требуемый интерфейс MyPipeline<T>
#### Базовые методы
- MyPipeline();
- size_t size() const;
- bool empty() const;
- void clear();


#### Добавление шага
template<class F>
void addStep(const std::string& name, F func);

Условия:
- если name пустая строка, выбросить std::invalid_argument
- шаг добавляется в конец


#### Удаление шага
void removeStep(size_t index);

Условия:
- если index вне диапазона [0; size()), выбросить std::out_of_range


#### Запуск конвейера
T run(T value) const;

Условия:
- последовательно применить все шаги к value
- если шагов нет, вернуть value без изменений


#### Трассировка
std::vector<T> trace(T value) const;

Условия:
- вернуть значения результата после каждого шага по порядку
- если шагов нет, вернуть пустой vector


### Вывод в поток
Перегрузить operator<< для вывода MyPipeline<T> в удобочитаемом виде.
Требования к выводу:
- количество шагов
- список шагов: "i) name" для каждого шага, где i - индекс (позиция в контейнере)

### Требования к main()
В main() реализовать:

#### Демонстрация MyPipeline<int>
- добавить минимум 3 шага
- вывести pipeline через cout
- вывести результат run для одного входного значения
- вывести все элементы trace для того же входного значения

#### Демонстрация MyPipeline<std::string>
- добавить минимум 3 шага
- вывести результат run для одной входной строки

#### Демонстрация исключения
- оформить try/catch и показать перехват одного исключения на выбор:
  removeStep(100) -> std::out_of_range
  addStep("", ...) -> std::invalid_argument




##  Код программы
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <functional>


template<class T>
class MyPipeline{
    private:
        struct Step{
            std::string name;
            std::function<T(T)> func;
        };

        std::vector<Step> steps;

    public:
        MyPipeline() {}

        size_t size() const{
            return steps.size();
        }

        bool empty() const{
            return steps.empty();
        }

        void clear(){
            steps.clear();
        }

        template<class F>
        void addStep(const std::string& name, F func){
            if (name.empty()){ throw std::invalid_argument("name is empty"); }
            Step s;
            s.name = name;
            s.func = func;
            steps.push_back(s);
        }

        void removeStep(size_t index){
            if (index >= steps.size()){
                throw std::out_of_range("index out of range");
            }
            steps.erase(steps.begin() + index);
        }

        T run(T value) const{
            for(const auto& step : steps){
                value = step.func(value);
            }
            return value;
        }

        std::vector<T> trace(T value) const{
            std::vector<T> result;
            for(const auto& step : steps){
                value = step.func(value);
                result.push_back(value);
            }
            return result;   
        }

        friend std::ostream& operator<<(std::ostream& os, const MyPipeline<T>& p) { 
            os << "Pipeline steps (" << p.steps.size() << "): " << std::endl;
            for (size_t i = 0; i < p.steps.size(); i++) {
                os << i+1 << ") " << p.steps[i].name << std::endl;
            }   

            return os;
        }
};

int main(){
    MyPipeline<int> IntPipeline;

    IntPipeline.addStep("Add 5", [](int x){return x+5;});
    IntPipeline.addStep("Multiply by 2", [](int x){return x*2;});
    IntPipeline.addStep("Subtract 3", [](int x){return x-3;});

    std::cout << IntPipeline << std::endl;

    int value = 10;
    std::cout << "Run:" << IntPipeline.run(value) << std::endl;


    std::cout << "Trace: "<< std::endl;
    auto trace = IntPipeline.trace(value);

    for (auto v : trace) {
        std::cout << v << std::endl;
    }
    std::cout << "-----------------" << std::endl;
    //////////////////////////////
    MyPipeline<std::string>StrPipeline;

    StrPipeline.addStep("Add Hello", [](std::string x){return "Hello, "+x;});
    StrPipeline.addStep("Add exclamation", [](std::string x){return x+"!";});
    StrPipeline.addStep("Add smile", [](std::string x){return x+" :)";});
   
    std::string s = "World";
    std::cout << StrPipeline.run(s) << std::endl;

    
    std::cout << "-----------------" << std::endl;
    //////////////////////////////

    try
    {
        StrPipeline.removeStep(100);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

}

```

## Выводит
```cpp
Pipeline steps (3): 
1) Add 5
2) Multiply by 2
3) Subtract 3

Run:27
Trace:
15
30
27
-----------------
Hello, World! :)
-----------------
index out of range

```



##  Cсылка на репризиторий
Cсылка на репризиторий: https://github.com/kartokhina/MyPipeline-Lab-10

##  Вывод
В ходе выполнения лабораторной работы был реализован шаблонный класс MyPipeline<T>, представляющий собой конвейер обработки данных.
Программа демонстрирует работу конвейера для типов int и std::string, а также обработку исключений при ошибочных операциях.

