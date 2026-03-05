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



