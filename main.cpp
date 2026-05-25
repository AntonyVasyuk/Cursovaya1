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


int SCALE = 50;
int SCALE_CHANGE = 2;


enum class FunctionType
{
    Linear,
    Quadric,
    Qubic,
    Sin,
    Cos,
    div_Log,
    Sinx_x,
    A_x
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


float calculate_integral_right_rectangle(std::function<float(float)> f, float a, float b, int n)
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


float calculate_integral_middle_rectangle(std::function<float(float)> f, float a, float b, int n)
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


float calculate_integral_trapezoid(std::function<float(float)> f, float a, float b, int n)
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


class Camera
{
private:
    float x;
    float y;
    int *scale;
    std::pair<int, int> range;
    int samples;

public:
    Camera(float x, float y, int *scale, int x2, int x1, int n)
    {
        this->x = x;
        this->y = y;
        this->scale = scale;
        this->range = std::make_pair(x2, x1);
        this->samples = n;
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

    std::pair<int, int> get_range()
    {
        return range;
    }

    int get_samples()
    {
        return samples;
    }

    int* get_scale()
    {
        return this->scale;
    }
};


class Function
{
private:
    FunctionType type;
    std::function<float(float)> f;
    //float* parameters;

public:
    Function(FunctionType t, float* params)
    {
        this->type = t;
        int size;
        //cout << "1\n";
        switch (t)
        {
            case FunctionType::Linear:
            {
                float k = params[0], b = params[1];
                this->f = [k, b](float x) { return k * x + b; };
                break;
            }
            case FunctionType::Quadric:
            {
                float a = params[0], b = params[1], c = params[2];
                this->f = [a, b, c](float x) {return a * x * x + b * x + c; };
                break;
            }
            case FunctionType::Qubic:
            {
                float a = params[0], b = params[1], c = params[2], d = params[3];
                this->f = [a, b, c, d](float x) {return a * x * x * x + b * x * x + c * x + d; };
                break;
            }
            case FunctionType::Sin:
            {
                float a = params[0], k = params[1], b = params[2], c = params[3];
                this->f = [a, k, b, c](float x) {return a * std::sin(k * x + b) + c; };
                break;
            }
            case FunctionType::Cos:
            {
                float a = params[0], k = params[1], b = params[2], c = params[3];
                this->f = [a, k, b, c](float x) {return a * std::cos(k * x + b) + c; };
                break;
            }
            case FunctionType::div_Log:
            {
                float l = params[0], k = params[1], b = params[2], c = params[3];
                this->f = [l, k, b, c](float x) {return l / std::log(k * x + b) + c; };
                break;
            }
            //case FunctionType::A_x:
            //{
            //    float l = params[0], a = params[1], k = params[2], b = params[3], c = params[4];
            //    this->f = [l, a, k, b, c](float x) {return l * std::log(k * x + b) + c; };
            //    break;
            //}
            case FunctionType::Sinx_x:
            {
                //float a = params[0], k = params[1], b = params[2], c = params[3];
                this->f = [](float x) {return std::sin(x) / x; };
                break;
            }
        }

        //this->parameters = new float[size];

        //for (int i = 0; i < size; i++)
        //{
        //    this->parameters[i] = params[i];
        //}
    }

    //5648678777777777777777777777777777777777777777777777777777777777777
    std::pair <float, float> calcualate_ranged_integral(float x1, float x2, int n)
    {
        return std::make_pair(calculate_integral_middle_rectangle(this->f, x1, x2, n), calculate_integral_trapezoid(this->f, x1, x2, n));
    }
    FunctionType get_type() { return this->type; }

