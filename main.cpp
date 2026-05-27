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
int RANGE_CHANGE = 1;
int INTEGRAL_SAMPLE_CHANGE = 5; 


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

    void set_range(float d1, float d2)
    {
        this->range = std::make_pair(d1, d2);
    }

    void change_range(float d1, float d2)
    {
        this->range = std::make_pair(this->range.first + d1, this->range.second + d2);
    }

    std::pair<float, float> get_integral_range() { return integral_range; }

    void set_integral_range(float d1, float d2)
    {
        this->integral_range = std::make_pair(d1, d2);
    }

    void change_integral_range(float d1, float d2)
    {
        this->integral_range = std::make_pair(this->integral_range.first + d1, this->integral_range.second + d2);
    }

    int get_integral_samples() { return integral_samples; }

    IntegralType get_integral_type() { return integral_type; }

    void change_integral_sample(int d)
    {
        if (this->integral_samples + d > 0)
        {
            this->integral_samples += d;
        }
    }

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

    Function& operator = (const Function& other)
    {
        type = other.type;
        f = other.f;

        return *this;
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


void change_function(sf::Keyboard::Scancode scancode, float (&params)[10], Function &f)
{
    switch (scancode)
    {
    case sf::Keyboard::Scan::Num1:
    {
        params[0] = 2;
        params[1] = 2;

        Function f1(FunctionType::Linear, params);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num2:
    {
        params[0] = 1;
        params[1] = 0;
        params[2] = 0;

        Function f1(FunctionType::Quadric, params);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num3:
    {
        params[0] = 1;
        params[1] = 0;
        params[2] = 0;
        params[3] = 0;

        Function f1(FunctionType::Qubic, params);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num4:
    {
        params[0] = 1;
        params[1] = 1;
        params[2] = 0;
        params[3] = 0;

        Function f1(FunctionType::Sin, params);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num5:
    {
        params[0] = 1;
        params[1] = 1;
        params[2] = 0;
        params[3] = 0;

        Function f1(FunctionType::Cos, params);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num6:
    {
        params[0] = 1;
        params[1] = 1;
        params[2] = 0;
        params[3] = 0;

        Function f1(FunctionType::div_Log, params);
        f = f1;
        break;
    }
    case sf::Keyboard::Scan::Num7:
    {
        Function f1(FunctionType::Sinx_x, params);
        f = f1;
        break;
    }

    }
}


void calculate_integral_and_print(Function &f, Camera &camera)
{

}


void made_actions(Function& f, float(&params)[10], Camera& camera, bool &is_console_mode)
{
    cout << "\n\nWhat do you want to do (the actions will appear after the option is done)?";
    cout << "\n 0. Calculate integral and show all information";
    cout << "\n 1. Set function type";
    cout << "\n 2. Set function range";
    cout << "\n 3. Set function accuracy";

    //cout << "\n 4. Set integral type";
    cout << "\n 4. Set integral range";
    cout << "\n 5. Set integral accuracy";
    cout << "\n q. Quit console mode\t";
    //cout << "\n 1. Set ";
    //cout << "\n 1. Set ";

    char c;
    cin >> c;

    switch (c)
    {
    case '0':
        calculate_integral_and_print(f, camera);
        break;
    case '1':
        cout << "Which function type you want?";
        cout << "\n 1. Linear\n 2. Quadric\n 3. Qubic\n 4. Sin(x)\n 5. Cos(x)\n 6. 1 / ln(x)\n 7. Sin(x) / x\n ";
        sf::Keyboard::Scancode scancode;

        cin >> c;

        switch (c)
        {
        case '1':
            scancode = sf::Keyboard::Scan::Num1;
            break;
        case '2':
            scancode = sf::Keyboard::Scan::Num2;
            break;
        case '3':
            scancode = sf::Keyboard::Scan::Num3;
            break;
        case '4':
            scancode = sf::Keyboard::Scan::Num4;
            break;
        case '5':
            scancode = sf::Keyboard::Scan::Num5;
            break;
        case '6':
            scancode = sf::Keyboard::Scan::Num6;
            break;
        case '7':
            scancode = sf::Keyboard::Scan::Num7;
            break;
        }

        change_function(scancode, params, f);
        break;
    case '2':
    {
        cout << "Enter new range for function (left and right divided by space):\t";
        float x1, x2;
        cin >> x1 >> x2;
        camera.set_range(x1, x2);
        break;
    }
    case '3':
    {
        cout << "Enter accuracy for function:\t";
        int n;
        cin >> n;
        camera.change_sample(n);
        break;
    }
    case '4':
    {
        cout << "Enter new range for integral (left and right divided by space):\t";
        float x1, x2;
        cin >> x1 >> x2;
        camera.set_integral_range(x1, x2);
        break;
    }
    case '5':
    {
        cout << "Enter accuracy for integral:\t";
        int n;
        cin >> n;
        camera.change_integral_sample(n);
        break;
    }
    case 'q':
        cout << "\n\t Console mode is now OFF\n ";
        is_console_mode = false;
        break;
    }
}


void console_mode(Function &f, float(&params)[10], Camera &camera, bool &is_console_mode)
{
    cout << "\n\n\t Console mode is now ON\n ";
    //cout << "To exit console mode and return to window enter \"q\"\n ";
    cout << "The current function integral equals:\n";

    calculate_integral_and_print(f, camera);

    made_actions(f, params, camera, is_console_mode);
}


void greetings_ENG()
{
    cout << "\tHello!\n In this app you can calculate ranged integrals and see different information in the little window.\n ";
    cout << "Functions that you can draw:";
    cout << "\n 1. Linear\n 2. Quadric\n 3. Qubic\n 4. Sin(x)\n 5. Cos(x)\n 6. 1 / ln(x)\n 7. Sin(x) / x\n ";
    cout << "Right now the window represents the graph of linear function y = x + 1\n ";
    cout << "To change function you should tap the number button(representing function number in list)\non your keyboard, the window must be in focus.";
    cout << "Controls that you have:";
    cout << "\n > By clicking and dragging mouse you can move the camera";
    cout << "\n > By scrolling mouse wheel you can change the scale";
    cout << "\n > By pressing \"=\" and \"-\" you can set the number of devidings (samples) (with wich the window is drawing integral)\nhigher or lower resspectively";
    cout << "\n > By adding CTRL to previous two buttons, you can do the same thing, but for drawing graph";
    cout << "\n > By pressing A and D you can move inregral's left bounder of range of calculating left or right, respectively";
    cout << "\n > By pressing Q and E you can do the same thing with right bounder";
    cout << "\n > By adding CTRL to previous four keys you can do same things with graph boundaries";
    cout << "\n > By pressing Enter you can enter console mode, where you can set all the settings by hand in the console\n";
}

//void greetings_RUS()
//{
//    cout << "\t Привет!";
//}


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Graphics");

    window.setFramerateLimit(60);

    //int *scale = &SCALE;

    Camera camera(-20, -580, SCALE, -10, 10, 200, 0, 3, 20, IntegralType::MiddleRectangle);
    bool moving_camera = false;
    std::pair<int, int> previous_position;

    float params[10];
    for (int i = 0; i < 10; i++) { params[i] = 0; }

    bool is_console_mode = false;
    //int mode = 1;

    params[0] = 2;
    params[1] = 2;

    Function f(FunctionType::Linear, params);

    //int scale = SCALE;

    std::vector <sf::VertexArray> lines;
    std::vector <sf::VertexArray> fancy_lines;
    std::vector <sf::ConvexShape> integral_rectangles;

    //f.add_graphic_to_vector(camera, lines, -2, 2, 10);

    greetings_ENG();
    //greetings_RUS();

    while (window.isOpen())
    {
        if (is_console_mode)
        {
            //console_mode(f, camera);
            made_actions(f, params, camera, is_console_mode);
            window.requestFocus();
        }

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

                auto key = event->getIf<sf::Event::KeyPressed>();

                if (key->scancode == sf::Keyboard::Scan::Hyphen)
                {
                    if (key->control)
                    {
                        camera.change_sample(-SAMPLE_CHANGE);
                        //cout << camera.get_samples();
                    }
                    else
                    {
                        camera.change_integral_sample(-INTEGRAL_SAMPLE_CHANGE);
                    }
                }
                else if (key->scancode == sf::Keyboard::Scan::Equal)
                {
                    if (key->control)
                    {
                        camera.change_sample(SAMPLE_CHANGE);
                        //cout << camera.get_samples();
                    }
                    else
                    {
                        camera.change_integral_sample(INTEGRAL_SAMPLE_CHANGE);
                    }
                }
                else if (key->scancode == sf::Keyboard::Scan::D)
                {
                    if (!key->control)
                    {
                        camera.change_integral_range(RANGE_CHANGE, 0);
                        //cout << camera.get_samples();
                    }
                    else
                    {
                        camera.change_range(RANGE_CHANGE, 0);
                    }
                }
                else if (key->scancode == sf::Keyboard::Scan::A)
                {
                    if (!key->control)
                    {
                        camera.change_integral_range(-RANGE_CHANGE, 0);
                        //cout << camera.get_samples();
                    }
                    else
                    {
                        camera.change_range(-RANGE_CHANGE, 0);
                    }
                }
                else if (key->scancode == sf::Keyboard::Scan::E)
                {
                    if (!key->control)
                    {
                        camera.change_integral_range(0, RANGE_CHANGE);
                        //cout << camera.get_samples();
                    }
                    else
                    {
                        camera.change_range(0, RANGE_CHANGE);
                    }
                }
                else if (key->scancode == sf::Keyboard::Scan::Q)
                {
                    if (!key->control)
                    {
                        camera.change_integral_range(0, -RANGE_CHANGE);
                        //cout << camera.get_samples();
                    }
                    else
                    {
                        camera.change_range(0, -RANGE_CHANGE);
                    }
                }
                else if (key->scancode == sf::Keyboard::Scan::Enter)
                {
                    is_console_mode = true;
                    console_mode(f, params, camera, is_console_mode);
                    window.requestFocus();
                }
                else
                {
                    change_function(key->scancode, params, f);
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

        //draw_info(window, camera, f);

        window.display();
    }
}
