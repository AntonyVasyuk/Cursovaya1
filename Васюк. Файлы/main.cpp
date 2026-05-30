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


const int NUMBER_OF_FUNCTIONS = 7;
const int NUMBER_OF_PARAMS = 10;

enum class FunctionType
{
    Linear,
    Quadric,
    Qubic,
    Sin,
    Cos,
    div_Log,
    Sinx_x,
    X_x,
    X__x,
    A_x
};

ostream& operator << (ostream &out, FunctionType type)
{
    switch (type)
    {
    case FunctionType::Linear:
    {
        out << "Linear";
        break;
    }
    case FunctionType::Quadric:
    {
        out << "Quadric";
        break;
    }
    case FunctionType::Qubic:
    {
        out << "Qubic";
        break;
    }
    case FunctionType::Sin:
    {
        out << "Sin(x)";
        break;
    }
    case FunctionType::Cos:
    {
        out << "Cos(x)";
        break;
    }
    case FunctionType::div_Log:
    {
        out << "1 / ln(x)";
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
        out << "Sin(x) / x";
        break;
    }
    }
    return out;
}


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

    void change_integral_type(IntegralType t)
    {
        this->integral_type = t;
    }

    int get_samples() { return samples; } 

    int get_scale() { return this->scale; } 
};


class Function
{
private:
    FunctionType type;
    //float (&params)[10];
    std::function<float(float)> f;
    //float* parameters;

public:
    Function(FunctionType t, float (& params)[NUMBER_OF_PARAMS])
    {
        this->type = t;
        int size;
        //for (int i = 0; i < 10; i++)
        //{
        //    this->params[i] = parameters[i];
        //}
        //cout << "1\n";
        switch (t)
        {
        case FunctionType::Linear:
        {
            float &k = params[0], &b = params[1];
            this->f = [k, b](float x) { return k * x + b; };
            break;
        }
        case FunctionType::Quadric:
        {
            float &a = params[0], &b = params[1], &c = params[2];
            this->f = [a, b, c](float x) {return a * x * x + b * x + c; };
            break;
        }
        case FunctionType::Qubic:
        {
            float &a = params[0], &b = params[1], &c = params[2], &d = params[3];
            this->f = [a, b, c, d](float x) {return a * x * x * x + b * x * x + c * x + d; };
            break;
        }
        case FunctionType::Sin:
        {
            float &a = params[0], &k = params[1], &b = params[2], &c = params[3];
            this->f = [a, k, b, c](float x) {return a * std::sin(k * x + b) + c; };
            break;
        }
        case FunctionType::Cos:
        {
            float &a = params[0], &k = params[1], &b = params[2], &c = params[3];
            this->f = [a, k, b, c](float x) {return a * std::cos(k * x + b) + c; };
            break;
        }
        case FunctionType::div_Log:
        {
            float &l = params[0], &k = params[1], &b = params[2], &c = params[3];
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
        case FunctionType::X_x:
        {
            //float a = params[0], k = params[1], b = params[2], c = params[3];
            this->f = [](float x) {return std::pow(x, x); };
            break;
        }
        case FunctionType::X__x:
        {
            //float a = params[0], k = params[1], b = params[2], c = params[3];
            this->f = [](float x) {return std::pow(x, -x); };
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

    std::pair <float, float> calcualate_ranged_integral(Camera camera)
    {
        auto c = camera.get_integral_range();
        float x1 = c.first, x2 = c.second;
        int n = camera.get_integral_samples();
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


void change_function(sf::Keyboard::Scancode scancode, float (&params)[NUMBER_OF_FUNCTIONS][NUMBER_OF_PARAMS], Function &f)
{
    switch (scancode)
    {
    case sf::Keyboard::Scan::Num1:
    {
        Function f1(FunctionType::Linear, params[0]);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num2:
    {
        Function f1(FunctionType::Quadric, params[1]);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num3:
    {
        Function f1(FunctionType::Qubic, params[2]);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num4:
    {
        Function f1(FunctionType::Sin, params[3]);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num5:
    {
        Function f1(FunctionType::Cos, params[4]);
        f = f1;
        break;
    }

    case sf::Keyboard::Scan::Num6:
    {
        Function f1(FunctionType::div_Log, params[5]);
        f = f1;
        break;
    }
    case sf::Keyboard::Scan::Num7:
    {
        Function f1(FunctionType::Sinx_x, params[6]);
        f = f1;
        break;
    }
    case sf::Keyboard::Scan::Num8:
    {
        Function f1(FunctionType::X_x, params[6]);
        f = f1;
        break;
    }
    case sf::Keyboard::Scan::Num9:
    {
        Function f1(FunctionType::X__x, params[6]);
        f = f1;
        break;
    }

    }
}


ostream& operator <<(ostream& out, Function f)
{
    switch (f.get_type())
    {
    case FunctionType::Linear:
    {
        out << "y = kx + b";
        break;
    }
    case FunctionType::Quadric:
    {
        out << "y = ax^2 + bx + c";
        break;
    }
    case FunctionType::Qubic:
    {
        out << "y = ax^3 + bx^2 + cx + d";
        break;
    }
    case FunctionType::Sin:
    {
        out << "y = a * sin(kx + b) + c";
        break;
    }
    case FunctionType::Cos:
    {
        out << "y = a * cos(kx + b) + c";
        break;
    }
    case FunctionType::div_Log:
    {
        out << "y = (l / ln(kx + b)) + c";
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
        out << "sin(x) / x";
        break;
    }
    }
    return out;
}


void show_info(Function &f, float(&params)[NUMBER_OF_FUNCTIONS][NUMBER_OF_PARAMS], Camera &camera)
{
    cout << "Current function is: " << f;
    switch (f.get_type())
    {
        case FunctionType::Linear:
        {
            cout << "k, b = ";
            cout << params[0][0] << ' ' << params[0][1];
            break;
        }
        case FunctionType::Quadric:
        {
            cout << "a, b, c = ";
            cout << params[1][0] << ' ' << params[1][1] << ' ' << params[1][2];
            break;
        }
        case FunctionType::Qubic:
        {
            cout << "a, b, c, d = ";
            cout << params[2][0] << ' ' << params[2][1] << ' ' << params[2][2] << ' ' << params[2][3];
            break;
        }
        case FunctionType::Sin:
        {
            cout << "a, k, b, c = ";
            cout << params[3][0] << ' ' << params[3][1] << ' ' << params[3][2] << ' ' << params[3][3];
            break;
        }
        case FunctionType::Cos:
        {
            cout << "a, b, c, d = ";
            cout << params[4][0] << ' ' <<  params[4][1] << ' ' << params[4][2] <<  ' ' << params[4][3];
            break;
        }
        case FunctionType::div_Log:
        {
            cout << "l, k, b, c = ";
            cout << params[5][0] << ' ' << params[5][1] << ' ' << params[5][2] << ' ' << params[5][3];
            break;
        }
    }
    cout << "\n\tCurrent parameteres are:\n";
    cout << "Range from " << camera.get_integral_range().first << " to " << camera.get_integral_range().second << '\n';
    cout << "Accuracy " << camera.get_integral_samples() << " samples\n";
}


void calculate_integral_and_print(Function &f, float(&params)[NUMBER_OF_FUNCTIONS][NUMBER_OF_PARAMS], Camera &camera)
{
    show_info(f, params, camera);
    cout << "\tThe current function integral equals:\n";
    std::pair <float, float> integrals = f.calcualate_ranged_integral(camera);
    cout << "Middle rectangle method: " << integrals.first << '\n';
    cout << "Trapezoid method: " << integrals.second << '\n';
    cout << "Real integral value: ";

    auto range = camera.get_integral_range();
    float x1 = range.first, x2 = range.second;
    bool flag = true;
    std::function<float(float)> func;
    //f = [k, b](float x) { return k * x * x / 2 + b * x; };

    switch (f.get_type())
    {
        case FunctionType::Linear:
        {
            float k = params[0][0], b = params[0][1];
            func = [k, b](float x) { return k * x * x / 2 + b * x; };
            break;
        }
        case FunctionType::Quadric:
        {
            float& a = params[1][0], & b = params[1][1], & c = params[1][2];
            func = [a, b, c](float x) {return a * x * x * x / 3 + b * x * x / 2 + c * x; };
            break;
        }
        case FunctionType::Qubic:
        {
            float& a = params[2][0], & b = params[2][1], & c = params[2][2], & d = params[2][3];
            func = [a, b, c, d](float x) {return a * x * x * x * x / 4 + b * x * x * x / 3 + c * x * x / 2 + d * x; };
            break;
        }
        case FunctionType::Sin:
        {
            float& a = params[3][0], & k = params[3][1], & b = params[3][2], & c = params[3][3];
            func = [a, k, b, c](float x) {return -a * std::cos(k * x + b) / k + c * x; };
            break;
        }
        case FunctionType::Cos:
        {
            float& a = params[4][0], & k = params[4][1], & b = params[4][2], & c = params[4][3];
            func = [a, k, b, c](float x) {return a * std::sin(k * x + b) / k + c * x; };
            break;
        }
        default:
            cout << "Can't calculate!";
            flag = false;
    }
    if (flag)
    {
        cout << func(x2) - func(x1);
    }
    cout << '\n';
}


void made_actions(Function& f, float(&params)[NUMBER_OF_FUNCTIONS][NUMBER_OF_PARAMS], Camera& camera, bool &is_console_mode)
{
    cout << "\n\nWhat do you want to do (the actions will appear on window after the option is done)?";
    cout << "\n 1. Calculate integral and show all information";
    cout << "\n 2. Set function coefficients";
    cout << "\n 3. Set function type";
    cout << "\n 4. Set function range";
    cout << "\n 5. Set function accuracy";

    //cout << "\n 4. Set integral type";
    cout << "\n 6. Set integral range";
    cout << "\n 7. Set integral accuracy";
    cout << "\n q. Quit console mode\t";
    //cout << "\n 1. Set ";
    //cout << "\n 1. Set ";

    char c;
    cin >> c;

    switch (c)
    {
    case '1':
        calculate_integral_and_print(f, params, camera);
        break;
    case '2':
        show_info(f, params, camera);
        cout << "\n Enter coefficients divided by space in following order: ";
        switch (f.get_type())
        {
            case FunctionType::Linear:
            {
                cout << "k, b\t";
                cin >> params[0][0] >> params[0][1];
                //cout << params[0][0] << params[0][1];
                Function f1(f.get_type(), params[0]);
                f = f1;
                break;
            }
            case FunctionType::Quadric:
            {
                cout << "a, b, c\t";
                cin >> params[1][0] >> params[1][1] >> params[1][2];
                Function f1(f.get_type(), params[1]);
                f = f1;
                break;
            }
            case FunctionType::Qubic:
            {
                cout << "a, b, c, d\t";
                cin >> params[2][0] >> params[2][1] >> params[2][2] >> params[2][3];
                Function f1(f.get_type(), params[2]);
                f = f1;
                break;
            }
            case FunctionType::Sin:
            {
                cout << "a, k, b, c\t";
                cin >> params[3][0] >> params[3][1] >> params[3][2] >> params[3][3];
                Function f1(f.get_type(), params[3]);
                f = f1;
                break;
            }
            case FunctionType::Cos:
            {
                cout << "a, b, c, d\t";
                cin >> params[4][0] >> params[4][1] >> params[4][2] >> params[4][3];
                Function f1(f.get_type(), params[4]);
                f = f1;
                break;
            }
            case FunctionType::div_Log:
            {
                cout << "l, k, b, c\t";
                cin >> params[5][0] >> params[5][1] >> params[5][2] >> params[5][3];
                Function f1(f.get_type(), params[5]);
                f = f1;
                break;
            }
        }

        break;
    case '3':
        cout << "Which function type you want (current is " << f.get_type() << ")? ";
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
    case '4':
    {
        cout << "Enter new range for function (left and right divided by space) (current is from " << camera.get_range().first << " to " << camera.get_range().second << "):\t";
        float x1, x2;
        cin >> x1 >> x2;
        camera.set_range(x1, x2);
        break;
    }
    case '5':
    {
        cout << "Enter accuracy for function (current is " << camera.get_samples() << "):\t";
        int n;
        cin >> n;
        camera.change_sample(n);
        break;
    }
    case '6':
    {
        cout << "Enter new range for integral (left and right divided by space) (current is from " << camera.get_integral_range().first << " to " << camera.get_integral_range().second << "):\t";
        float x1, x2;
        cin >> x1 >> x2;
        camera.set_integral_range(x1, x2);
        break;
    }
    case '7':
    {
        cout << "Enter accuracy for integral (current is " << camera.get_integral_samples() << "):\t";
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


void console_mode(Function &f, float(&params)[NUMBER_OF_FUNCTIONS][NUMBER_OF_PARAMS], Camera &camera, bool &is_console_mode)
{
    cout << "\n\n\t Console mode is now ON\n ";
    //cout << "To exit console mode and return to window enter \"q\"\n ";

    //calculate_integral_and_print(f, params, camera);

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
    cout << "\n > By pressing Space you can toggle continuous printing of integral - first number is middle rectangles method, second - trapezoid";
    cout << "\n > By pressing S you can toggle type of integral that will be drawn";
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

    float params[NUMBER_OF_FUNCTIONS][NUMBER_OF_PARAMS];
    for (int i = 0; i < NUMBER_OF_FUNCTIONS; i++)
    {
        for (int j = 0; j < NUMBER_OF_PARAMS; j++)
        {
            params[i][j] = 0;
        }
    }

    {
        params[0][0] = 2;
        params[0][1] = 2;

        params[1][0] = 1;
        params[1][1] = 0;
        params[1][2] = 0;

        params[2][0] = 1;
        params[2][1] = 0;
        params[2][2] = 0;
        params[2][3] = 0;

        params[3][0] = 1;
        params[3][1] = 1;
        params[3][2] = 0;
        params[3][3] = 0;

        params[4][0] = 1;
        params[4][1] = 1;
        params[4][2] = 0;
        params[4][3] = 0;

        params[5][0] = 1;
        params[5][1] = 1;
        params[5][2] = 0;
        params[5][3] = 0;
    }

    bool is_console_mode = false;
    bool print_integrals = false;
    //int mode = 1;

    Function f(FunctionType::Linear, params[0]);

    //int scale = SCALE;

    std::vector <sf::VertexArray> lines;
    std::vector <sf::VertexArray> fancy_lines;
    std::vector <sf::ConvexShape> integral_rectangles;

    //f.add_graphic_to_vector(camera, lines, -2, 2, 10);

    greetings_ENG();
    //greetings_RUS();

    while (window.isOpen())
    {
        if (print_integrals)
        {
            cout << f.calcualate_ranged_integral(camera).first << ' ' << f.calcualate_ranged_integral(camera).second << '\n';
        }

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
                    print_integrals = false;
                    console_mode(f, params, camera, is_console_mode);
                    window.requestFocus();
                }
                else if (key->scancode == sf::Keyboard::Scan::Space)
                {
                    if (print_integrals)
                    {
                        print_integrals = false;
                        greetings_ENG();
                    }
                    else
                    {
                        print_integrals = true;
                    }
                }
                else if (key->scancode == sf::Keyboard::Scan::S)
                {
                    if (camera.get_integral_type() == IntegralType::MiddleRectangle)
                    {
                        camera.change_integral_type(IntegralType::Trapezoid);
                    }
                    else
                    {
                        camera.change_integral_type(IntegralType::MiddleRectangle);
                    }
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