    void add_graphic_to_vector(Camera camera, std::vector <sf::VertexArray>& v);
};


void Function::add_graphic_to_vector(Camera camera, std::vector <sf::VertexArray>& v)
{
    int x2 = camera.get_range().first, x1 = camera.get_range().second;
    int n = camera.get_samples();

    float dx = (x2 - x1) / n;
    //std::cout << dx << std::endl;
    float x_i = x1;
    while (x_i < x2) {
        float x_i1 = x_i + dx;

        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        auto c1 = camera.calculate_position(x_i * *camera.get_scale(), -this->f(x_i) * *camera.get_scale());
        auto c2 = camera.calculate_position(x_i1 * *camera.get_scale(), -this->f(x_i1) * *camera.get_scale());
        line[0].position = sf::Vector2f(c1.first, c1.second);
        line[1].position = sf::Vector2f(c2.first, c2.second);

        line[0].color = sf::Color({ 200, 0, 0 });
        line[1].color = sf::Color({ 200, 0, 0 });

        v.push_back(line);

        x_i += dx;
    }
}


void draw_fancy(sf::RenderWindow& window, std::pair<int, int>& c, std::vector <sf::VertexArray>& fancy_lines, Camera camera)
{
    sf::VertexArray line1(sf::PrimitiveType::Lines, 2);
    line1[0].position = sf::Vector2f((float)c.first - 200, (float)c.second);
    line1[1].position = sf::Vector2f((float)c.first + 2000, (float)c.second);

    line1[0].color = sf::Color({ 200, 200, 200 });
    line1[1].color = sf::Color({ 0, 0, 0 });

    sf::VertexArray line2(sf::PrimitiveType::Lines, 2);
    line2[0].position = sf::Vector2f((float)c.first, (float)c.second + 200);
    line2[1].position = sf::Vector2f((float)c.first, (float)c.second - 2000);

    line2[0].color = sf::Color({ 200, 200, 200 });
    line2[1].color = sf::Color({ 0, 0, 0 });

    window.draw(line1);
    window.draw(line2);

    fancy_lines.clear();
    for (int i = 1; i < 67; i++)
    {
        {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            line[0].position = sf::Vector2f((float)c.first + i * *camera.get_scale(), (float)c.second + 5);
            line[1].position = sf::Vector2f((float)c.first + i * *camera.get_scale(), (float)c.second - 5);

            line[0].color = sf::Color({ 0, 0, 0 });
            line[1].color = sf::Color({ 0, 0, 0 });
            fancy_lines.push_back(line);
        }
        {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            line[0].position = sf::Vector2f((float)c.first + 5, (float)c.second - i * *camera.get_scale());
            line[1].position = sf::Vector2f((float)c.first - 5, (float)c.second - i * *camera.get_scale());

            line[0].color = sf::Color({ 0, 0, 0 });
            line[1].color = sf::Color({ 0, 0, 0 });
            fancy_lines.push_back(line);
        }
    }

    for (auto line : fancy_lines)
    {
        window.draw(line);
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Graphics");

    int *scale = &SCALE;

    Camera camera(-20, -580, scale, -10, 10, 200);
    bool moving_camera = false;
    std::pair<int, int> previous_position;

    //int scale = SCALE;

    float params[3];

    //params[0] = 0.1;
    //params[1] = 0;
    //params[2] = 1;

    //Function f(FunctionType::Quadric, params);

    //params[0] = 1;
    //params[1] = 1;
    //params[2] = 0;
    //params[3] = 0;

    //Function f(FunctionType::Sin, params);

    params[0] = 1;
    params[1] = 1;
    params[2] = 0;
    params[3] = 0;

    Function f(FunctionType::div_Log, params);

    //float params[2];

    //params[0] = 2;
    //params[1] = 2;

    //Function f(FunctionType::Linear, params);

    std::vector <sf::VertexArray> lines;
    std::vector <sf::VertexArray> fancy_lines;

    //f.add_graphic_to_vector(camera, lines, -2, 2, 10);

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

            if (event->is<sf::Event::MouseWheelScrolled>())
            {
                int d = event->getIf<sf::Event::MouseWheelScrolled>()->delta;

                //cout << d << '\n';
                *scale += SCALE_CHANGE * d;
                //camera.set_scale(scale);
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

        lines.clear();
        f.add_graphic_to_vector(camera, lines);

        draw_fancy(window, c, fancy_lines, camera);

        for (auto line : lines)
        {
            window.draw(line);
        }

        window.display();
    }
}
