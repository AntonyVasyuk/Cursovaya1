#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

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
    A_x,
    Sin,
    Cos
};


float calculate_integral_left_rectangle(std::function<float(float)> f, float a, float b, int n)
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
    std::function<float(float)> f;
    float* parameters;

public:
    Function(FunctionType t, float* params)
    {
        this->type = t;
        int size;
        switch (t)
        {
        case FunctionType::Linear:
            size = 2;

            break;
        case FunctionType::Quadric:
            size = 3;
            break;
        case FunctionType::Qubic:
            size = 4;
            break;
        case FunctionType::Sin:
            size = 4;
            break;
        case FunctionType::Cos:
            size = 4;
            break;
        case FunctionType::Log:
            size = 5;
            break;
        case FunctionType::A_x:
            size = 5;
            break;
        }

        this->parameters = new float[size];

        for (int i = 0; i < size; i++)
        {
            this->parameters[i] = params[i];
        }
    }

    //5648678777777777777777777777777777777777777777777777777777777777777
    float calcualate_ranged_integral(float x1, float x2, int n)
    {
        switch (this->get_type())
        {
        case FunctionType::Linear:
            float k = this->parameters[0], b = this->parameters[1];
            return calculate_integral_left_rectangle([k, b](float x) {return k * x + b;}, x1, x2, n);

        case FunctionType::Quadric:
            float a = this->parameters[0], b = this->parameters[1], c = this->parameters[2];
            return calculate_integral_left_rectangle([a, b, c](float x) {return a * x * x + b * x + c; }, x1, x2, n);

        case FunctionType::Qubic:
            float a = this->parameters[0], b = this->parameters[1], c = this->parameters[2], d = this->parameters[3];
            return calculate_integral_left_rectangle([a, b, c, d](float x) {return a * x * x * x + b * x * x + c * x + d; }, x1, x2, n);
        };
    }
    FunctionType get_type() { return this->type; }

    void add_graphic_to_vector(std::vector <sf::VertexArray>& v, float x1, float x2, int n)
    {
        float dx = (x2 - x1) / n;
        //std::cout << dx << std::endl;
        float x_i = x1;
        float ans = 0;
        while (x_i < x2) {
            float x_i1 = x_i + dx;
            //float dy = f(a, b, c, x_i1) - f(a, b, c, x_i);
            ans += f(x_i1) * dx;
            x_i += dx;
        }
    }
};


class Camera
{
private:
    float x;
    float y;

public:
    Camera(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void move(int dx, int dy)
    {
        this->x += dx;
        this->y += dy;
    }

    std::pair<int, int> calculate_position(int x, int y)
    {
        return std::make_pair(x - this->x, y - this->y);
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Graphics");

    Camera camera(-20, -580);
    bool moving_camera = false;
    std::pair<int, int> previous_position;

    std::vector <sf::VertexArray> lines;

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                moving_camera = true;
                sf::Vector2i v = sf::Mouse::getPosition();
                previous_position = std::make_pair(v.x, v.y);
            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                moving_camera = false;
            }

            if (moving_camera)
            {
                sf::Vector2i v = sf::Mouse::getPosition();
                camera.move(-v.x + previous_position.first, -v.y + previous_position.second);

                previous_position = std::make_pair(v.x, v.y);
            }
        }

        window.clear(sf::Color({200, 200, 200}));

        std::pair<int, int> c = camera.calculate_position(0, 0);
        //circle.setPosition({ (float)c.first, (float)c.second });

        sf::VertexArray line1(sf::PrimitiveType::Lines, 2);
        line1[0].position = sf::Vector2f((float)c.first - 200, (float)c.second);
        line1[1].position = sf::Vector2f((float)c.first + 760, (float)c.second);

        line1[0].color = sf::Color({ 200, 200, 200 });
        line1[1].color = sf::Color({ 0, 0, 0 });

        sf::VertexArray line2(sf::PrimitiveType::Lines, 2);
        line2[0].position = sf::Vector2f((float)c.first, (float)c.second + 200);
        line2[1].position = sf::Vector2f((float)c.first, (float)c.second - 560);

        line2[0].color = sf::Color({ 200, 200, 200 });
        line2[1].color = sf::Color({ 0, 0, 0 });

        window.draw(line1);
        window.draw(line2);

        //window.draw(circle);

        window.display();
    }
}