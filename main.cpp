#include <SFML/Graphics.hpp>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;
using std::ios;


enum class FunctionType
{
    Linear,
    Quadric,
    Qubic,
    Log,
    E_x,
    Sin,
    Cos
};


float calculate_integral_left_rectangle(float (*f)(float), float a, float b, int n)
{
    float dx = (b - a) / n;
    //std::cout << dx << std::endl;
    float x_i = a;
    float ans = 0;
    while (x_i < b) {
        //float x_i1 = x_i + dx;
        //float dy = f(a, b, c, x_i1) - f(a, b, c, x_i);
        ans += f(x_i) * dx;
        x_i += dx;
    }

    return ans;
}


float calculate_integral_right_rectangle(float (*f)(float), float a, float b, int n)
{
    float dx = (b - a) / n;
    //std::cout << dx << std::endl;
    float x_i = a;
    float ans = 0;
    while (x_i < b) {
        float x_i1 = x_i + dx;
        //float dy = f(a, b, c, x_i1) - f(a, b, c, x_i);
        ans += f(x_i1) * dx;
        x_i += dx;
    }

    return ans;
}


float calculate_integral_middle_rectangle(float (*f)(float), float a, float b, int n)
{
    float dx = (b - a) / n;
    //std::cout << dx << std::endl;
    float x_i = a;
    float ans = 0;
    while (x_i < b) {
        float x_i1 = x_i + dx;
        //float dy = f(a, b, c, x_i1) - f(a, b, c, x_i);
        ans += f((x_i1 + x_i) / 2) * dx;
        x_i += dx;
    }

    return ans;
}


float calculate_integral_trapezoid(float (*f)(float), float a, float b, int n)
{
    float dx = (b - a) / n;
    //std::cout << dx << std::endl;
    float x_i = a;
    float ans = 0;
    while (x_i < b) {
        float x_i1 = x_i + dx;
        float dy = f(x_i1) - f(x_i);
        ans += (dx * dy) / 2 + f(x_i) * dx;
        x_i += dx;
    }

    return ans;
}


class Function
{
private:
    FunctionType type;
    float* parameters;

public:
    Function(FunctionType t, int size, float* params);
    float calcualate_ranged_integral(float a, float b);
    FunctionType get_type() { return this->type; }
};

Function::Function(FunctionType t, int size, float* params)
{
    type = t;
    parameters = new float[size];

    for (int i = 0; i < size; i++)
    {
        parameters[i] = params[i];
    }
}

float Function::calcualate_ranged_integral(float a, float b)
{
    switch (this->get_type())
    {
    case FunctionType::Linear:
        
        break;

    case FunctionType::Quadric:

        break;
    };
}


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Graphics");

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}