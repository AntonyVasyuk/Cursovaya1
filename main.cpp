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
int SAMPLE_CHANGE = 50;


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

enum class IntegralType
{
    LeftRectangle,
    RightRectangle,
    MiddleRectangle,
    Trapezoid
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
    int scale;
    std::pair<float, float> range;
    int samples;
    std::pair<float, float> integral_range;
    int integral_samples;
    IntegralType integral_type;

public:
    Camera(float x, float y, int scale, float x1, float x2, int n, float i_x1, float i_x2, int i_n, IntegralType integral_type)
    {
        this->x = x;
        this->y = y;
        this->scale = scale;
        this->range = std::make_pair(x1, x2);
        this->samples = n;

        this->integral_range = std::make_pair(i_x1, i_x2);
        this->integral_samples = i_n;
        this->integral_type = integral_type;
    }

    void move(int dx, int dy)
    {
        this->x += dx;
        this->y += dy;
    }

    void change_scale(int d)
    {
        if (this->scale + d > 0)
        {
            this->scale += d;
        }
    }

    void change_sample(int d)
    {
        if (this->samples + d > 0)
        {
            this->samples += d;
        }
    }

    std::pair<float, float> calculate_position(float x, float y) { return std::make_pair(x - this->x, y - this->y); }

    std::pair<float, float> get_range() { return range; }

    std::pair<float, float> get_integral_range() { return integral_range; }

    int get_integral_samples() { return integral_samples; }

    IntegralType get_integral_type() { return integral_type; }

    int get_samples() { return samples; } 

    int get_scale() { return this->scale; } 
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
    std::pair <float, float> calcualate_ranged_integral(std::vector <sf::VertexArray> &integral_rectangles, float x1, float x2, int n)
    {
        return std::make_pair(calculate_integral_middle_rectangle(this->f, x1, x2, n), calculate_integral_trapezoid(this->f, x1, x2, n));
    }
    FunctionType get_type() { return this->type; }

    void add_graphic_to_vector(Camera camera, std::vector <sf::VertexArray>& v);
    void add_integral_to_vector(Camera camera, std::vector <sf::ConvexShape>& v);
};


void Function::add_graphic_to_vector(Camera camera, std::vector <sf::VertexArray>& v)
{
    float x1 = camera.get_range().first, x2 = camera.get_range().second;
    int n = camera.get_samples();

    //int x1 = -10, x2 = 10, n = 200;

    //cout << x1 << ' ' << x2 << '\n' << n << '\n';

    float dx = (x2 - x1) / n;
    //std::cout << dx << std::endl;
    float x_i = x1;
    while (x_i < x2) {
        float x_i1 = x_i + dx;

        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        auto c1 = camera.calculate_position(x_i * camera.get_scale(), -this->f(x_i) * camera.get_scale());
        auto c2 = camera.calculate_position(x_i1 * camera.get_scale(), -this->f(x_i1) * camera.get_scale());
        line[0].position = sf::Vector2f(c1.first, c1.second);
        line[1].position = sf::Vector2f(c2.first, c2.second);

        line[0].color = sf::Color({ 200, 0, 0 });
        line[1].color = sf::Color({ 200, 0, 0 });

        v.push_back(line);
        //cout << "1\n";

        x_i += dx;
    }

    //cout << "1\n";
}


void Function::add_integral_to_vector(Camera camera, std::vector <sf::ConvexShape>& v)
{
    float x1 = camera.get_integral_range().first, x2 = camera.get_integral_range().second;
    int n = camera.get_integral_samples();
    IntegralType type = camera.get_integral_type();

    //int x1 = -10, x2 = 10, n = 200;

    //cout << x1 << ' ' << x2 << '\n' << n << '\n';

    auto c = camera.calculate_position(0, 0);

    float dx = (x2 - x1) / n;
    //std::cout << dx << std::endl;
    float x_i = x1;
    while (x_i < x2) {
        float x_i1 = x_i + dx;

        sf::ConvexShape rect;
        rect.setPointCount(4);
        auto c1 = camera.calculate_position(x_i * camera.get_scale(), -this->f(x_i) * camera.get_scale());
        auto c2 = camera.calculate_position(x_i1 * camera.get_scale(), -this->f(x_i1) * camera.get_scale());
        //cout << c2.first << ' ' << c2.second << '\n';
        if (type == IntegralType::Trapezoid)
        {
            rect.setPoint(0, { c1.first, c1.second });
            rect.setPoint(1, { c2.first, c2.second });
            rect.setPoint(2, { c2.first, c.second });
            rect.setPoint(3, { c1.first, c.second });
        }
        else if (type == IntegralType::MiddleRectangle)
        {
            rect.setPoint(0, { c1.first, (c1.second + c2.second) / 2 });
            rect.setPoint(1, { c2.first, (c1.second + c2.second) / 2 });
            rect.setPoint(2, { c2.first, c.second });
            rect.setPoint(3, { c1.first, c.second });
        }


        rect.setFillColor(sf::Color(0, 0, 150, 100));
        //rect[0].color = sf::Color({ 0, 0, 200 });
        //rect[1].color = sf::Color({ 0, 0, 200 });
        //rect[2].color = sf::Color({ 0, 0, 200 });
        //rect[3].color = sf::Color({ 0, 0, 200 });

        v.push_back(rect);
        //cout << "1\n";

        x_i += dx;
    }

    //cout << "1\n";
}


void draw_fancy(sf::RenderWindow& window, std::vector <sf::VertexArray>& fancy_lines, Camera camera)
{
    auto c = camera.calculate_position(0, 0);

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
            line[0].position = sf::Vector2f((float)c.first + i * camera.get_scale(), (float)c.second + 5);
            line[1].position = sf::Vector2f((float)c.first + i * camera.get_scale(), (float)c.second - 5);

            line[0].color = sf::Color({ 0, 0, 0 });
            line[1].color = sf::Color({ 0, 0, 0 });
            fancy_lines.push_back(line);
        }
        {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            line[0].position = sf::Vector2f((float)c.first + 5, (float)c.second - i * camera.get_scale());
            line[1].position = sf::Vector2f((float)c.first - 5, (float)c.second - i * camera.get_scale());

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

    //int *scale = &SCALE;

    Camera camera(-20, -580, SCALE, -10, 10, 200, 0, 3, 20, IntegralType::MiddleRectangle);
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
    std::vector <sf::ConvexShape> integral_rectangles;

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
                camera.change_scale(SCALE_CHANGE * d);
                
                //camera.set_scale(scale);
            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                moving_camera = false;
            }

            if (event->is<sf::Event::KeyPressed>())
            {
                if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::LShift)
                {
                    camera.change_sample(SAMPLE_CHANGE);
                    cout << camera.get_samples();
                }

                if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::LControl)
                {
                    camera.change_sample(-SAMPLE_CHANGE);
                    cout << camera.get_samples();
                }
            }

            if (moving_camera)
            {
                sf::Vector2i v = sf::Mouse::getPosition();
                camera.move(-v.x + previous_position.first, -v.y + previous_position.second);

                previous_position = std::make_pair(v.x, v.y);
            }
        }

        window.clear(sf::Color({ 200, 200, 200 }));

        //circle.setPosition({ (float)c.first, (float)c.second });

        lines.clear();
        integral_rectangles.clear();
        f.add_graphic_to_vector(camera, lines);
        f.add_integral_to_vector(camera, integral_rectangles);

        draw_fancy(window, fancy_lines, camera);

        for (auto rect : integral_rectangles)
        {
            window.draw(rect);
        }

        for (auto line : lines)
        {
            window.draw(line);
        }

        window.display();
    }
}
